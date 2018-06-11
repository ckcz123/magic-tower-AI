#ifndef AI_ITEM_H
#define AI_ITEM_H

#include <string>

class Item {
 public:

  Item() = default;
  explicit Item(Item* another) {
    if (another == nullptr) return;
    hp=another->hp;
    atk=another->atk;
    def=another->def;
    mdef=another->mdef;
    yellow=another->yellow;
    blue=another->blue;
    red=another->red;
    green=another->green;
    special=another->special;
  }

  std::string toString() {
    std::string ans = "";
    if (hp>0) ans.append("hp+").append(std::to_string(hp)).append(";");
    if (atk>0) ans.append("atk+").append(std::to_string(atk)).append(";");
    if (def>0) ans.append("def+").append(std::to_string(def)).append(";");
    if (mdef>0) ans.append("mdef+").append(std::to_string(mdef)).append(";");
    if (yellow>0) ans.append("yellow+").append(std::to_string(yellow)).append(";");
    if (blue>0) ans.append("blue+").append(std::to_string(blue)).append(";");
    if (red>0) ans.append("red+").append(std::to_string(red)).append(";");
    if (green>0) ans.append("green+").append(std::to_string(green)).append(";");
    if (special>0) ans.append("special+").append(std::to_string(special)).append(";");
    return ans;
  }

  void merge(Item* another) {
    if (another == nullptr) return;
    hp+=another->hp; atk+=another->atk; def+=another->def;
    mdef+=another->mdef; yellow+=another->yellow; blue+=another->blue;
    red+=another->red; green+=another->green; special+=another->special;
  }

  Item* setHp(int _hp) {hp=_hp;return this;}

  Item* setAtk(int _atk) {atk=_atk;return this;}

  Item* setDef(int _def) {def=_def;return this;}

  Item* setMDef(int _mdef) {mdef=_mdef;return this;}

  Item* setYellow(int _yellow) {yellow=_yellow;return this;}

  Item* setBlue(int _blue) {blue=_blue;return this;}

  Item* setRed(int _red) {red=_red;return this;}

  Item* setGreen(int _green) {green=_green;return this;}

  Item* setSpecial(int _special) {special=_special;return this;}

  int hp, atk, def, mdef, yellow, blue, red, green, special;
};

#endif //AI_ITEM_H
