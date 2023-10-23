 #pragma once

#include<string>
using namespace std;

typedef struct _userinfo {
	int id;//ÓÃ»§id;
	string username;
	string passwd;
	int level_id; //¹Ø¿¨id
}userinfo;


bool fetch_user_info(userinfo &user);
