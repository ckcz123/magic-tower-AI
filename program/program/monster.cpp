#include "stdafx.h"

extern c_hero hero;
extern constants consts;
void c_monster::init(int t)
{
	id=t;
	state=0; special=0;
	switch(id)
	{
	case 1:wcscpy_s(name,L"绿色史莱姆");position=0;break;
	case 2:wcscpy_s(name,L"红色史莱姆");position=1;break;
	case 3:wcscpy_s(name,L"青头怪");position=2;break;
	case 4:wcscpy_s(name,L"史莱姆王");position=3;break;
	case 5:wcscpy_s(name,L"小蝙蝠");position=4;break;
	case 6:wcscpy_s(name,L"大蝙蝠");position=5;break;
	case 7:wcscpy_s(name,L"红蝙蝠");position=6;break;
	case 8:wcscpy_s(name,L"冥灵魔王");position=7;break;
	case 9:wcscpy_s(name,L"初级法师");position=8;break;
	case 10:wcscpy_s(name,L"高级法师");position=9;break;
	case 11:wcscpy_s(name,L"初级巫师");position=10;break;
	case 12:wcscpy_s(name,L"高级巫师");position=11;break;
	case 13:wcscpy_s(name,L"骷髅人");position=12;break;
	case 14:wcscpy_s(name,L"骷髅士兵");position=13;break;
	case 15:wcscpy_s(name,L"骷髅队长");position=14;break;
	case 16:wcscpy_s(name,L"冥队长");position=15;break;
	case 17:wcscpy_s(name,L"兽面人");position=16;break;
	case 18:wcscpy_s(name,L"兽面武士");position=17;break;
	case 19:wcscpy_s(name,L"石头人");position=18;break;
	case 20:wcscpy_s(name,L"影子战士");position=19;break;
	case 21:wcscpy_s(name,L"初级卫兵");position=20;break;
	case 22:wcscpy_s(name,L"中级卫兵");position=21;break;
	case 23:wcscpy_s(name,L"高级卫兵");position=22;break;
	case 24:wcscpy_s(name,L"双手剑士");position=23;break;
	case 25:wcscpy_s(name,L"冥战士");position=24;break;
	case 26:wcscpy_s(name,L"骑士队长");position=25;break;
	case 27:wcscpy_s(name,L"金骑士");position=26;break;
	case 28:wcscpy_s(name,L"灵武士");position=27;break;
	case 29:wcscpy_s(name,L"红衣魔王");position=28;break;
	case 30:wcscpy_s(name,L"白衣武士");position=29;break;
	case 31:wcscpy_s(name,L"灵法师");position=30;break;
	case 32:wcscpy_s(name,L"黄头怪");position=31;break;
	case 33:wcscpy_s(name,L"紫骷髅");position=32;break;
	case 34:wcscpy_s(name,L"紫蝙蝠");position=33;break;
	case 35:wcscpy_s(name,L"铁面人");position=34;break;
	case 36:wcscpy_s(name,L"绿兽人");position=35;break;
	case 37:wcscpy_s(name,L"黑衣魔王");position=36;break;
	case 38:wcscpy_s(name,L"黄衣魔王");position=37;break;
	case 39:wcscpy_s(name,L"青衣武士");position=38;break;
	case 40:wcscpy_s(name,L"近卫骑士");position=39;break;
	case 99:wcscpy_s(name,L"黑衣魔王");position=36;break;
	default:position=-1;
	}
	if (position!=-1) {
		hp=consts.monster_map[100+id][0];
		atk=consts.monster_map[100+id][1];
		def=consts.monster_map[100+id][2];
		money=consts.monster_map[100+id][3];
		special=consts.monster_map[100+id][4];
		for(int i=0;i<4;i++)
			monster[i]=new hgeSprite(consts.ht_monster,32*i,32*position,32,32);
	}

}
int c_monster::getAtk() {
	// 模仿
	if (special==10) return hero.getAtk();
	return atk;
}
int c_monster::getDef() {
	// 坚固
	if (special==3) return hero.getAtk()-1;
	// 模仿
	if (special==10) return hero.getDef();
	return def;
}
void c_monster::show(GfxFont* f,int i,int j)
{
	if(id!=0)
	{
		monster[state]->Render(j*32+consts.ScreenLeft,i*32);
		if (f!=NULL) {
			int damage=hero.getDamage(this);
			int hp=hero.getHP();
			if (damage>=hp) {
				f->SetColor(0xFFFF0000);
			}
			else if (damage<=hp/10) {
				f->SetColor(0xFF00FF00);
			}
			else if (damage<=hp/5) {
				f->SetColor(0xFF66CD00);
			}
			else if (damage<=hp/2) {
				f->SetColor(0xFF96CDCD);
			}
			else if (damage<=hp*3/4) {
				f->SetColor(0xFFEE7942);
			}
			else {
				f->SetColor(0xFFEE3B3B);
			}
			wchar_t ss[100];
			if (damage>=c_hero::MAX_DAMAGE)
				wcscpy_s(ss, L"???");
			else if (damage<100000)
				wsprintf(ss, L"%d", damage);
			else
				wsprintf(ss, L"%.1fW", damage/10000.0);
			SIZE size=f->GetTextSize(ss);
			int left=j*32+consts.ScreenLeft, top=i*32+20;
			consts.s_bg_font->RenderStretch(left-1, top-1, left+size.cx+1, top+size.cy+1);
			f->Render(left, top, ss);
		}
	}
}

void c_monster::printInfo()
{
	if(id!=0)
	{
		int py=16;
		GfxFont *f=new GfxFont(L"楷体",24);
		wchar_t s0[100]=L"";
		if (special==1) wcscpy_s(s0, L"（先攻）");
		else if (special==2) wcscpy_s(s0, L"（魔攻）");
		else if (special==3) wcscpy_s(s0, L"（坚固）");
		else if (special==4) wcscpy_s(s0, L"（2连击）");
		else if (special==5) wcscpy_s(s0, L"（3连击）");
		else if (special==6) wcscpy_s(s0, L"（4连击）");
		else if (special==7) wcscpy_s(s0, L"（破甲）");
		else if (special==8) wcscpy_s(s0, L"（反击）");
		else if (special==9) wcscpy_s(s0, L"（净化）");
		else if (special==10) wcscpy_s(s0, L"（模仿）");
		f->Print(consts.ScreenLeft+consts.map_width*32+16,py,L"%s%s",name,s0);
		delete f;
		py+=32;
		consts.s_heart->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60, py, HGETEXT_LEFT, "%d", getHp());
		py+=32;
		consts.s_atk->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getAtk());
		py+=32;
		consts.s_def->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",getDef());
		py+=32;
		consts.s_money->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",money);
		py+=32;
		consts.s_damage->Render(consts.ScreenLeft+consts.map_width*32+16,py);
		int damage=hero.getDamage(this), defDamage=hero.getDefDamage(getHp(), getAtk(), getDef(), getSpecial());
		int critical=hero.getCritical(getHp(), getAtk(), getDef(), getSpecial());
		int criticalDamage=hero.getCriticalDamage(getHp(), getAtk(), getDef(), getSpecial());
		if (damage==c_hero::MAX_DAMAGE)
			consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"???  (%d)",defDamage);
		else
			consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d  (%d)",damage, defDamage);
		py+=32;
		if (critical>0)
		{
			consts.s_critical->Render(consts.ScreenLeft+consts.map_width*32+16,py);
			if (criticalDamage>0)
				consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d  (%d)",critical,criticalDamage);
			else
				consts.hgef->printf(consts.ScreenLeft+consts.map_width*32+60,py,HGETEXT_LEFT,"%d",critical);
		}
	}
}
void c_monster::changeState()
{
	state++;
	state%=4;
}