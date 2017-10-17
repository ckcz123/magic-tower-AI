#pragma once

class c_shop {
public:
	void init(FILE*);
	int needMoney();
	int getHpPoint();
	int getAtkPoint();
	int getDefPoint();
	int getMDefPoint();
	void useShop() {times++;}
	void output(FILE*);
private:
	int start, delta, hp, atk, def, mdef, times;
};