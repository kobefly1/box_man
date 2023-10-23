
#include"database1.h"
#include<mysql.h>
#include<stdio.h>
#include<iostream>
#define DB_NAME "box_man"
#define DB_HOST "192.168.79.1"
#define DB_PORT 3306
#define DB_USER "root"
#define DB_USER_PASSWD "1234"


static int debug = 1;



static bool connect_db(MYSQL& mysql);




bool fetch_user_info(userinfo& user)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char sql[256];
	bool ret = false;

	//1.���ӵ����ݿ�
	if (connect_db(mysql) == false) {
		return false;
	}

	//2.�����û����������ȡ�û���Ϣ
	snprintf(sql, 256, "select id,level_id from users where username = '%s' and password=md5('%s');", user.username.c_str(), user.passwd.c_str());
	ret = mysql_query(&mysql, sql);
	//printf("%s\n", sql);
	if (ret) {
		printf("���ݿ��ѯ����,%s ����ԭ��: %s\n", sql, mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	//3.��ȡ�����
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) {//û�в鵽��¼
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}
	user.id = atoi(row[0]);
	user.level_id = atoi(row[1]);
	if(debug)printf("userid: %d level_id:%d\n", user.id, user.level_id); //��ӡid
	//4.���ؽ��

	//�ͷŽ����
	mysql_free_result(res);

	//�ر����ݿ�
	mysql_close(&mysql);

	return true;
}

bool connect_db(MYSQL& mysql) {

	//1.��ʼ�����ݿ���
	mysql_init(&mysql);

	//2.�����ַ�����
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	if (mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_USER_PASSWD, DB_NAME, DB_PORT, NULL, 0)==NULL)
	{
		printf("���ӳ���,����ԭ��:%s\n", mysql_error(&mysql));
		return false;
	}
	return true;
}
/************************
����:ͨ���û�����������Դ����ݿ��ȡ�û���Ϣ
����:
	user - �û���Ϣ�ṹ��

����ֵ:
	��ȡ�ɹ�����true,ʧ��false
***********************/

bool fetch_level_info(levelinfo& level, int level_id) {
	MYSQL mysql;
	MYSQL_RES* res;//��ѯ�����
	MYSQL_ROW row;//��¼�ṹ��
	char sql[256];
	bool ret = false;

	//1.���ӵ����ݿ�
	if (connect_db(mysql) == false) {
		return false;
	}
	snprintf(sql,256,"select name,map_row,map_column,map_data,next_level_id \
		from levels where id = %d",level_id);
	
	ret = mysql_query(&mysql, sql);

	if (ret)
	{
		printf("���ݿ��ѯ����,%s ����ԭ��: %s\n", sql, mysql_error(&mysql));
	}
	
	//3.��ȡ���
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) {//û�в��ҵ���¼
		mysql_free_result(res);
		mysql_close(&mysql);
		//cout << 1 << endl;
		return false;
	}

	level.id = level_id;
	level.name = row[0];
	level.map_row = atoi(row[1]);
	level.map_column = atoi(row[2]);
	level.map_data = row[3];
	level.next_level = atoi(row[4]);
	//cout << level.map_data << endl;
	if (debug)
		printf("level id: %d  name: %s  map_row: %d  map_column: %d  map_data: %s  next_level: %d\n", level.id, level.name.c_str(), level.map_row, level.map_column, level.map_data.c_str(), level.next_level);
	
	//�ͷŽ����
	mysql_free_result(res);

	//�ر����ݿ�
	mysql_close(&mysql);

	return true;
		
}

bool transform_map_db2array(levelinfo& level, int Map[LINE][COLUMN])
{
	if (level.map_row > LINE || level.map_column > COLUMN) {
		printf("��ͼ����,����������!\n");
		return false;
	}

	if (level.map_data.length() < 1) {
		printf("��ͼ��������,����������!\n");
		return false;
	}
	int row = 0;
	
	int start = 0, end = 0;
	
	do {
		end = level.map_data.find('|', start);
		if (end < 0) {
			end = level.map_data.length();
		}
		if (start >= end)break;

		string line = level.map_data.substr(start, end - start);
		//printf("get line:%s\n", line.c_str());

		//�������ݽ��н���
		for (int i = 0; i < line.size(); i += 2)
		{
			Map[row][i/2] = line[i]-'0';
			
		}
		//cout << endl;
		row++;
		start = end + 1;
	} while (1);
}

//����
bool update_user_level(userinfo& user, int next_level_id)
{
	MYSQL mysql;
	MYSQL_RES* res; //��ѯ�����
	MYSQL_ROW row; //��¼�ṹ��
	char sql[256];
	bool ret = false;

	//1.���ӵ����ݿ�
	if (connect_db(mysql) == false) {
		
		return false;
	}
	
	//2.�����û�id ������һ��id
	snprintf(sql, 256, "update users set level_id  = %d where id = %d;", next_level_id, user.id);

	ret = mysql_query(&mysql, sql);

	if (ret) {
		printf("���ݿ���³���,%s ����ԭ��: %s", sql, mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	mysql_close(&mysql);
	return true;
}