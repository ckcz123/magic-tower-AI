#pragma once
#include "map.h"
#include <string>
#include <vector>

class constants
{
public:
	static const int MESSAGE_NONE = 0;
	static const int MESSAGE_QUIT = 1;
	static const int MESSAGE_RESTART = 2;
	static const int MESSAGE_SAVE = 3;
	static const int MESSAGE_LOAD = 4;
	static const int MESSAGE_WIN = 5;
	static const int MESSAGE_LOSE = 6;
	static const int MESSAGE_FLYING = 7;
	static const int MESSAGE_NPC = 8;
	static const int MESSAGE_HINT = 9;
	static const int MESSAGE_SHOP = 10;

	constants();
	void init(FILE*);
	void loadResources();
	void destroy();
	void setMsg(const wchar_t*[50]);
	void setMsg(const wchar_t*);
	bool isFree() {return !moving && !opening && !flooring && msg==MESSAGE_NONE && clock()-lasttime>150;}
	void goOn(c_hero*, c_map_floor*, float);
	void printInfo();
	void output(FILE*);

	int msg;
	vector<wstring> hint;
	int nowcnt;

	char route[3100];
	int pos;

	float playtime;
	int step;
	long lasttime;

	bool moving,opening,flooring;
	int map_floornum,map_height,map_width,ScreenLeft;

	int monster_map[300][10];

	int atk_val, def_val, mdef_val, redpotion_val, bluepotion_val, yellowpotion_val, greenpotion_val, sword_val, shield_val;

	// 正在打开的门
	c_map_door *map_openingdoor;

	// HGE
	HGE *hge;

	//各种纹理
	HTEXTURE ht_map,ht_icon,ht_hero,ht_monster,ht_skin;

	//各种图块
	hgeSprite *s_ground,*s_wall,*s_redjewel,*s_bluejewel,*s_redkey,*s_bluekey,*s_yellowkey,*s_greenjewel, *s_redpotion,*s_bluepotion,
		*s_yellowpotion, *s_greenpotion, *s_storey, *s_sword, *s_shield;
	hgeSprite *s_atk, *s_def, *s_mdef, *s_bg, *s_bg_font, *s_money;
	hgeSprite *s_heart, *s_damage, *s_time, *s_step,*s_reddoor,*s_bluedoor,*s_yellowdoor;
	hgeSprite *s_upstair, *s_downstair;

	// 文字
	hgeFont *hgef;

	// 其他常量
	float time_move,time_open,time_animation,time_floor;

};