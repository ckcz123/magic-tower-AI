#ifndef AI_UTIL_H
#define AI_UTIL_H

#include "monster.h"
#include "hero.h"
#include <vector>
#include <unordered_set>

bool hasSpecial(int special, int test);

int getDamage(Monster* monster, Hero* hero);

std::string vecInt2String(const std::vector<int>* vec);

std::string vec2String(const std::vector<Monster*>* vec);


#endif //AI_UTIL_H
