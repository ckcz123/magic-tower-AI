#ifndef AI_NODE_H
#define AI_NODE_H

#include "item.h"
#include "hero.h"
#include "monster.h"
#include "util.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Node {
 public:

  Node(int _type, int _f, int _x, int _y):type(_type),f(_f),x(_x),y(_y),
         hero(nullptr),item(nullptr),doors(nullptr),monsters(nullptr) {
    id = hash();
  }

  Node(Node*& another) {
    type = another->type;
    f=another->f;
    x=another->x;
    y=another->y;
    if (another->hero!= nullptr)
      hero = new Hero(another->hero);
    else hero = nullptr;
    if (another->item!= nullptr)
      item = new Item(another->item);
    else item = nullptr;
    if (another->doors!= nullptr) {
      doors = new vector<int>(*another->doors);
    }
    else doors = nullptr;
    if (another->monsters!= nullptr) {
      monsters = new vector<Monster*>(*another->monsters);
    }
    else monsters = nullptr;
    linked = another->linked;
    id = hash();
  }

  Node* setHero(Hero* _hero) {hero=_hero; return this;}

  Node* setMonster(Monster* monster) {
    if (monsters== nullptr) monsters = new vector<Monster*>;
    monsters->push_back(monster); return this;
  }

  Node* setItem(Item* _item) {item=_item; return this;}

  Node* setDoor(int door) {
    if (doors== nullptr) doors = new vector<int>;
    doors->push_back(door); return this;
  }

  Node* addNode(int another) {
    linked.insert(another);
    return this;
  }

  Node* addNode(Node* another) {
    linked.insert(another->id);
    return this;
  }

  void merge(Node*& another);

  Node* merge(Node*& another, bool*& visited);

  int getScore() {
    return hero==nullptr?0:hero->getScore();
  }

  int hash() {
     return 10000*f+100*x+y;
  }

  bool shouldEat(Hero* hero) {
    if (item!= nullptr) return true;
    if (hero == nullptr) return false;

    if (doors!=nullptr) return false;
    if (monsters== nullptr) return false;

    for (const auto& m: *monsters) {
      if (getDamage(m, hero)!=0) return false;
    }

    return true;
  }

  string toString() {
    string str;
    if (hero==nullptr) str.append("Id=").append(std::to_string(id)).append(": ");
    str.append("(").append(std::to_string(f)).append(",")
        .append(std::to_string(y)).append(",").append(std::to_string(x)).append(")");

    if (hero!=nullptr) str.append(" -- Hero: ").append(hero->toString());
    if (item!=nullptr) str.append(" -- Items: ").append(item->toString());
    if (doors!=nullptr) str.append(" -- Doors: ").append(vecInt2String(doors));
    if (monsters!=nullptr) str.append(" -- Monsters: ").append(vec2String(monsters));

    str.append(" -- ").append(std::to_string(linked.size())).append(" Linked\n");

    return str;
  }

  ~Node() {
    delete hero;
    delete item;
    delete doors;
    delete monsters;
  }

  int id, type;
  int f, x, y;
  Hero* hero;
  Item* item;
  vector<int>* doors;
  vector<Monster*>* monsters;
  unordered_set<int> linked;
};

#endif //AI_NODE_H
