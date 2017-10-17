#include "stdafx.h"

constants::constants()
{
	hge=NULL;
	ScreenLeft=240;
}

void constants::init(FILE* file)
{
	moving=opening=flooring=false;
	playtime=0.0;
	step=0;
	time_move=time_open=time_animation=time_floor=0;

	fscanf_s(file,"%d%d%d%d%d%d%d%d%d", 
		&atk_val,&def_val,&mdef_val,&redpotion_val,&bluepotion_val,&yellowpotion_val,&greenpotion_val,
		&sword_val,&shield_val);
	int k;
	fscanf_s(file,"%d",&k);
	while (k--)
	{
		int id;
		fscanf_s(file,"%d",&id);
		fscanf_s(file,"%d%d%d%d%d",&monster_map[id][0],&monster_map[id][1],&monster_map[id][2],&monster_map[id][3],&monster_map[id][4]);
	}
	/*
	FILE* output_file;
	int err=fopen_s(&output_file,"output.txt","r");
	if (err!=0)
		strcpy_s(route,"");
	else {
		fgets(route,3100,output_file);
		fclose(output_file);
	}
	*/
	strcpy_s(route, "");
	pos=0;

}
void constants::loadResources()
{
	ht_map=hge->Texture_Load("Res/map.png");
	ht_icon=hge->Texture_Load("Res/icon.png");
	ht_hero=hge->Texture_Load("Res/hero.png");
	ht_monster=hge->Texture_Load("Res/monster.png");
	ht_skin=hge->Texture_Load("Res/skin.png");
	hgef=new hgeFont("Res/font.fnt");
	//ht_map
	s_ground=new hgeSprite(ht_map,0,0,32,32);
	s_wall=new hgeSprite(ht_map,32,0,32,32);
	s_bg=new hgeSprite(ht_skin,0,0,128,128);
	s_bg->SetColor(0xD0FFFFFF);
	s_bg_font=new hgeSprite(ht_skin,0,0,128,128);
	s_bg_font->SetColor(0x72FFFFFF);
	//ht_door
	s_yellowdoor=new hgeSprite(ht_map,0,64,32,32);
	s_bluedoor=new hgeSprite(ht_map,32,64,32,32);
	s_reddoor=new hgeSprite(ht_map,64,64,32,32);
	s_downstair=new hgeSprite(ht_map,0,192,32,32);
	s_upstair=new hgeSprite(ht_map,32,192,32,32);
	//ht_item
	s_yellowkey=new hgeSprite(ht_icon,0,0,32,32);
	s_bluekey=new hgeSprite(ht_icon,32,0,32,32);
	s_redkey=new hgeSprite(ht_icon,64,0,32,32);
	s_storey=new hgeSprite(ht_icon,64,96,32,32);
	s_money=new hgeSprite(ht_icon,32,192,32,32);
	s_heart=new hgeSprite(ht_icon,64,192,32,32);
	s_time=new hgeSprite(ht_icon,32,96,32,32);
	s_step=new hgeSprite(ht_icon,32,128,32,32);
	s_damage=new hgeSprite(ht_icon,64,160,32,32);
	s_bluejewel=new hgeSprite(ht_icon,64,32,32,32);
	s_redjewel=new hgeSprite(ht_icon,96,32,32,32);
	s_greenjewel=new hgeSprite(ht_icon,0,64,32,32);
	s_redpotion=new hgeSprite(ht_icon,64,64,32,32);
	s_bluepotion=new hgeSprite(ht_icon,96,64,32,32);
	s_yellowpotion=new hgeSprite(ht_icon, 96, 96, 32, 32);
	s_greenpotion=new hgeSprite(ht_icon, 96, 128, 32, 32);
	s_atk=new hgeSprite(ht_icon,0,32,32,32);
	s_def=new hgeSprite(ht_icon,32,32,32,32);
	s_mdef=new hgeSprite(ht_icon,96,160,32,32);
	s_sword=new hgeSprite(ht_icon,0,32,32,32);
	s_shield=new hgeSprite(ht_icon,32,32,32,32);

}
void constants::destroy()
{
	hge->Texture_Free(ht_map);
	hge->Texture_Free(ht_icon);
	hge->Texture_Free(ht_hero);
	hge->Texture_Free(ht_skin);
	hge->Texture_Free(ht_monster);
	delete hgef;
}
void constants::setMsg(const wchar_t* s[50])
{
	hint.clear();
	for (int i=0;i<50;i++) {
		if (s[i]==NULL) break;
		hint.push_back(wstring(s[i]));
	}

	lasttime=clock();
	nowcnt=0;
	msg=MESSAGE_HINT;
}
void constants::setMsg(const wchar_t* s) 
{
	hint.clear();
	hint.push_back(wstring(s));

	lasttime=clock();
	nowcnt=0;
	msg=MESSAGE_HINT;
}
void constants::goOn(c_hero* hero, c_map_floor* currFloor, float dt)
{
	playtime+=dt;
	if(moving) // 移动
	{
		time_move+=dt;
		if(time_move>=0.03)
		{
			time_move-=0.03;
			if(hero->moveComplete())
				moving=false;
		}		
	}
	if(flooring) // 上下楼
	{
		time_floor+=dt;
		if(time_floor>=0.4)
		{
			time_floor=0;
			flooring=false;
		}
	}
	if(opening) // 开门
	{
		time_open+=dt;
		if(time_open>=0.05)
		{
			time_open-=0.05;
			if(map_openingdoor->open())
				opening=false;
		}		
	}

	time_animation+=dt;
	if(time_animation>=0.1) // 四次后又回到自身状态
	{
		time_animation-=0.1;
		currFloor->animation();
	}

}
void constants::printInfo()
{
	s_step->Render(ScreenLeft+map_width*32+16, 32*map_height-76);
	hgef->printf(ScreenLeft+map_width*32+60, 32*map_height-72, HGETEXT_LEFT, "%d", step);
	int ptm=playtime;
	s_time->Render(ScreenLeft+map_width*32+16,32*map_height-120);
	if (ptm>=3600)
		hgef->printf(ScreenLeft+map_width*32+60,32*map_height-116,HGETEXT_LEFT,"%02d : %02d : %02d",ptm/3600, (ptm/60)%60, ptm%60);
	else hgef->printf(ScreenLeft+map_width*32+60,32*map_height-116,HGETEXT_LEFT,"%02d : %02d", ptm/60, ptm%60);
}
