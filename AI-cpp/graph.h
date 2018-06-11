#ifndef AI_GRAPH_H
#define AI_GRAPH_H

#include <iostream>
#include <ctime>
#include <cstdio>
#include <unordered_map>
#include <queue>
#include "node.h"
#include "state.h"
#include "shop.h"
#include "consts.h"

class State;

class CompareState {
 public:
  bool operator() (State* s1, State* s2);
};

class Graph {
 public:

  Graph(bool _shouldMerge, bool _shouldEat);

  void setPrint(int _print) {print_every = _print;}

  void initGraph();

  void buildMap();

  void mergeNode();

  bool shouldMerge(Node* n1, Node* n2);

  bool isLinked(int f1, int x1, int y1, int f2, int x2, int y2);

  void print();

  void run();

  ~Graph();

  int floor, row, col;
  int print_every;
  int mp[12][21][21];
  int stair[12][4];
  int bossId;
  int p_atk, p_def, p_mdef, p_red, p_blue, p_yellow, p_green, p_sword, p_shield;
  unordered_map<int, Monster*> monsterMap;
  unordered_map<int, Node*> nodes;
  unordered_map<int, int> nodeToIndex;
  Node* startNode;
  Shop* shop;
  bool _shouldMerge, _shouldEat;

};

#endif //AI_GRAPH_H
