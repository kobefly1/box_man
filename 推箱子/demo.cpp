#include<graphics.h>
#include<conio.h>
#include<iostream>

#include"database1.h"
using namespace std;



#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 650

#define LINE 48
#define COLUMN 48

#define RATIO 50

#define START_X 50
#define START_Y 50

typedef enum _DIRECTION DIRECTION;
typedef struct _POS POS;
typedef enum _PROPS PROPS;
//控制 上.下,左,右 控制方向,'q'退出

#define KEY_UP 'w'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_DOWN 's'
#define KEY_QUIT 'q'


#define MAX_RETRY_TIMES 4
enum _PROPS {
	WALL,
	FLOOR,
	BOX_DES,
	MAN,
	BOX,
	HIT,
	ALL
};
enum _DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};




struct _POS {
	int x;
	int y;
};

int map[LINE][COLUMN] = { 0 };
/*int map[LINE][COLUMN]
{
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,1,1,1,1,1,1,0,0},
	{0,1,4,1,0,2,1,0,2,1,0,0},
	{0,1,0,1,0,1,0,0,1,1,1,0},
	{0,1,0,2,0,1,1,4,1,1,1,0},
	{0,1,1,1,0,3,1,1,1,4,1,0},
	{0,1,2,1,1,4,1,1,1,1,1,0},
	{0,1,0,0,1,0,1,1,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};*/
//0,0,0,0,0,0,0,0,0,0,0,0|0,1,0,1,1,1,1,1,1,1,0,0|0,1,4,1,0,2,1,0,2,1,0,0|0,1,0,1,0,1,0,0,1,1,1,0|
//0,1,0,2,0,1,1,4,1,1,1,0|0,1,1,1,0,3,1,1,1,4,1,0|0,1,2,1,1,4,1,1,1,1,1,0|
//0,1,0,0,1,0,1,1,0,0,1,0|0,0,0,0,0,0,0,0,0,0,0,0
IMAGE images[ALL];

POS man;//小人实时的位置

void changeMap(POS* pos, PROPS prop) {
	map[pos->x][pos->y] = prop;
	putimage(START_X + pos->y * RATIO, START_Y+pos->x * RATIO, &images[prop]);
}

bool isGameOver() {
	for(int i = 0;i<LINE;i++)
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == BOX_DES)return false;
		}
	return true;
}

void gameNextScene(IMAGE* bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	settextstyle(20, 0, "宋体");
	drawtext("恭喜您,点击进入下一关",&rec,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//::system("pause");
	Sleep(1000);
	cleardevice();
}
void gameOverScene(IMAGE* bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	settextstyle(20, 0, "宋体");
	drawtext("恭喜您,成功通关!", &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void gameControl(DIRECTION direct){
	POS next_pos = man, next_next_pos = man;
	switch (direct) {
	case UP:
		next_pos.x = man.x - 1;
		next_next_pos.x = man.x - 2;
		break;
	case DOWN:
		next_pos.x = man.x + 1;
		next_next_pos.x = man.x + 2;
		break;
	case LEFT:
		next_pos.y = man.y - 1;
		next_next_pos.y = man.y - 2;
		break;
	case RIGHT:
		next_pos.y = man.y + 1;
		next_next_pos.y = man.y + 2;
		break;
	}

	//小人前进一步如果是地板,人前进一步()
	if (map[next_pos.x][next_pos.y] == FLOOR) {
		changeMap(&next_pos,MAN);
		changeMap(&man, FLOOR);
		man = next_pos;
	}
	//箱子前是箱子
	if (map[next_pos.x][next_pos.y] == BOX) {

		if (map[next_next_pos.x][next_next_pos.y] == FLOOR) {
			changeMap(&next_next_pos, BOX);
			changeMap(&next_pos, MAN);
			changeMap(&man, FLOOR);
			man = next_pos;
		}
		else if (map[next_next_pos.x][next_next_pos.y] == BOX_DES){

			changeMap(&next_next_pos, HIT);
			changeMap(&next_pos, MAN);
			changeMap(&man, FLOOR);
			man = next_pos;
			
		}
	}
}

bool login(userinfo& user) {

	int times = 0;
	bool ret = false;
	do {
		cout << "请输入用户名:";
		cin >> user.username;

		cout << "请输入密码:";
		cin >> user.passwd;
		//返回bool,成功true,失败false
		ret = fetch_user_info(user);
		times++;
		if (times >= MAX_RETRY_TIMES)
		{
			//cout << "尝试次数超过限制" << endl;
			break;
		}
		if (ret == false) {
			cout << "登录失败,请重新输入" << endl;
			
		}
	} while (!ret);
	return ret;
}

void init_game_graph(IMAGE &bg_img)
{

	//搭台唱戏
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);

	//设置背景
	loadimage(&bg_img, "blackground.bmp", SCREEN_WIDTH, SCREEN_HEIGHT, true);
	putimage(0, 0, &bg_img);

	//墙:0,地板:1,箱子目的地:2,小人:3,箱子:4,箱子命中目标:5
	loadimage(&images[WALL], "wall_right.bmp", RATIO, RATIO, true);
	loadimage(&images[FLOOR], "floor.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX_DES], "des.bmp", RATIO, RATIO, true);
	loadimage(&images[MAN], "man.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX], "box.bmp", RATIO, RATIO, true);
	loadimage(&images[HIT], "box.bmp", RATIO, RATIO, true);
}
int main()
{
	//用户身份验证
	userinfo user;
	levelinfo level;
	IMAGE bg_img;
	bool ret = false;
	if (!login(user)) {
		cout << "登录失败,请重新登录" << endl;
		::system("pause");
		exit(-1);
	}

	cout << "登陆成功" << endl;
	::system("pause");
	

	init_game_graph(bg_img);

	//游戏环节
	bool quit = false;
	do {
		//根据用户所在的id获取关卡数据
		ret = fetch_level_info(level, user.level_id);
		//::system("pause");
		if (!ret) {
			cout << "获取关卡数据失败,请重试!" << endl;
			::system("pause");
			exit(-1);
		}
		

		//把数据中的地图数据转换到map中.
		ret = transform_map_db2array(level, map);
		//::system("pause");




		

		for (int i = 0; i < level.map_row; i++)
		{
			for (int j = 0; j < level.map_column; j++)
			{
				if (map[i][j] == MAN) {
					man.x = i;
					man.y = j;
				}
				putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[i][j]]);
			}
		}


		
		do {
			if (_kbhit()) {
				char ch = _getch();
				if (ch == KEY_UP) {
					gameControl(UP);

				}
				else if (ch == KEY_DOWN) {
					gameControl(DOWN);

				}
				else if (ch == KEY_LEFT) {
					gameControl(LEFT);

				}
				else if (ch == KEY_RIGHT) {
					gameControl(RIGHT);

				}
				else if (ch == KEY_QUIT) {
					quit = true;

				}
				if (isGameOver()) {
					if (level.next_level < 1) {
						gameOverScene(&bg_img);
						quit = true;
						break;
					}
					gameNextScene(&bg_img);
					
					//更新用户下一关的关卡信息
					if (update_user_level(user, level.next_level)) {
						//cout << level.next_level << endl;
						user.level_id = level.next_level;
					}
										
					break;
				}
			}
			Sleep(10);
		} while (quit == false);//!quit
		
	} while (quit ==false);
	::system("pause");
	closegraph();
	return 0;
}

