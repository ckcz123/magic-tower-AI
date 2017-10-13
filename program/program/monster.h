#pragma once
#include <hge.h>
#include <hgefont.h>
#include <hgesprite.h>
#include "gfxfont.h"
using namespace std;

class c_monster
{
public:
	void init(int);//初始化
	void show(GfxFont*,int,int);//渲染
	void printInfo();//输出信息
	void changeState();//怪物动画
	int getId() {return id;}
	int getHp() {return hp;}
	int getAtk();
	int getDef();
	int getSpecial() {return special;}
private:
	wchar_t name[30];
	int id,hp,atk,def,money,special;
	int state,position;
	hgeSprite *monster[4];
};