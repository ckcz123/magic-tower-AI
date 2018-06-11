#ifndef AI_SHOP_H
#define AI_SHOP_H

#include "hero.h"

class Shop {
 public:
  Shop(int _start, int _delta, int _hp, int _atk, int _def, int _mdef) {
    start=_start; delta=_delta; hp=_hp; atk=_atk; def=_def; mdef=_mdef;
  }

  int moneyNeeded(int visitTime) {
    return start+delta*visitTime;
  }

  int getHpPoint() {
    return hp;
  }

  int getAtkPoint() {
    return atk;
  }

  int getDefPoint() {
    return def;
  }

  int getMDefPoint() {
    return mdef;
  }

  bool useShop(Hero* hero, int visitTime) {
    if (visitTime<0) return false;
    if (hero->money<moneyNeeded(visitTime)) return false;


    // Add atk

    hero->money -= moneyNeeded(visitTime);
    hero->atk += getAtkPoint();
    return true;
  }


  int start, delta, hp, atk, def, mdef;
};

#endif //AI_SHOP_H
