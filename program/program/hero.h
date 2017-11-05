#pragma once

#include "monster.h"
class c_hero
{
public:
	const static int MAX_DAMAGE = 999999999;

	c_hero();//构造函数
	void init(FILE*);
	int nextX();//下一步的横坐标
	int nextY();//下一步的纵坐标
	bool canMove(int);//是否可以朝某个方向移动
	bool moveComplete();//移动是否完成
	bool openRedDoor();//打开红色的门
	bool openBlueDoor();//打开蓝色的门
	bool openYellowDoor();//打开黄色的门
	void upstair();//上楼
	void downstair();//下楼
	void turn(); // 转向
	void show();
	void printInfo();//输出勇士信息
	bool canBeat(c_monster*);//能否打败怪物
	int getDamage(c_monster*);//打败怪物普通伤害
	int getDamage(int h_atk, int h_def, int h_mdef, int m_hp, int m_atk, int m_def, int m_spe);
	int getCritical(int m_hp, int m_atk, int m_def, int m_spe);
	int getCriticalDamage(int m_hp, int m_atk, int m_def, int m_spe);
	int getDefDamage(int m_hp, int m_atk, int m_def, int m_spe);
	void beat(c_monster*);//打怪物
	int getNowFloor() {return now_floor;}
	int getHP() {return hp;}
	int getAtk() {return atk;}
	int getDef() {return def;}
	void useShop(int);
	void output(FILE*);
	int dir[2][4];
private:
	int hp,atk,def,mdef,money,redkey,bluekey,yellowkey;
	int x,y,move,face,now_floor;
	hgeSprite* sprites[4][4];
};
