#pragma once
#include "hero.h"

class c_map_door
{
public:
	void init(int);//初始化
	void show(int,int);//渲染
	bool open();//开某个普通门
	int getType() {return type;} //门的类型
private:
	int type;
	int state;
	hgeSprite *door[4];
};

class c_map_point
{
public:
	void init(int);//初始化
	int getId();
	void show(GfxFont*,int,int);//渲染
	void animation() {monster.changeState();}//npc状态改变
	void printMonsterInfo() {monster.printInfo();} //输出怪物信息
	bool canMove(int);//判断该点是否可移动
	void distroyItem() {item=0;} //获取该点物品
	int getItem() {return item;}
	hgeSprite* getTypeSprite();
	hgeSprite* getItemSprite();
private:
	int type;
	int item;
	c_monster monster;//敌人类型
	c_map_door door;//门类型
};

class c_map_floor
{
public:
	void init(int,int[30][30]);//初始化
	void show();//渲染
	void animation();//状态改变
	void printMonsterInfo(int,int);//输出怪物信息
	bool canMove(int,int,int);//判断该地图的某点是否可移动
	int getItem(int,int);//获取地图某点的物品
	void getDownPosition(int&,int&);//找到地图的下楼楼梯
	void getUpPosition(int&,int&);//找到地图的上楼楼梯
	c_map_point* getinfo(int x, int y) {return &info[x][y];}
	void output(FILE*);
private:
	int id;//楼层编号
	int dx,dy,ux,uy;//楼梯坐标
	c_map_point info[30][30];//每点信息
};