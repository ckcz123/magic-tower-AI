#include "graph.h"

extern Graph* graph;

void Node::merge(Node*& another) {
  if (item!= nullptr) item->merge(another->item);
  // addAll
  if (another->doors!= nullptr) {
    if (doors== nullptr) doors = new vector<int>;
    doors->insert(doors->end(), another->doors->begin(), another->doors->end());
  }
  if (another->monsters!= nullptr) {
    if (monsters== nullptr) monsters = new vector<Monster*>;
    monsters->insert(monsters->end(), another->monsters->begin(), another->monsters->end());
  }
  for (const auto& to: another->linked) {
    if (to!=this->id) {
      addNode(to);
      graph->nodes[to]->addNode(this);
    }
    graph->nodes[to]->linked.erase(another->id);
  }
}

Node* Node::merge(Node*& another, bool*& visited) {
  Hero* h = new Hero(hero);

  if (another->item!= nullptr) {
    h->getItem(another->item);
  }

  if (another->doors!= nullptr) {
    for (const int& v: *another->doors) {
      if (v==1) h->yellow--;
      if (v==2) h->blue--;
      if (v==3) h->red--;
      if (v==4) h->green--;
    }
  }

  if (another->monsters!= nullptr) {
    for (const auto& m: *another->monsters) {
      h->hp -= getDamage(m, h);
    }
  }

  if (!h->isValid()) {
    delete h;
    return nullptr;
  }

  Node* nd = (new Node(type, another->f, another->x, another->y))->setHero(h);
  nd->linked = (linked);

  for (const auto& to: another->linked) {
    if (!visited[graph->nodeToIndex[to]])
      nd->addNode(to);
  }

  return nd;
}
