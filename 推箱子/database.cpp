

#define DB_NAME "box_man"
#define DB_HOST "127.0.0.1"
#define DB_PORT 3306
#define DB_USER "root"
#define DB_USER_PASSWD "1234"

static bool connect_db(MYSQL& mysql);

#include"database.h"
#include<mysql.h>
#include<stdio.h>


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
	snprintf(sql, 256, "select id,level from users where username = 's' and password=md5('%s');",user.username.c_str(), user.passwd.c_str());
	ret = mysql_query(&mysql, sql);

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
	printf("userid: %d level_id:%d\n", user.id, user.level_id); //��ӡid
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
	mysql_options(&mysql,MYSQL_SET_CHARSET_NAME,"gbk");
	if (mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_USER_PASSWD, DB_NAME, DB_PORT, NULL, 0))
	{
		printf("���ӳ���,����ԭ��:%s\n", mysql_error(&mysql));
	}
	return true;
}