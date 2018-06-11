#ifndef AI_STATE_H
#define AI_STATE_H

#include "node.h"
#include "graph.h"
#include <cstring>

class Graph;

#define int64 long long

class State {
 public:

  State(Graph* _graph, Node* node);

  State(State* another, Node* mergedNode);

  Node* tryMerge(Node* node);

  State* merge(Node* node);

  void eatItem();

  void visit(Node* node);

  bool shouldStop();

  int64 hash();

  int getScore();

  ~State();

  Node* current;
  Graph* graph;
  bool* visited;
  // unordered_map<int, bool> visited;
  // vector<string> route;
  char route[10000];
  int shopTime;
  int cnt;
  const int STOP_COUNT = 45;
};

#endif //AI_STATE_H
