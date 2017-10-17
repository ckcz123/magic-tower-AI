#include "stdafx.h"

extern c_map_floor map_floor[50];
extern constants consts;
extern c_shop shop;

c_hero::c_hero()
{
	dir[0][0]=dir[0][3]=dir[1][1]=dir[1][2]=0;
	dir[0][1]=dir[1][3]=-1;
	dir[0][2]=dir[1][0]=1;
};
void c_hero::init(FILE* file)
{
	face=3;
	move=0;
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			sprites[i][j]=new hgeSprite(consts.ht_hero, 32*i, 33*j, 32, 33);
	
	fscanf_s(file, "%d%d%d%d%d%d%d%d%d%d%d", &hp, &atk, &def, &mdef, &money, &yellowkey, &bluekey, &redkey, &now_floor, &y, &x);
}
int c_hero::nextX()
{
	return x+dir[0][face];
}
int c_hero::nextY()
{
	return y+dir[1][face];
}
bool c_hero::canMove(int f)
{
	face=f;
	return map_floor[now_floor].canMove(nextX(),nextY(),f);
}
void c_hero::show()
{
	sprites[move][face]->Render(x*32+8*move*dir[0][face]+consts.ScreenLeft, y*32+8*move*dir[1][face]);
}
bool c_hero::moveComplete()
{
	bool c=false;
	if(move==3)
	{
		c=true;
		x+=dir[0][face];
		y+=dir[1][face];
	}
	if (c)
	{
		char item=map_floor[now_floor].getItem(x,y);
		switch(item)
		{
		case 11: atk+=consts.atk_val;break;
		case 12: def+=consts.def_val;break;
		case 13: mdef+=consts.mdef_val;break;
		case 15: yellowkey++;break;
		case 16: bluekey++;break;
		case 17: redkey++;break;
		case 18: hp+=consts.redpotion_val;break;
		case 19: hp+=consts.bluepotion_val;break;
		case 20: hp+=consts.yellowpotion_val; break;
		case 21: hp+=consts.greenpotion_val; break;
		case 22: atk+=consts.sword_val;break;
		case 23: def+=consts.shield_val;break;
		}
		consts.step++;
	}
	move++;
	move%=4;
	return c;
}
bool c_hero::openRedDoor()
{
	if(redkey>0)
	{
		redkey--;
		return true;
	}
	return false;
}
bool c_hero::openBlueDoor()
{
	if(bluekey>0)
	{
		bluekey--;
		return true;
	}
	return false;
}
bool c_hero::openYellowDoor()
{
	if(yellowkey>0)
	{
		yellowkey--;
		return true;
	}
	return false;
}
void c_hero::upstair()
{
	now_floor++;
	map_floor[now_floor].getDownPosition(x,y);
	consts.flooring=true;
	consts.step++;
	if (now_floor>=consts.map_floornum)
		consts.map_floornum=now_floor+1;
}
void c_hero::downstair()
{
	now_floor--;
	map_floor[now_floor].getUpPosition(x,y);
	consts.flooring=true;
	consts.step++;
}
void c_hero::turn()
{
	if (face==0) face=2;
	else if (face==2) face=3;
	else if (face==3) face=1;
	else face=0;
}
void c_hero::printInfo()
{
	int py=16;
	consts.s_storey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"(%d, %d, %d)",now_floor, y, x);
	py+=32;
	consts.s_heart->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",hp);
	py+=32;
	consts.s_atk->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",atk);
	py+=32;
	consts.s_def->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",def);
	py+=32;
	consts.s_mdef->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",mdef);
	py+=32;
	consts.s_money->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",money);
	py+=32;
	consts.s_yellowkey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",yellowkey);
	py+=32;
	consts.s_bluekey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",bluekey);
	py+=32;
	consts.s_redkey->Render(16,py);
	consts.hgef->printf(60,py,HGETEXT_LEFT,"%d",redkey);
}
bool c_hero::canBeat(c_monster* monster) // ÅÐ¶ÏÄÜ·ñ´ò°Ü
{
	return hp>getDamage(monster);
}
int c_hero::getDamage(c_monster* monster) // ´ò°Ü¹ÖÎï£¬·µ»Øhp
{
	return getDamage(atk,def,mdef,monster->getHp(),monster->getAtk(),monster->getDef(),monster->getSpecial());
}
int c_hero::getDamage(int hero_atk, int hero_def, int hero_mdef,
					  int mon_hp, int mon_atk, int mon_def, int mon_special)
{
	// Ä§¹¥
	if (mon_special==2) hero_def=0;
	// ¼á¹Ì
	if (mon_special==3 && mon_def<hero_atk-1) mon_def=hero_atk-1;
	// Ä£·Â
	if (mon_special==10)
	{
		mon_atk=hero_atk; mon_def=hero_def;
	}
	if (hero_atk<=mon_def) return 999999999;

	int per_damage=mon_atk-hero_def;
	if (per_damage<0) per_damage=0;
	// 2Á¬»÷ & 3Á¬»÷

	if (mon_special==4) per_damage*=2;
	if (mon_special==5) per_damage*=3;
	if (mon_special==6) per_damage*=4;
	// ·´»÷
	if (mon_special==8) per_damage+=(int)(0.1*hero_atk);

	// ÏÈ¹¥
	int damage=mon_special==1?per_damage:0;
	// ÆÆ¼×
	if (mon_special==7) damage=(int)(0.9*hero_def);
	// ¾»»¯
	if (mon_special==9) damage=3*hero_mdef;

	int ans=damage+(mon_hp-1)/(hero_atk-mon_def)*per_damage;
	ans -= hero_mdef;

	// Ä§·À»ØÑª
	// return ans;

	// Ä§·À²»»ØÑª
	return ans<=0?0:ans;
}
void c_hero::beat(c_monster* monster)
{
	int damage=getDamage(monster);
	if (hp>damage)
	{
		hp-=damage;
		money+=monster->getMoney();
		monster->init(0);
	}
	consts.lasttime=clock();
}
void c_hero::useShop(int choose)
{
	if (money<shop.needMoney()) return;
	money-=shop.needMoney();
	if (choose==1) hp+=shop.getHpPoint();
	if (choose==2) atk+=shop.getAtkPoint();
	if (choose==3) def+=shop.getDefPoint();
	if (choose==4) mdef+=shop.getMDefPoint();
	shop.useShop();
}
void c_hero::output(FILE* file)
{
	fprintf_s(file, "%d %d %d %d %d %d %d %d %d %d %d\n", hp, atk, def, mdef, money,
		yellowkey, bluekey, redkey, now_floor, x, y);
}