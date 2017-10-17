#include "stdafx.h"

void c_shop::init(FILE* file) 
{
	// start=_start; delta=_delta; hp=_hp; atk=_atk; def=_def; mdef=_mdef;
	fscanf_s(file, "%d%d%d%d%d%d", &start, &delta, &hp, &atk, &def, &mdef);
	times=0;
}

int c_shop::needMoney()
{
	return start+times*delta;
}

int c_shop::getHpPoint()
{
	return hp;
}

int c_shop::getAtkPoint()
{
	return atk;
}

int c_shop::getDefPoint()
{
	return def;
}

int c_shop::getMDefPoint()
{
	return mdef;
}

void c_shop::output(FILE* file) {
	fprintf_s(file, "%d %d %d %d %d %d\n\n", needMoney(), delta, hp, atk, def, mdef);
}