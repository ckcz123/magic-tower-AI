#include "stdafx.h"

// 地图信息
c_map_floor map_floor[50];

// 勇士信息
c_hero hero;

// 常量信息
constants consts;

// 商店信息
c_shop shop;

void showMessage(const wchar_t *_s) // 显示提示
{

	wchar_t s[500];
	wcscpy_s(s, _s);

	// cal lines
	int l=1;
	for (wchar_t* p=s; *p!=L'\0';p++) {
		if (*p==L'\n') l++;
	}
	if (l<6) l=6;

	int left=16+consts.ScreenLeft, right=consts.map_width*32+consts.ScreenLeft-16,
		top=consts.map_height*32-12-28*l, bottom=consts.map_height*32-8;
	consts.s_bg->RenderStretch(left, top, right, bottom);
	GfxFont *f=new GfxFont(L"楷体", 22);

	// get name
	wchar_t* pos=s;
	while (*pos!=L'\t' && *pos!=L'\0') *pos++;
	if (*pos==L'\t') {
		*pos=L'\0';
		consts.s_bg->RenderStretch(left, top-40, left+25+f->GetTextSize(s).cx, top-2);
		f->Print(left+12, top-33, L"%s", s);
		pos++;
	}
	else pos=s;

	f->Print(left+12, top+12, L"%s", pos);

	delete f;
}
void output()
{
	FILE* file;
	fopen_s(&file, "output.txt", "w");
	fprintf_s(file, "%d\n%d %d\n\n", consts.map_floornum, consts.map_height, consts.map_width);
	for (int i=0;i<consts.map_floornum;i++) {
		map_floor[i].output(file);
	}
	fprintf_s(file, "%d %d %d %d %d %d %d %d %d\n\n", consts.atk_val, consts.def_val, consts.mdef_val,
		consts.redpotion_val, consts.bluepotion_val, consts.yellowpotion_val, consts.greenpotion_val,
		consts.sword_val, consts.shield_val);
	int cnt=0;
	for (int i=0;i<200;i++) {
		if (consts.monster_map[i][0]!=0)
			cnt++;
	}
	fprintf_s(file, "%d\n", cnt);
	for (int i=0;i<200;i++) {
		if (consts.monster_map[i][1]!=0) {
			fprintf_s(file, "%d %d %d %d %d %d\n", i, consts.monster_map[i][0], consts.monster_map[i][1],
				consts.monster_map[i][2], consts.monster_map[i][3], consts.monster_map[i][4]);
		}
	}
	fprintf_s(file, "\n");
	shop.output(file);
	hero.output(file);
	fclose(file);
	consts.setMsg(L"当前状态已输出！");
}
bool frameFunc()
{
	float dt=consts.hge->Timer_GetDelta();

	if (consts.route[consts.pos]!='\0') {
		if (consts.isFree()) {
			char c=consts.route[consts.pos++];
			if (c=='w' && hero.canMove(3)) consts.moving=true;
			if (c=='a' && hero.canMove(1)) consts.moving=true;
			if (c=='s' && hero.canMove(0)) consts.moving=true;
			if (c=='d' && hero.canMove(2)) consts.moving=true;
		}
	}
	else {
		if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_DOWN) && hero.canMove(0))
			consts.moving=true;
		if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_LEFT) && hero.canMove(1))
			consts.moving=true;
		if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_RIGHT) && hero.canMove(2))
			consts.moving=true;
		if(consts.isFree() && consts.hge->Input_GetKeyState(HGEK_UP) && hero.canMove(3))
			consts.moving=true;
	}

	if (consts.isFree() && consts.hge->Input_GetKeyState(HGEK_O))
		output();

	if (consts.isFree() && consts.hge->Input_GetKeyState(HGEK_U))
		consts.msg=consts.MESSAGE_SHOP;

	if (consts.msg==consts.MESSAGE_SHOP) {
		if (consts.hge->Input_GetKeyState(HGEK_1) && clock()-consts.lasttime>200) {
			hero.useShop(1);
			consts.lasttime=clock();
		}
		else if (consts.hge->Input_GetKeyState(HGEK_2) && clock()-consts.lasttime>200) {
			hero.useShop(2);
			consts.lasttime=clock();
		}
		else if (consts.hge->Input_GetKeyState(HGEK_3) && clock()-consts.lasttime>200) {
			hero.useShop(3);
			consts.lasttime=clock();
		}
		else if (consts.hge->Input_GetKeyState(HGEK_4) && clock()-consts.lasttime>200) {
			hero.useShop(4);
			consts.lasttime=clock();
		}
		else if (consts.hge->Input_GetKeyState(HGEK_ENTER)) {
			consts.msg=consts.MESSAGE_NONE;
		}
	}


	// 提示消息
	if (consts.msg==consts.MESSAGE_HINT)
	{
		if(consts.hge->Input_GetKeyState(HGEK_ENTER) && clock()-consts.lasttime>200) {
			consts.nowcnt++;
			consts.lasttime=clock();
		}
		if (consts.nowcnt>=consts.hint.size())
			consts.msg=consts.MESSAGE_NONE;
	}

	// 胜利or失败
	// if (consts.msg==consts.MESSAGE_WIN && consts.hge->Input_GetKeyState(HGEK_ENTER)) return true;

	consts.goOn(&hero, &map_floor[hero.getNowFloor()], dt);
	return false;

}

bool renderFunc()
{
	consts.hge->Gfx_BeginScene();
	consts.hge->Gfx_Clear(0);
	map_floor[hero.getNowFloor()].show();
	hero.show();
	hero.printInfo();
	consts.printInfo();

	float mx,my; // 鼠标位置
	consts.hge->Input_GetMousePos(&mx,&my);
	int x=(mx-consts.ScreenLeft)/32,y=my/32;
	if (x>=0 && x<consts.map_width)map_floor[hero.getNowFloor()].printMonsterInfo(x,y);


	switch (consts.msg)
	{
	case consts.MESSAGE_HINT:
		showMessage(consts.hint.at(consts.nowcnt).c_str());
		break;
	case consts.MESSAGE_SHOP:
	{
		wchar_t ss[1000];
		wsprintf(ss, L"贪婪之神\t你可以使用%d金币来提升能力：\n[1] 生命+%d\n[2] 攻击+%d\n[3] 防御+%d\n[4] 魔防+%d\n[ENTER] 退出商店",
			shop.needMoney(), shop.getHpPoint(), shop.getAtkPoint(), shop.getDefPoint(), shop.getMDefPoint());
		showMessage(ss);
	}
	default:
		break;
	}
	consts.hge->Gfx_EndScene();
	return false;
}
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	FILE *map_infile;
	fopen_s(&map_infile,"input.txt","r");
	fscanf_s(map_infile,"%d",&consts.map_floornum);
	fscanf_s(map_infile,"%d%d",&consts.map_height, &consts.map_width);

	//hge初始化
	consts.hge=hgeCreate(HGE_VERSION);
	consts.hge->System_SetState(HGE_FRAMEFUNC,frameFunc);
	consts.hge->System_SetState(HGE_RENDERFUNC,renderFunc);
	consts.hge->System_SetState(HGE_USESOUND,true);
	consts.hge->System_SetState(HGE_TITLE,"魔塔");
	consts.hge->System_SetState(HGE_WINDOWED,true);
	consts.hge->System_SetState(HGE_HIDEMOUSE,false);
	consts.hge->System_SetState(HGE_SCREENHEIGHT,32*consts.map_height);
	consts.hge->System_SetState(HGE_SCREENWIDTH,32*consts.map_width+2*consts.ScreenLeft);
	if(consts.hge->System_Initiate())
	{
		consts.loadResources();

		int ch[50][30][30];
		
		for (int i=0;i<consts.map_floornum;i++) {
			// int ch[30][30];
			for(int j=0;j<consts.map_height;j++)for(int k=0;k<consts.map_width;k++)fscanf_s(map_infile,"%d",&ch[i][j][k]);
			// map_floor[i].init(i, ch);
		}
		consts.init(map_infile);
		shop.init(map_infile);
		hero.init(map_infile);
		for (int i=0;i<consts.map_floornum;i++) {
			map_floor[i].init(i, ch[i]);
		}
		consts.hge->System_Start();
		consts.destroy();
	}
	consts.hge->System_Shutdown();
	consts.hge->Release();
	fclose(map_infile);
	return 0;
}


