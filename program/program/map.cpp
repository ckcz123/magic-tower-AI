#include "stdafx.h"

extern c_map_floor map_floor[50];
extern c_hero hero;
extern constants consts;


void c_map_door::init(int t)
{
	type=t;
	if(t<=4)for(int i=0;i<4;i++) door[i]=new hgeSprite(consts.ht_map,32*(t-1),32*(i+2),32,32);
	if(t==7)for(int i=0;i<4;i++) door[i]=consts.s_upstair;
	if(t==8)for(int i=0;i<4;i++) door[i]=consts.s_downstair;
	state=0;
}
void c_map_door::show(int i,int j)
{
	if(type!=0) door[state]->Render(j*32+consts.ScreenLeft,i*32);
}
bool c_map_door::open()
{
	state++;
	if(state==3)
	{
		this->init(0);
		return true;
	}
	return false;
}

void c_map_point::init(int id)
{
	type=0; item=0; monster.init(0); door.init(0);
	if (id<=9) type=id;
	else if (id<=40) item=id;
	else if (id<=80) ;
	else if (id<=100) door.init(id-80);
	else if (id<=200) monster.init(id-100);
	else ;
}
int c_map_point::getId()
{
	if (monster.getId()!=0) return 100+monster.getId();
	if (door.getType()!=0) return 80+door.getType();
	if (item!=0) return item;
	return type;
}
void c_map_point::show(GfxFont* f,int i,int j)
{
	getTypeSprite()->Render(j*32+consts.ScreenLeft,i*32);
	hgeSprite* s_item=getItemSprite();
	if(s_item!=NULL) s_item->Render(j*32+consts.ScreenLeft,i*32);
	monster.show(f,i,j);
	door.show(i,j);
}
hgeSprite* c_map_point::getTypeSprite() {
	if (type==1) return consts.s_wall;
	/*
	if (type==2) return consts.s_wall2;
	if (type==3) return consts.s_water;
	if (type==4) return consts.s_sky;
	if (type==5) return consts.s_barrier;
	if (type==6) return consts.s_wall3;
	*/
	return consts.s_ground;
}
hgeSprite* c_map_point::getItemSprite()
{
	switch (item)
	{
	case 11: return consts.s_redjewel;
	case 12: return consts.s_bluejewel;
	case 13: return consts.s_greenjewel;
	case 15: return consts.s_yellowkey;
	case 16: return consts.s_bluekey;
	case 17: return consts.s_redkey;
	case 18: return consts.s_redpotion;
	case 19: return consts.s_bluepotion;
	case 20: return consts.s_yellowpotion;
	case 21: return consts.s_greenpotion;
	case 22: return consts.s_sword;
	case 23: return consts.s_shield;
	default:
		break;
	}
	return NULL;
}
bool c_map_point::canMove(int f)
{
	if(type==0)
	{
		if(door.getType()!=0)
		{
			bool c=false;
			switch(door.getType())
			{
			case 1:c=hero.openYellowDoor();break;
			case 2:c=hero.openBlueDoor();break;
			case 3:c=hero.openRedDoor();break;
			case 7:hero.upstair();break;
			case 8:hero.downstair();break;
			}
			if(c)
			{
				consts.opening=true;
				consts.map_openingdoor=&door;
			}
			return c;
		}
		if(monster.getId()!=0)
		{ 
			if (hero.canBeat(&monster))
				hero.beat(&monster);
			return false;
		}
		return true;
	}
	else return false;
}

void c_map_floor::init(int d,int ch[30][30])
{
	id=d;
	ux=-1; uy=-1; dx=-1; dy=-1;
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].init(ch[i][j]);
			if(ch[i][j]==87)
			{
				ux=j;
				uy=i;
			}
			if(ch[i][j]==88)
			{
				dx=j;
				dy=i;
			}
		}
	}
}
void c_map_floor::show()
{
	GfxFont *f=new GfxFont(L"¿¬Ìå",12,true);
	for(int i=0;i<consts.map_height;i++)
	{
		for(int j=0;j<consts.map_width;j++)
		{
			info[i][j].show(f,i,j);
		}
	}
	delete f;
}
void c_map_floor::animation()
{	
	for(int i=0;i<consts.map_height;i++)
		for(int j=0;j<consts.map_width;j++)
			info[i][j].animation();
}
void c_map_floor::printMonsterInfo(int i,int j)
{
	info[j][i].printMonsterInfo();
}
bool c_map_floor::canMove(int x,int y,int f)
{
	if (x>=consts.map_width || y>=consts.map_height || x<0 || y<0) return false;
	return info[y][x].canMove(f);
}
int c_map_floor::getItem(int x,int y)
{
	int i=info[y][x].getItem();
	info[y][x].distroyItem();
	return i;
}
void c_map_floor::getDownPosition(int &x,int &y)
{
	x=dx;
	y=dy;
}
void c_map_floor::getUpPosition(int &x,int &y)
{
	x=ux;
	y=uy;
}
