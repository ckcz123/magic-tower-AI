#include "graph.h"

bool CompareState::operator() (State* s1, State* s2) {
  if (s1->cnt == s2->cnt) return s1->getScore() < s2->getScore();
  return s1->cnt > s2->cnt;
}

Graph::Graph(bool _shouldMerge, bool _shouldEat) {
  scanf("%d%d%d", &floor, &row, &col);
  for (int i=0;i<floor;i++) for (int j=0;j<4;j++) stair[i][j] = -1;
  for (int i=0;i<floor;i++) {
    for (int j=0;j<row;j++) {
      for (int k=0;k<col;k++) {
        scanf("%d", &mp[i][j][k]);
      }
    }
  }
  scanf("%d%d%d%d%d%d%d%d%d", &p_atk, &p_def, &p_mdef,
         &p_red, &p_blue, &p_yellow, &p_green, &p_sword, &p_shield);

  int num;
  scanf("%d", &num);
  for (int i=0;i<num;i++) {
    int id, hp, atk, def, money, special;
    scanf("%d%d%d%d%d%d", &id, &hp, &atk, &def, &money, &special);
    // monsterMap.insert(make_pair(id, new Monster(id, hp, atk, def, money, special)));
    monsterMap[id] = new Monster(id, hp, atk, def, money, special);
  }

  printf("201: %s", monsterMap[201]->toString().c_str());

  int s_start, s_delta, s_hp, s_atk, s_def, s_mdef;
  scanf("%d%d%d%d%d%d", &s_start, &s_delta, &s_hp, &s_atk, &s_def, &s_mdef);
  shop = new Shop(s_start, s_delta, s_hp, s_atk, s_def, s_mdef);

  int hp, atk, def, mdef, money, yellow, blue, red, floor, x, y;
  scanf("%d%d%d%d%d%d%d%d%d%d%d", &hp, &atk, &def, &mdef, &money,
         &yellow, &blue, &red, &floor, &x, &y);

  startNode = (new Node(0, floor, x, y))->setHero(
      new Hero(hp, atk, def, mdef, money, yellow, blue, red, 0));

  nodes.insert(make_pair(startNode->id, startNode));

  this->_shouldMerge = _shouldMerge;
  this->_shouldEat = _shouldEat;

  print_every = 1000;
}

void Graph::initGraph() {
  buildMap();

  if (_shouldMerge)
    mergeNode();

  int index=0;
  for (const auto& nd: nodes) {
    if (nd.second->type == MONSTER_BOSS) bossId = index;
    nodeToIndex.insert(make_pair(nd.first, index++));
  }

}

void Graph::buildMap() {
  for (int i=0;i<floor;i++) {
    for (int j=0;j<row;j++) {
      for (int k=0;k<col;k++) {
        Node* node= nullptr;
        if (mp[i][j][k]==UP_STAIR) {
          stair[i][0]=j; stair[i][1]=k;
        }
        if (mp[i][j][k]==DOWN_STAIR) {
          stair[i][2]=j; stair[i][3]=k;
        }
        if (mp[i][j][k]==YELLOW_KEY)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setYellow(1));
        if (mp[i][j][k]==BLUE_KEY)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setBlue(1));
        if (mp[i][j][k]==RED_KEY)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setRed(1));
        if (mp[i][j][k]==GREEN_KEY)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setGreen(1));
        if (mp[i][j][k]==RED_JEWEL)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setAtk(p_atk));
        if (mp[i][j][k]==BLUE_JEWEL)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setDef(p_def));
        if (mp[i][j][k]==GREEN_JEWEL)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setMDef(p_mdef));
        if (mp[i][j][k]==RED_POTION)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setHp(p_red));
        if (mp[i][j][k]==BLUE_POTION)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setHp(p_blue));
        if (mp[i][j][k]==YELLOW_POTION)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setHp(p_yellow));
        if (mp[i][j][k]==GREEN_POTION)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setHp(p_green));
        if (mp[i][j][k]==SWORD)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setAtk(p_sword));
        if (mp[i][j][k]==SHIELD)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setDef(p_shield));
        if (mp[i][j][k]==SHOP)
          node=(new Node(mp[i][j][k],i,j,k))->setItem((new Item())->setSpecial(1));

        if (mp[i][j][k]==YELLOW_DOOR)
          node=(new Node(mp[i][j][k],i,j,k))->setDoor(1);
        if (mp[i][j][k]==BLUE_DOOR)
          node=(new Node(mp[i][j][k],i,j,k))->setDoor(2);
        if (mp[i][j][k]==RED_DOOR)
          node=(new Node(mp[i][j][k],i,j,k))->setDoor(3);
        if (mp[i][j][k]==GREEN_DOOR)
          node=(new Node(mp[i][j][k],i,j,k))->setDoor(4);
        if (mp[i][j][k]>=MONSTER_BOUND) {
          // Monster* monster=monsterMap.get(map[i][j][k]);
          auto monster = monsterMap[mp[i][j][k]];
          if (monster==nullptr) continue;
          node=(new Node(mp[i][j][k],i,j,k))->setMonster(monster);
        }

        if (node!= nullptr)
          nodes.insert(make_pair(node->id, node));
      }
    }
  }

  // build graph
  for (const auto& i: nodes) {
    for (const auto& j: nodes) {
      if (i.first == j.first) continue;
      Node* n1=i.second, *n2=j.second;
      if (isLinked(n1->f, n1->x, n1->y, n2->f, n2->x, n2->y)) {
        n1->addNode(n2);
        n2->addNode(n1);
      }
    }
  }
}

void Graph::mergeNode() {
  for (const auto& i1: nodes) {
    for (const auto& i2: nodes) {
      if (i1.first==i2.first) continue;
      Node *n1=i1.second, *n2=i2.second;
      if (shouldMerge(n1, n2)) {
        n1->merge(n2);
        delete n2;
        nodes.erase(i2.first);
        mergeNode();
        return;
      }
    }
  }
}

bool Graph::shouldMerge(Node* n1, Node* n2) {
  if (n1->linked.count(n2->id)==0 || n2->linked.count(n1->id)==0) return false;
  if (n1->item!=nullptr && n2->item!=nullptr) return true;
  if (n1->item!=nullptr || n2->item!=nullptr) return false;

  if (n1->type==MONSTER_BOSS || n2->type==MONSTER_BOSS) return false;
  for (const auto& nd: n2->linked) {
    if (n1->linked.count(nd)>0)
      return false;
  }
  for (const auto& x: n1->linked) {
    for (const auto& y: n1->linked) {
      if (x==y || x==n2->id || y==n2->id) continue;
      if (nodes[x]->linked.count(y)==0 || nodes[y]->linked.count(x)==0) return false;
    }
  }
  for (const auto& x: n2->linked) {
    for (const auto& y: n2->linked) {
      if (x==y || x==n1->id || y==n1->id) continue;
      if (nodes[x]->linked.count(y)==0 || nodes[y]->linked.count(x)==0) return false;
    }
  }
  return true;
}

bool Graph::isLinked(int f1, int x1, int y1, int f2, int x2, int y2) {
  if (f1<f2) return isLinked(f2, x2, y2, f1, x1, y1);
  if (f1!=f2)
    return isLinked(f1,x1,y1,f1,stair[f1][2],stair[f1][3])
        && isLinked(f1-1,stair[f1-1][0],stair[f1-1][1],f2,x2,y2);

  if (x1==x2 && y1==y2) return true;

  bool visited[21][21] = {false};
  visited[x1][y1]=true;
  queue<int> q;
  q.push(x1);
  q.push(y1);
  int dir[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
  while (!q.empty()) {
    int x=q.front(); q.pop();
    int y=q.front(); q.pop();
    for (auto& i : dir) {
      int nx=x+i[0], ny=y+i[1];
      if (nx<0 || nx>=row || ny<0 || ny>=col) continue;
      if (nx==x2 && ny==y2) return true;
      if (visited[nx][ny] || (mp[f1][nx][ny]!=ROAD
          && mp[f1][nx][ny]!=UP_STAIR && mp[f1][nx][ny]!=DOWN_STAIR))
        continue;

      visited[nx][ny]=true;
      q.push(nx); q.push(ny);
    }
  }
  return false;
}

void Graph::print() {
  printf("\n------ Printing Nodes ------\n");
  int valid = 0;
  for (const auto& node: nodes) {
    printf("%s", node.second->toString().c_str());
    if (node.second->doors!=nullptr || node.second->monsters!= nullptr) {
      valid++;
    }
  }
  printf("%d/%lu nodes in total.\n", valid, nodes.size());

}

void Graph::run() {
  printf("\n------ Starting BFS ------\n");

  clock_t begin = clock();

  auto state = new State(this, new Node(startNode));

  int score = 0;
  char answer[10000];

  int index = 0, solutions = 0;

  unordered_set<int64> set;
  unordered_map<int64, int> map;

  priority_queue<State*, vector<State*>, CompareState> pq;

  pq.push(state);

  while (!pq.empty()) {
    state = pq.top();
    pq.pop();

    int64 hash = state->hash();
    if (set.count(hash)>0) {
      delete state;
      continue;
    }

    set.insert(hash);

    if (state->shouldStop()) {
      if (score < state->getScore()) {
        score = state->getScore();
        strcpy(answer, state->route);
      }
      solutions++;
      delete state;
      continue;
    }

    // extend
    for (const auto& node: state->current->linked) {
      if (state->visited[nodeToIndex[node]]) continue;

      /*
      // should extend?
      bool shouldExtend = false;
      for (const auto& x: node->linked) {
        if (x!=state->current && state->current->linked.count(x)==0) {
          shouldExtend = true;
          break;
        }
      }
      if (!shouldExtend)
        continue;
      */

      Node* tmp = state->tryMerge(nodes[node]);
      if (tmp == nullptr) continue;
      State* another = (new State(state, tmp))->merge(nodes[node]);
      // check map
      int& v = map[another->hash()];
      if (v>another->getScore()) {
        delete another;
        continue;
      }
      v = another->getScore();

      pq.push(another);
    }

    delete state;

    index++;
    if (index % print_every == 0) {
      printf("Calculating... %d calculated, %lu still in queue.\n", index, pq.size());
    }
  }
  printf("cnt: %d; solutions: %d\n", index, solutions);
  printf("------ BFS FINISHED ------\n");

  if (strlen(answer)==0) {
    printf("No solution!\n");
  }
  else {
    printf("%s\n", answer);
  }

  clock_t end = clock();
  double seconds = double(end-begin) / CLOCKS_PER_SEC;
  printf("Time used: %.3fs\n", seconds);

}

Graph::~Graph() {
  delete shop;
  for (const auto& nd: nodes) {
    delete nd.second;
  }
  for (const auto& mon: monsterMap) {
    delete mon.second;
  }
}
