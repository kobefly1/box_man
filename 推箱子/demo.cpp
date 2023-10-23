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
//���� ��.��,��,�� ���Ʒ���,'q'�˳�

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

POS man;//С��ʵʱ��λ��

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
	settextstyle(20, 0, "����");
	drawtext("��ϲ��,���������һ��",&rec,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//::system("pause");
	Sleep(1000);
	cleardevice();
}
void gameOverScene(IMAGE* bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	settextstyle(20, 0, "����");
	drawtext("��ϲ��,�ɹ�ͨ��!", &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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

	//С��ǰ��һ������ǵذ�,��ǰ��һ��()
	if (map[next_pos.x][next_pos.y] == FLOOR) {
		changeMap(&next_pos,MAN);
		changeMap(&man, FLOOR);
		man = next_pos;
	}
	//����ǰ������
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
		cout << "�������û���:";
		cin >> user.username;

		cout << "����������:";
		cin >> user.passwd;
		//����bool,�ɹ�true,ʧ��false
		ret = fetch_user_info(user);
		times++;
		if (times >= MAX_RETRY_TIMES)
		{
			//cout << "���Դ�����������" << endl;
			break;
		}
		if (ret == false) {
			cout << "��¼ʧ��,����������" << endl;
			
		}
	} while (!ret);
	return ret;
}

void init_game_graph(IMAGE &bg_img)
{

	//��̨��Ϸ
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);

	//���ñ���
	loadimage(&bg_img, "blackground.bmp", SCREEN_WIDTH, SCREEN_HEIGHT, true);
	putimage(0, 0, &bg_img);

	//ǽ:0,�ذ�:1,����Ŀ�ĵ�:2,С��:3,����:4,��������Ŀ��:5
	loadimage(&images[WALL], "wall_right.bmp", RATIO, RATIO, true);
	loadimage(&images[FLOOR], "floor.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX_DES], "des.bmp", RATIO, RATIO, true);
	loadimage(&images[MAN], "man.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX], "box.bmp", RATIO, RATIO, true);
	loadimage(&images[HIT], "box.bmp", RATIO, RATIO, true);
}
int main()
{
	//�û������֤
	userinfo user;
	levelinfo level;
	IMAGE bg_img;
	bool ret = false;
	if (!login(user)) {
		cout << "��¼ʧ��,�����µ�¼" << endl;
		::system("pause");
		exit(-1);
	}

	cout << "��½�ɹ�" << endl;
	::system("pause");
	

	init_game_graph(bg_img);

	//��Ϸ����
	bool quit = false;
	do {
		//�����û����ڵ�id��ȡ�ؿ�����
		ret = fetch_level_info(level, user.level_id);
		//::system("pause");
		if (!ret) {
			cout << "��ȡ�ؿ�����ʧ��,������!" << endl;
			::system("pause");
			exit(-1);
		}
		

		//�������еĵ�ͼ����ת����map��.
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
					
					//�����û���һ�صĹؿ���Ϣ
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

