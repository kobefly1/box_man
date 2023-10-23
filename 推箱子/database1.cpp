
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

	//1.连接到数据库
	if (connect_db(mysql) == false) {
		return false;
	}

	//2.根据用户名和密码获取用户信息
	snprintf(sql, 256, "select id,level_id from users where username = '%s' and password=md5('%s');", user.username.c_str(), user.passwd.c_str());
	ret = mysql_query(&mysql, sql);
	//printf("%s\n", sql);
	if (ret) {
		printf("数据库查询出错,%s 错误原因: %s\n", sql, mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	//3.获取结果集
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) {//没有查到记录
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}
	user.id = atoi(row[0]);
	user.level_id = atoi(row[1]);
	if(debug)printf("userid: %d level_id:%d\n", user.id, user.level_id); //打印id
	//4.返回结果

	//释放结果集
	mysql_free_result(res);

	//关闭数据库
	mysql_close(&mysql);

	return true;
}

bool connect_db(MYSQL& mysql) {

	//1.初始化数据库句柄
	mysql_init(&mysql);

	//2.设置字符编码
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	if (mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_USER_PASSWD, DB_NAME, DB_PORT, NULL, 0)==NULL)
	{
		printf("连接出错,错误原因:%s\n", mysql_error(&mysql));
		return false;
	}
	return true;
}
/************************
功能:通过用户名和密码可以从数据库获取用户信息
输入:
	user - 用户信息结构体

返回值:
	获取成功返回true,失败false
***********************/

bool fetch_level_info(levelinfo& level, int level_id) {
	MYSQL mysql;
	MYSQL_RES* res;//查询结果集
	MYSQL_ROW row;//记录结构体
	char sql[256];
	bool ret = false;

	//1.连接到数据库
	if (connect_db(mysql) == false) {
		return false;
	}
	snprintf(sql,256,"select name,map_row,map_column,map_data,next_level_id \
		from levels where id = %d",level_id);
	
	ret = mysql_query(&mysql, sql);

	if (ret)
	{
		printf("数据库查询出错,%s 错误原因: %s\n", sql, mysql_error(&mysql));
	}
	
	//3.获取结果
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) {//没有查找到记录
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
	
	//释放结果集
	mysql_free_result(res);

	//关闭数据库
	mysql_close(&mysql);

	return true;
		
}

bool transform_map_db2array(levelinfo& level, int Map[LINE][COLUMN])
{
	if (level.map_row > LINE || level.map_column > COLUMN) {
		printf("地图超大,请重新设置!\n");
		return false;
	}

	if (level.map_data.length() < 1) {
		printf("地图数据有误,请重新设置!\n");
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

		//对行数据进行解析
		for (int i = 0; i < line.size(); i += 2)
		{
			Map[row][i/2] = line[i]-'0';
			
		}
		//cout << endl;
		row++;
		start = end + 1;
	} while (1);
}

//更新
bool update_user_level(userinfo& user, int next_level_id)
{
	MYSQL mysql;
	MYSQL_RES* res; //查询结果集
	MYSQL_ROW row; //记录结构体
	char sql[256];
	bool ret = false;

	//1.连接到数据库
	if (connect_db(mysql) == false) {
		
		return false;
	}
	
	//2.根据用户id 更新下一关id
	snprintf(sql, 256, "update users set level_id  = %d where id = %d;", next_level_id, user.id);

	ret = mysql_query(&mysql, sql);

	if (ret) {
		printf("数据库更新出错,%s 错误原因: %s", sql, mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	mysql_close(&mysql);
	return true;
}