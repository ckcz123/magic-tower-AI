#ifndef AI_MONSTER_H
#define AI_MONSTER_H

#include <string>
#include <cstdio>

class Monster {
public:

  Monster(int _id, int _hp, int _atk, int _def, int _money, int _special):
      id(_id), hp(_hp), atk(_atk), def(_def), money(_money), special(_special) {}

  std::string toString() {
    char s[100];
    sprintf(s, "(%d,%d,%d,%d,%d,%d)", id, hp, atk, def, money, special);
    return std::string(s);
  }

  int id, hp, atk, def, money, special;
};

#endif //AI_MONSTER_H
