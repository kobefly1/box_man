#pragma once

#include<string>
using namespace std;

#define LINE 48
#define COLUMN 48
typedef struct _userinfo {
	int id;//用户id;
	string username;
	string passwd;
	int level_id; //关卡id
}userinfo;

typedef struct _levelinfo {
	int id;//关卡id
	string name;
	int map_row;
	int map_column;
	string map_data;
	int next_level;
}levelinfo;


bool fetch_user_info(userinfo& user);
bool fetch_level_info(levelinfo& level, int level_id);

bool transform_map_db2array(levelinfo& level, int Map[LINE][COLUMN]);
bool update_user_level(userinfo& user, int next_level_id);