#ifndef AI_HERO_H
#define AI_HERO_H

#include "item.h"
#include <string>

class Hero {
 public:

  Hero(int _hp, int _atk, int _def, int _mdef, int _money, int _yellow, int _blue, int _red, int _green) {
    hp=_hp; atk=_atk; def=_def; mdef=_mdef; money=_money; yellow=_yellow; blue=_blue; red=_red; green=_green;
  }

  explicit Hero(Hero* another) {
    hp=another->hp; atk=another->atk; def=another->def; mdef=another->mdef; money=another->money;
    yellow=another->yellow; blue=another->blue; red=another->red; green=another->green;
  }

  void getItem(Item* item) {
    if (item == nullptr) return;
    hp+=item->hp; atk+=item->atk; def+=item->def; mdef+=item->mdef;
    yellow+=item->yellow; blue+=item->blue; red+=item->red; green+=item->green;
  }

  bool isValid() {
    return hp>0 && yellow>=0 && blue>=0 && red>=0 && green>=0;
  }

  int getScore() {
    // return hp+1000*(atk+def)+60*mdef+200*yellow+450*blue;
    return hp;
  }

  std::string toString() {
    char s[100];
    sprintf(s, "(%d,%d,%d,%d,%d,%d,%d,%d,%d)",
            hp, atk, def, mdef, money, yellow, blue, red, green);
    return std::string(s);
  }

  int hp, atk, def, mdef, money, yellow, blue, red, green;
};

#endif //AI_HERO_H
