#include "state.h"

State::State(Graph* _graph, Node* node) {
  graph = _graph;
  current = node;
  cnt = 0;
  visited = new bool[graph->nodeToIndex.size()] {false};
  visited[graph->nodeToIndex[current->id]] = true;
  strcpy(route, current->toString().c_str());
  shopTime = -1;
  eatItem();
}

State::State(State* another, Node* mergedNode) {
  graph = another->graph;
  current = mergedNode;
  visited = new bool[graph->nodeToIndex.size()] {false};
  // for (int i=0;i<graph->nodeToIndex.size();i++) visited[i]=another->visited[i];
  copy(another->visited, another->visited+graph->nodeToIndex.size(), visited);
  strcpy(route, another->route);
  shopTime = another->shopTime;
  cnt = another->cnt;
}

Node* State::tryMerge(Node* node) {
  if (visited[graph->nodeToIndex[node->id]] || !current->linked.count(node->id)) {
    return nullptr;
  }
  return current->merge(node, visited);
}

State* State::merge(Node* node) {
  // assume we have tried merge above

  visit(node);
  strcat(route, current->toString().c_str());
  eatItem();
  cnt++;
  return this;

}

void State::eatItem() {
  bool has = true;
  while (has) {
    has = false;
    for (const auto& node: current->linked) {
      if (visited[graph->nodeToIndex[node]]) continue;
      Node* nd = graph->nodes[node];
      if (!nd->shouldEat(graph->_shouldEat?current->hero: nullptr)) continue;
      has = true;
      Node* nxt = current->merge(nd, visited);
      if (nxt == nullptr) continue;
      delete current;
      current = nxt;
      if (nd->item!= nullptr && (nd->item->special&1)!=0 && shopTime<0)
        shopTime = 0;
      visit(nd);
      break;
    }
  }
  while (graph->shop->useShop(current->hero, shopTime)) shopTime++;
}

void State::visit(Node* node) {
  if (!visited[graph->nodeToIndex[node->id]] && current->linked.erase(node->id)>0) {
    if (node->monsters!= nullptr) {
      for (const auto& m: *node->monsters) {
        current->hero->money += m->money;
      }
    }
    visited[graph->nodeToIndex[node->id]] = true;
  }
}

bool State::shouldStop() {
  if (cnt>STOP_COUNT) return true;

  if (graph->bossId>=0 && visited[graph->bossId]) return true;

  return false;
}

int64 State::hash() {
  int64 val = 0;
  int i = 0;
  for (const auto& node: graph->nodes) {
    if (node.second->doors==nullptr && node.second->monsters==nullptr) continue;
    if (visited[graph->nodeToIndex[node.first]]) val |= (1L<<i);
    i++;
  }
  return val;
}

int State::getScore() {
  return current->getScore();
}

State::~State() {
  delete current;
  delete visited;
}
