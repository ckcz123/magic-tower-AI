#include "util.h"

inline bool hasSpecial(int special, int test) {
  return special!=0 && (special%100==test || hasSpecial(special/100, test));
}

int getDamage(Monster* monster, int hero_hp, int hero_atk, int hero_def, int hero_mdef) {
  int mon_hp = monster->hp, mon_atk = monster->atk, mon_def = monster->def, mon_special = monster->special;

  // 魔攻 & 坚固
  if (hasSpecial(mon_special, 2)) hero_def=0;
  if (hasSpecial(mon_special, 3) && mon_def<hero_atk-1) mon_def=hero_atk-1;

  if (hero_atk<=mon_def) return 999999999;

  int per_damage = mon_atk - hero_def;
  if (per_damage<0) per_damage=0;

  if (hasSpecial(mon_special, 4)) per_damage*=2;
  if (hasSpecial(mon_special, 5)) per_damage*=3;
  if (hasSpecial(mon_special, 6)) per_damage*=4;

  int counterDamage = 0;
  if (hasSpecial(mon_special, 8)) counterDamage += static_cast<int>(0.1 * hero_atk);

  int initDamage = 0;
  if (hasSpecial(mon_special, 1)) initDamage += per_damage;
  if (hasSpecial(mon_special, 7)) initDamage+= static_cast<int>(0.9 * hero_def);
  if (hasSpecial(mon_special, 9)) initDamage+= 2*hero_mdef;

  int turn = (mon_hp-1)/(hero_atk-mon_def)+1;
  int ans = initDamage + (turn-1)*per_damage + turn * counterDamage;
  ans -= hero_mdef;
  if (ans<0) ans=0;
  return ans;
}

int getDamage(Monster* monster, Hero* hero) {
  return getDamage(monster, hero->hp, hero->atk, hero->def, hero->mdef);
}

std::string vecInt2String(const std::vector<int>* vec) {
  std::string str = "[";
  bool start=true;
  for (const auto& v: *vec) {
    if (!start) str.append(",");
    start = false;
    str.append(std::to_string(v));
  }
  str.append("]");
  return str;
}

std::string vec2String(const std::vector<Monster*>* vec) {
  std::string str = "[";
  bool start=true;
  for (const auto& v: *vec) {
    if (!start) str.append(",");
    start = false;
    str.append(v->toString());
  }
  str.append("]");
  return str;
}
