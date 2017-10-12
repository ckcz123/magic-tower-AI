/*	
 *	作品：魔塔AI
 *	队伍：厉害了word队
 *	队员：顾友鹏、金朝、黄瀚贤、宋博宁
 *	
 */
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<algorithm>
#include<queue>
#include<stack>
#include<ctime>
using namespace std;

string rev(string s, int sx, int sy, int ex, int ey, int type = 0);
int getDamage(int h_atk, int h_def, int h_mdef, int m_hp, int m_atk, int m_def, int m_spe);
bool is_connected(int sx, int sy, int ex, int ey);
string cal_trace(int sx, int sy, int ex, int ey);

class Node;
fstream fin, fout;



/*构图*/
int vis[15][15];
int Mov[4][2] = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 } };
char str[5] = "wsad";
struct Path {	//存路径结构
	int x, y;
	char sp;
}path[15][15];
int i = 1;
Node *graph[200];	//存图结点
/*属性*/
int layer;
int row, col;
int map[15][15];
string ans;  //output string
string bestway;  //best output
int monster[100][6] = { 0 };  //exist, hp, attack, defence, special skill
int hero_hp, hero_attack, hero_defence, hero_mdefence, sx, sy;
int node_num = 0;	//结点数量
int power_of_Redgem, power_of_Bluegem, power_of_Greengem, capacity_of_Smallhpbottle, capacity_of_Largehpbottle;
int hero_yellow_key = 0, hero_red_key = 0, hero_blue_key = 0;
int anshp = 0, monster_defeat = 0;

/*边结构*/
class Edge {
public:
	Node* to;	//目的地结点
	string move;	//去往目的地的路径字符串
	Edge* nextedge;	//起点相同的下一条边

	Edge() {
		to = NULL;
		move = "";
		nextedge = NULL;
	}
	Edge(Node* g, string s) {
		to = g;
		move = s;
		nextedge = NULL;
	}

	Edge(Edge *s, Edge *e) {
		to = e->to;
		move = s->move + e->move;
		nextedge = NULL;
	}

	Edge(Edge *e) {
		to = e->to;
		move = e->move;
		nextedge = NULL;
	}
};

/*	图结点
 *
 *	用于存储地图上每个非墙非道路结点，存于graph指针数组
 *	graph[0]代表开始的结点，并且记录英雄的信息
 *	每次行动时，合并结点，将当前结点更新，更换关键结点。
 */
class Node {
public:
	int hp;	//记录人物的属性/怪物的属性/宝石加成
	int attack_increase;
	int defence_increase;
	int magicdefence_increase;
	int yellow_key, blue_key, red_key;	//三种钥匙的数量，门为-1
	int type_num;	//1代表宝物，>50代表怪，每次行动继承目的地结点是type_num
	int visited;	//标记是否已经走过
	int x, y;	//当前结点在地图上的坐标
	int id;	//记录当前位置的结点在graph数组存储的位置
	Edge* firstedge;	//邻接表指针
	Node() {
		hp = attack_increase = defence_increase = magicdefence_increase = blue_key = yellow_key = red_key = 0;
		type_num = visited = x = y = 0;
		firstedge = NULL;
	}
	Node(int h, int a, int d, int m, int type, int xx = 0, int yy = 0, int i = -1) {
		hp = h;
		attack_increase = a;
		defence_increase = d;
		magicdefence_increase = m;
		firstedge = NULL;
		yellow_key = blue_key = red_key = 0;
		type_num = type;
		visited = 0;
		x = xx;
		y = yy;
		id = i;
	}

	/*向邻接表中加入一条边*/
	Edge* SetEdge(Node* g, string s) {
		Edge* e = new Edge(g, s);
		if (this->firstedge == NULL) {			
			this->firstedge = e;			
		}
		else {
			Edge* temp = this->firstedge;
			while (temp->nextedge != NULL) {
				temp = temp->nextedge;
			}
			temp->nextedge = e;
		}
		return this->firstedge;
	}

	/*寻找到达g结点的边*/
	Edge* ToNode(Node* g) {
		Edge* tmp;
		for (tmp = this->firstedge; tmp != NULL; tmp = tmp->nextedge) {
			if (tmp->to == g)
				break;
		}
		return tmp;
	}

	/*删除到达g结点的边（本程序没有用到）*/
	bool DelEdge(Node *g) {
		Edge* target = ToNode(g);
		if (target == NULL)
			return false;
		Edge* tmp = this->firstedge;
		if (tmp == target) {
			this->firstedge = target->nextedge;
			delete target;
		}
		else {
			while (tmp->nextedge != target && tmp->nextedge != NULL)
				tmp = tmp->nextedge;
			tmp->nextedge = target->nextedge;
			delete target;
		}
		return true;
	}

	//创造一个新的合并结点，创造连接周围结点的新边，直接架与原图之上，不干扰原图各个结点的数据和指针结构
	Node* createunite(Node *g) {   //没有给g加visit标记
		//创造一个新的结点，
		Node *tmp;

		//创造新的合并节点，更新属性值
		if (g->type_num < 50)
		{
			tmp = new Node(hp + g->hp, attack_increase + g->attack_increase, defence_increase + g->defence_increase, magicdefence_increase + g->magicdefence_increase, g->type_num, g->x, g->y, g->id);
		}
		else {
			int damage = getDamage(attack_increase, defence_increase, magicdefence_increase, g->hp, g->attack_increase, g->defence_increase, g->magicdefence_increase);
			tmp = new Node(hp - damage, attack_increase, defence_increase, magicdefence_increase, g->type_num, g->x, g->y, g->id);
		}
		tmp->yellow_key = yellow_key + g->yellow_key;
		tmp->blue_key = blue_key + g->blue_key;
		tmp->red_key = red_key + g->red_key;

		Edge *e;
		Edge *tmpe = new Edge();	//链表尾指针
		Edge *head = tmpe;			//链表头指针

		/*将出发结点和目的地结点周围连接的所有结点和他们相连的边都存入邻接表，只存出边，不改变入边*/

		//先接目的结点的所有边，直接复制边即可
		for (e = g->firstedge; e != NULL; e = e->nextedge) {	
			if (e->to->visited == 1)	//如果这个结点的标记是1，表示之前访问过，继续访问下一条边
				continue;
			e->to->visited = 1;	//标记我已经访问过，以防产出重边，与-1标记相适应
			if (e->to == this)
				continue;
			tmpe->nextedge = new Edge(e);
			tmpe = tmpe->nextedge;
			
		}

		Edge *ee;
		//再接出发结点的边，每一次都需要寻路一次
		for (e = firstedge; e != NULL; e = e->nextedge) {
			if (e->to->visited == 1)
				continue;
			e->to->visited = 1;
			if (e->to == g)
				continue;
			if ((ee = g->ToNode(e->to)) != NULL)	//如果目的结点也有到达这个结点的路径，直接复制接上
			{
				tmpe->nextedge = new Edge(ee);
			}
			else	//否则为了优化路径，再次寻路一次
			{
				if (is_connected(g->x, g->y, e->to->x, e->to->y)) {
					tmpe->nextedge = new Edge(e->to, cal_trace(g->x,g->y,e->to->x,e->to->y));
				}				
			}
			tmpe = tmpe->nextedge;
		}
		
		tmp->firstedge = head->nextedge;	//把链表挂在结点上
		for (e = tmp->firstedge; e != NULL; e = e->nextedge) {  //重置visited为0
			e->to->visited = 0;
		}
		delete head;
		return tmp;
	}

	//删除e边之后的所有边
	void delete_afteredge(Edge *e) {
		if (e == NULL)
			return;
		delete_afteredge(e->nextedge);
		delete(e);
		return;
	}

	//解构，删除大结点
	void deunite() {
		Edge *tmp = this->firstedge;
		delete_afteredge(tmp);
		delete this;
	}

	//改变英雄状态（最后的版本不使用）
	void change_hero()
	{
		hero_hp += hp;
		hero_attack += attack_increase;
		hero_defence += defence_increase;
		hero_mdefence += magicdefence_increase;
		hero_yellow_key += yellow_key;
		hero_blue_key += blue_key;
		hero_red_key += red_key;
		visited = 1;
	}

	//恢复英雄状态
	void unchange_hero()
	{
		hero_hp -= hp;
		hero_attack -= attack_increase;
		hero_defence -= defence_increase;
		hero_mdefence -= magicdefence_increase;
		hero_yellow_key -= yellow_key;
		hero_blue_key -= blue_key;
		hero_red_key -= red_key;
		visited = 0;
	}
};

/*宽搜存储的结构*/
struct state {
	Node* g;	//当前结点
	Node* prev;	//产生当前结点的previous结点
	string answer;	//走到当前状态的路径
	bool visit[200];	//储存当前哪些结点已经走过
	state(Node *gg, string str, Node* p) :g(gg), answer(str), prev(p) {
		memset(visit, 0, sizeof(visit));
	}
	state(Node *gg, string str, bool v[200], Node* p) :g(gg), answer(str), prev(p) {
		memcpy(visit, v, sizeof(visit));
	}

	//在s的基础上，又经过move字符串走到了graph[mark]
	state(const state &s, Node *cur, string move, Node *p, int mark = 0) {
		g = cur;
		answer = s.answer + move;
		memcpy(visit, s.visit, sizeof(visit));
		visit[mark] = true;
		prev = p;
	}

	//优先队列估值，决定了搜索效率
	bool operator<(const state &a) const {
		int val1, val2;
		val1 = g->hp + 1500 * (g->attack_increase + g->defence_increase) + 60 * g->magicdefence_increase + 300 * (g->yellow_key + g->blue_key + g->red_key);
		val2 = a.g->hp + 1500 * (a.g->attack_increase + a.g->defence_increase) + 60 * a.g->magicdefence_increase + 300 * (a.g->yellow_key + a.g->blue_key + a.g->red_key);
		return val1 < val2;
		//return g->hp - getDamage(g->attack_increase, g->defence_increase, g->magicdefence_increase, monster[99][1], monster[99][2], monster[99][3], monster[99][4])
	//< a.g->hp - getDamage(a.g->attack_increase, a.g->defence_increase, a.g->magicdefence_increase, monster[99][1], monster[99][2], monster[99][3], monster[99][4]);
	}
};

/*读入文件数据*/
void init(fstream &fin) {
	fin >> layer >> row >> col;
	for (int i = 1; i <= row; ++i)
		for (int j = 1; j <= col; ++j)
		{
			fin >> map[i][j];
			if (map[i][j] != 10 && map[i][j] != 11)
				node_num++;
		}
	fin >> power_of_Redgem >> power_of_Bluegem >> power_of_Greengem >> capacity_of_Smallhpbottle >> capacity_of_Largehpbottle;
	int k;
	fin >> k;
	while (k--) {
		int id, hp, att, def, spe;
		fin >> id >> hp >> att >> def >> spe;
		monster[id][0] = 1;
		monster[id][1] = hp;
		monster[id][2] = att;
		monster[id][3] = def;
		monster[id][4] = spe;
	}
	fin >> hero_hp >> hero_attack >> hero_defence >> hero_mdefence >> sx >> sy;
	graph[0] = new Node(hero_hp, hero_attack, hero_defence, hero_mdefence, 0, sx + 1, sy + 1);
}

/*人工向命令行窗口输入数据*/
void initbyhuman() {
	cin >> layer >> row >> col;
	for (int i = 1; i <= row; ++i)
		for (int j = 1; j <= col; ++j)
		{
			cin >> map[i][j];
			if (map[i][j] != 10 && map[i][j] != 11)
				node_num++;
		}
	cin >> power_of_Redgem >> power_of_Bluegem >> power_of_Greengem >> capacity_of_Smallhpbottle >> capacity_of_Largehpbottle;
	int k;
	cin >> k;
	while (k--) {
		int id, hp, att, def, spe;
		cin >> id >> hp >> att >> def >> spe;
		monster[id][0] = 1;
		monster[id][1] = hp;
		monster[id][2] = att;
		monster[id][3] = def;
		monster[id][4] = spe;
	}
	cin >> hero_hp >> hero_attack >> hero_defence >> hero_mdefence >> sx >> sy;
	graph[0] = new Node(hero_hp, hero_attack, hero_defence, hero_mdefence, 0, sx + 1, sy + 1);
}
/*
此函数计算一个怪物对勇士造成的伤害
参数说明：
h_atk: 勇士攻击
h_def: 勇士防御
h_mdef: 勇士魔防
m_hp: 怪物生命值
m_atk: 怪物攻击
m_def: 怪物防御
m_spe: 怪物特效
返回值：
一个整数，代表伤害值。
如果勇士无法对怪物造成伤害，返回999999999

怪物特效说明：
0: 无特效
1: 魔攻（怪物无视勇士的防御）
2: 坚固（怪物防御=勇士攻击-1）
3: 先攻（战斗前怪物首先攻击勇士一次）
4: 2连击（每回合怪物打勇士两次）
*/
int getDamage(int h_atk, int h_def, int h_mdef, int m_hp, int m_atk, int m_def, int m_spe)
{
	if (m_spe == 1) // 魔攻
		h_def = 0;
	if (m_spe == 2) // 坚固
		m_def = h_atk - 1;

	if (h_atk <= m_def) // 无法战斗
		return 999999999;
	if (h_def >= m_atk) // 无法造成伤害
		return 0;

	int damage = 0;
	if (m_spe == 3) // 先攻
		damage += m_atk - h_def;

	int m_times = 1;
	if (m_spe == 4) // 2连击
		m_times = 2;

	damage += (m_hp - 1) / (h_atk - m_def)*(m_atk - h_def)*m_times; // 怪物伤害计算公式
	damage -= h_mdef;

	return damage <= 0 ? 0 : damage;
}

/*构建每一个结点*/
void build_node()
{
	for (int x = 1; x <= 13; x++)
		for (int y = 1; y <= 13; y++)
			if (map[x][y] > 20)
			{
				if (map[x][y] == 21)
				{
					graph[i] = new Node(0, 0, 0, 0, 1, x, y, i);
					graph[i++]->yellow_key = 1;
				}
				if (map[x][y] == 22)
				{
					graph[i] = new Node(0, 0, 0, 0, 1, x, y, i);
					graph[i++]->blue_key = 1;
				}
				if (map[x][y] == 23)
				{
					graph[i] = new Node(0, 0, 0, 0, 1, x, y, i);
					graph[i++]->red_key = 1;
				}
				if (map[x][y] == 24)
					graph[i++] = new Node(0, power_of_Redgem, 0, 0, 1, x, y, i);
				if (map[x][y] == 25)
					graph[i++] = new Node(0, 0, power_of_Bluegem, 0, 1, x, y, i);
				if (map[x][y] == 26)
					graph[i++] = new Node(0, 0, 0, power_of_Greengem, 1, x, y, i);
				if (map[x][y] == 27)
					graph[i++] = new Node(capacity_of_Smallhpbottle, 0, 0, 0, 1, x, y, i);
				if (map[x][y] == 28)
					graph[i++] = new Node(capacity_of_Largehpbottle, 0, 0, 0, 1, x, y, i);
				if (map[x][y] == 31)
				{
					graph[i] = new Node(0, 0, 0, 0, 31, x, y, i);
					graph[i++]->yellow_key = -1;
				}
				if (map[x][y] == 32)
				{
					graph[i] = new Node(0, 0, 0, 0, 32, x, y, i);
					graph[i++]->blue_key = -1;
				}
				if (map[x][y] == 33)
				{
					graph[i] = new Node(0, 0, 0, 0, 33, x, y, i);
					graph[i++]->red_key = -1;
				}
				if (map[x][y] > 50 && map[x][y] <= 70 || map[x][y] == 99)
					graph[i++] = new Node(monster[map[x][y]][1], monster[map[x][y]][2], monster[map[x][y]][3], monster[map[x][y]][4], map[x][y], x, y, i);
			}
}

/*检测两个点在地图上是否能走到*/
bool is_connected(int sx, int sy, int ex, int ey)
{
	if (sx == ex && sy == ey)
		return true;
	Node now;
	memset(vis, 0, sizeof(vis));
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			path[i][j].x = path[i][j].y = -1;
			path[i][j].sp = '?';
		}
	now.x = sx;
	now.y = sy;
	queue<Node> q;
	q.push(now);
	while (q.size() > 0)
	{
		now = q.front();
		
		q.pop();
		Node next = now;
		for (int i = 0; i < 4; i++)
		{
			next.x = now.x + Mov[i][0];
			next.y = now.y + Mov[i][1];
			if (next.x == ex && next.y == ey)
			{
				vis[next.x][next.y] = 1;
				path[next.x][next.y].x = now.x;
				path[next.x][next.y].y = now.y;
				path[next.x][next.y].sp = str[i];
				return true;
			}
			if ((next.x > 0 && next.x <= row &&next.y > 0 && next.y <= col) && map[next.x][next.y] == 10 && (!vis[next.x][next.y])) {
				vis[next.x][next.y] = 1;
				path[next.x][next.y].x = now.x;
				path[next.x][next.y].y = now.y;
				path[next.x][next.y].sp = str[i];
				q.push(next);
			}
		}
	}
	return false;
}

/*在检测后立即调用，构造路径字符串*/
string cal_trace(int sx, int sy, int ex, int ey)  //反向路径
{
	string step = "";
	int nx = ex, ny = ey;
	int nextx = nx, nexty = ny;
	while (1) {
		nx = nextx;
		ny = nexty;
		if (nx == sx && ny == sy)
			break;
		step = path[nx][ny].sp + step;
		nextx = path[nx][ny].x;
		nexty = path[nx][ny].y;
	}
	if (map[ex][ey] > 50 && map[ex][ey] <= 70 || map[ex][ey] == 99)
	{
		step += path[ex][ey].sp;
	}
	return step;
}

/*构建结点之间的边*/
void switch_node()
{
	for (int i = 0; i <= node_num; i++)
		for (int j = i + 1; j <= node_num; j++)
		{
			if (is_connected(graph[i]->x, graph[i]->y, graph[j]->x, graph[j]->y))
			{
				string s = cal_trace(graph[i]->x, graph[i]->y, graph[j]->x, graph[j]->y);
				graph[i]->SetEdge(graph[j], s);
				graph[j]->SetEdge(graph[i], rev(s, graph[i]->x, graph[i]->y, graph[j]->x, graph[j]->y));
			}
		}
}

/*翻转字符串（最后的版本没有用到），sx,sy是s的起点，ex、ey是s的终点*/
string rev(string s, int sx, int sy, int ex, int ey, int type)
{
	string temp = "";
	int l = s.length();
	if (map[ex][ey] > 50 && map[ex][ey] <= 70 || map[ex][ey] == 99)
		l--;
	for (int i = l - 1; i >= 0; i--)
	{
		if (s[i] == 'w')
			temp += 's';
		if (s[i] == 's')
			temp += 'w';
		if (s[i] == 'a')
			temp += 'd';
		if (s[i] == 'd')
			temp += 'a';
	}
	if (type == 0 && map[sx][sy] > 50 && map[sx][sy] <= 70 || map[sx][sy] == 99)
	{
		temp += temp[l-1];
	}
	return temp;
}

/*建图*/
void build_map()
{
	build_node();
	switch_node();
}

/*优先队列的宽搜
 *
 *将需要搜索的合并结点按照一定的估值放到优先队列，结点保存了英雄的当前状态
 *在每次搜索一个结点时，如果结点后面是宝物，直接吃掉宝物，并放入优先队列，等待下一次搜索，这样可以使得我每次获取的大结点周围只有门或者怪物结点
 *如果在开门或者战斗后，不会发现任何的宝物，那么我必须沿着这条路向下探索，直到有宝物或者不能探索为止，不可以到其他路上去，否则可以在其他路进行搜索时不去消耗你现在这条路上的结点，而是获取了收益后再来走这个结点，这样一定会得到不差的路径（剪枝）
 *如果我遇到了boss并且能够战胜它，我会像战胜怪物一样把他放入临时搜索队列，并且得不到收益，因此可以在临时搜索队列得到这个结点状态并输出
 */
void BFS(Node *g) {
	//保存地图数据
	int temp_map[15][15];	
	memcpy(temp_map, map, sizeof(map));

	//优先队列
	priority_queue<state> q;
	//创建初始state结构，即起始状态
	state cur(g, ans, NULL);
	cur.g->id = 0;
	cur.visit[0] = true;
	int sum = 1;
	//寻找起点周围的收益
	//有可能在走到收益结点后又发现新的收益结点，所以用while循环直到看不到收益结点
	while (sum) {
		sum = 0;
		for (Edge *e = cur.g->firstedge; e != NULL; e = e->nextedge)
		{
			if ((e->to->type_num == 1
				|| (e->to->type_num > 50 && e->to->type_num < 99 && getDamage(cur.g->attack_increase, cur.g->defence_increase, cur.g->magicdefence_increase, e->to->hp, e->to->attack_increase, e->to->defence_increase, e->to->magicdefence_increase) == 0))
				&& cur.visit[e->to->id] == false)
			{
				sum++;	//收益数量，标记图有改动，有可能发现新的收益
				e->to->visited = 1;
				Node *tmp = cur.g->createunite(e->to);
				//直接更改cur属性
				cur.answer += cur.g->ToNode(e->to)->move;
				cur.g = tmp;
				cur.visit[e->to->id] = true;
				map[e->to->x][e->to->y] = 10;
			}
		}
	}
	q.push(cur);	//插入起始结点
	while (!q.empty())
	{
		memcpy(map, temp_map, sizeof(map));
		cur = q.top();
		q.pop();

		/*限制时间*/
		//clock_t t = clock();
		//if (t > 300000)
		//{
		//	cerr << "Time Out：" << t <<endl;
		//	break;
		//}


		//恢复结点记录的地图状态
		for (int i = 0; i <= node_num; i++)
		{
			if (cur.visit[i] == true)
			{
				graph[i]->visited = 1;
				map[graph[i]->x][graph[i]->y] = 10;
			}
			else
				graph[i]->visited = 0;
		}

		/*寻找当前打怪最多的路线*/
		/*int msum = 0;
		for (int i = 1; i <= node_num; i++)
			if (graph[i]->visited == 1 && graph[i]->type_num > 50)
				msum++;
		if (msum > monster_defeat)
		{
			monster_defeat = msum;
			ans = cur.answer;
		}*/

		/*打败了boss，输出（其实不会到这一步）*/
		if (cur.g->type_num == 99)
		{
			/*寻找最优解*/
			//if (cur.g->hp > anshp)
			//{
			//	anshp = cur.g->hp;
			//	bestway = cur.answer;
			//}
			//cout << cur.answer << endl;
			fout << cur.answer << endl;
			//continue;
			//break;
			return;

		}

		//fout << cur.answer << endl;

		queue<state> sNo;	//临时队列

		//寻找我能走的路线,存入临时队列中
		for (Edge *e = cur.g->firstedge; e != NULL; e = e->nextedge)
		{

			if (cur.visit[e->to->id] == false)
			{
				//怪打不过
				if (e->to->type_num > 50 && cur.g->hp <= getDamage(cur.g->attack_increase, cur.g->defence_increase, cur.g->magicdefence_increase, e->to->hp, e->to->attack_increase, e->to->defence_increase, e->to->magicdefence_increase))
					continue;

				//钥匙不够
				if (e->to->type_num == 31 && cur.g->yellow_key == 0)
					continue;
				if (e->to->type_num == 32 && cur.g->blue_key == 0)
					continue;
				if (e->to->type_num == 33 && cur.g->red_key == 0)
					continue;

				//e这条路能走
				e->to->visited = 1;
				Node *tmp = cur.g->createunite(e->to);	//合并结点，插入临时队列
				//队列保存我需要处理的结点状态
				sNo.push(state(cur, tmp, e->move, cur.g, e->to->id));
				e->to->visited = 0;
			}
		}
		int get_up = 0;

		//将临时队列的结点放入优先队列（收取收益或者继续向前）
		while (!sNo.empty()) {
			state cur_state = sNo.front();
			sNo.pop();

			//如果当前结点已经打败了boss，输出结果
			if (cur_state.g->type_num == 99)
			{
				//寻找最优解（需要跑完整张图，还未能有效剪枝，故无法寻找）
				//if (cur_state.g->hp > anshp)
				//{
				//	anshp = cur_state.g->hp;
				//	bestway = cur_state.answer;
				//}
				fout << cur_state.answer << endl;
				//cout << cur_state.answer << endl;
				//continue;
				//break;
				return;
			}

			//恢复当前地图：访问过的结点变成道路[10]，做上1标记，父节点能抵达的结点标记-1，用于剪枝
			memcpy(map, temp_map, sizeof(map));
			for (int i = 0; i <= node_num; i++)
			{
				if (cur_state.visit[i] == true)
				{
					graph[i]->visited = 1;
					map[graph[i]->x][graph[i]->y] = 10;
				}
				else
					graph[i]->visited = 0;
			}

			for (Edge *ee = cur_state.prev->firstedge; ee != NULL; ee = ee->nextedge)
			{
				ee->to->visited = -1;
			}
			graph[cur.g->id]->visited = 1;


			get_up = 0;
			sum = 1;
			//获取收益
			//有可能在走到收益结点后又发现新的收益结点，所以用while循环直到看不到收益结点
			while (sum)
			{
				sum = 0;
				for (Edge *ee = cur_state.g->firstedge; ee != NULL; ee = ee->nextedge)
				{
					if ((ee->to->type_num == 1
						|| (ee->to->type_num > 50 && ee->to->type_num < 99 && getDamage(cur_state.g->attack_increase, cur_state.g->defence_increase, cur_state.g->magicdefence_increase, ee->to->hp, ee->to->attack_increase, ee->to->defence_increase, ee->to->magicdefence_increase) == 0))
						&& cur_state.visit[ee->to->id] == false)
					{
						sum++;
						if (ee->to->type_num == 1)	//是宝物结点，获取了收益
							get_up = 1;
						ee->to->visited = 1;	//标记合并结点（根据合并的原理，我一定能够找到大节点到现在发现的收益的这条路）
						Node *uni = cur_state.g->createunite(ee->to);
						cur_state.answer += cur_state.g->ToNode(ee->to)->move;	//记录路线
						cur_state.g = uni;	//换大节点
						cur_state.visit[ee->to->id] = true;	//更改visit状态
						map[ee->to->x][ee->to->y] = 10;		//更改地图状态
					}
				}
			}
			//如果我没找到收益，就继续沿着这条路前进一个结点，插入到当前需要探索的临时队列（直到获得收益为止）
			if (get_up == 0)
			{
				for (Edge *ee = graph[cur_state.g->id]->firstedge; ee != NULL; ee = ee->nextedge)
				{
					if (ee->to->visited == 0)
					{
						if (ee->to->type_num > 50 && cur_state.g->hp <= getDamage(cur_state.g->attack_increase, cur_state.g->defence_increase, cur_state.g->magicdefence_increase, ee->to->hp, ee->to->attack_increase, ee->to->defence_increase, ee->to->magicdefence_increase))
							continue;
						if (ee->to->type_num == 31 && cur_state.g->yellow_key == 0)
							continue;
						if (ee->to->type_num == 32 && cur_state.g->blue_key == 0)
							continue;
						if (ee->to->type_num == 33 && cur_state.g->red_key == 0)
							continue;
						//将下一步要走的结点存入临时队列
						ee->to->visited = 1;
						Node *uni = cur_state.g->createunite(ee->to);
						sNo.push(state(cur_state, uni, ee->move, cur_state.g, ee->to->id));
						//回溯：恢复地图状态
						for (Edge *Ee = cur_state.prev->firstedge; Ee != NULL; Ee = Ee->nextedge)
						{
							Ee->to->visited = -1;
						}
						graph[cur_state.g->id]->visited = 1;
						ee->to->visited = 0;
					}
				}
			}
			else {
				//将收益后的结点插入优先队列（所有的状态变化已经放入cur_state中了）
				q.push(cur_state);
			}

		}
	}
	return;
}

/*深搜尝试（最后的版本不用）*/
void search(Node* g)
{
	//cout << g->x << ' ' << g->y << endl;
	//cout << ans << endl;
	//system("pause");
	Node*tmp = NULL;//next node
	Node *uni = NULL;  //指向合并后的结点
	int tmp_type;
	string tmpstr = ans;
	//treasure
	for (Edge*temp = g->firstedge; temp != NULL; temp = temp->nextedge)//if finding a treasure node,get that and change position
	{
		tmp = temp->to;
		if (tmp->type_num != 1 || tmp->visited == 1)//if it is not treasure or it has been visited
			continue;

		tmp->change_hero();//change hero status
		tmp_type = map[tmp->x][tmp->y];
		uni = g->createunite(tmp);
		map[tmp->x][tmp->y] = 10;
		ans += temp->move;
		
		search(uni);//unite two node into next position node and go there

		tmp->unchange_hero();//开始回溯了！
		ans = tmpstr;
		map[tmp->x][tmp->y] = tmp_type;
		uni->deunite();//change to its original looks
		uni = NULL;

		return;
	}

	//door or monster
	for (Edge*temp = g->firstedge; temp != NULL; temp = temp->nextedge)
	{
		tmp = temp->to;
		if (tmp->visited)
			continue;
		
		if (tmp->type_num == 99)
		{
			//cout << ans << endl;
			//system("pause");
			int damage;
			int m_hp = tmp->hp, m_atk = tmp->attack_increase, m_def = tmp->defence_increase, m_spe = tmp->magicdefence_increase;
			damage = getDamage(hero_attack, hero_defence, hero_mdefence, m_hp, m_atk, m_def, m_spe);//calculate damage
			if (hero_hp > damage)
			{
				if (anshp < hero_hp - damage)
				{
					anshp = hero_hp - damage;
					bestway = ans + temp->move;
				}
				//cout << "answer: " << ans + temp->move << endl;
				//system("pause");
			}
		}
		else if (tmp->type_num == 31 && hero_yellow_key > 0//yellow
			|| tmp->type_num == 32 && hero_blue_key > 0//blue
			|| tmp->type_num == 33 && hero_red_key > 0)//red
		{
			tmp->change_hero();//change hero status
			tmp_type = map[tmp->x][tmp->y];
			uni = g->createunite(tmp);
			ans += temp->move;
			map[tmp->x][tmp->y] = 10;
			
			search(uni);//unite two node into next position node and go there

			tmp->unchange_hero();//开始回溯了！
			map[tmp->x][tmp->y] = tmp_type;
			ans = tmpstr;
			uni->deunite();//change to its original looks
			uni = NULL;
		}
		else if (tmp->type_num <= 70 && tmp->type_num >= 51)//monster
		{
			int damage;
			int m_hp = tmp->hp, m_atk = tmp->attack_increase, m_def = tmp->defence_increase, m_spe = tmp->magicdefence_increase;
			damage = getDamage(hero_attack, hero_defence, hero_mdefence, m_hp, m_atk, m_def, m_spe);//calculate damage
			
			if (hero_hp <= damage)
				continue;

			hero_hp -= damage;
			tmp->visited = 1;
			tmp_type = map[tmp->x][tmp->y];
			uni = g->createunite(tmp);
			map[tmp->x][tmp->y] = 10;
			ans += temp->move;

			search(uni);//unite two node into next position node and go there

			map[tmp->x][tmp->y] = tmp_type;
			tmp->visited = 0;
			hero_hp += damage;
			ans = tmpstr;
			tmp->visited = 0;
			uni->deunite();

		}
	}

}

int main() {
	fin.open("input.txt");
	fout.open("output.txt");
	fout.clear();

	//initbyhuman();
	init(fin);
	build_map();
	graph[0]->visited = 1;
	//search(graph[0]);
	BFS(graph[0]);
	
	//if (anshp > 0)
	//	fout << "Can beat boss : " << bestway << endl;	//输出最优路径
	//else
	//	fout << "Can't beat boss : " << ans << endl;
	fin.close();
	fout.close();
	//system("pause");
	return 0;
}

/*
输入文件为input.txt，输出文件为output.txt

--------------------------------------------------------------

输入说明：

第一行一个数，为层数，在本项目中恒为1
第二行两个数m和n，代表地图长宽；在本项目中m和n均恒为13。

接下来是一个m*n的矩阵，每个点一个数，代表地图的样子。

其中：
10     路
11     墙

21    黄钥匙
22    蓝钥匙
23    红钥匙
24    红宝石（加攻击）
25    蓝宝石（加防御）
26    绿宝石（加魔防）
27    小血瓶（加生命）
28    大血瓶（加生命）

31    黄门
32    蓝门
33    红门

51~70 普通怪物
99    boss

接下来一行有五个数，分别代表红宝石加攻击的数值、蓝宝石加防御的数值、绿宝石加魔防的数值、小血瓶加生命的数值和大血瓶加生命的数值。
例如  1 1 50 50 100  代表拾取红宝石可以加1点攻击，拾取小血瓶可以加50点生命；以此类推。


接下来一个数k，代表不同种类的怪物个数。
之后有k行，每行5个数，分别代表怪物的ID、生命、攻击、防御、特效。

如：52 200 15 5 2  代表52号怪物的生命值为200，攻击15，防御5，且为坚固怪物。

有关每个怪物特效的说明，以及对勇士的伤害计算公式可以参见 damage.cpp （直接调用即可）


最后一行有6个数，分别为勇士的初始生命、攻击、防御、魔防、初始位置的横坐标、初始位置的纵坐标。



--------------------------------------------------------------

输出说明：

直接输出一行由w,a,s,d组成的字符串，以换行结束。


备注：

·勇士前进方向为怪物，则与怪物发生战斗；勇士不前进；
·勇士前进方向为门，则进行开门操作；勇士前进
·勇士前进方向为宝物，则拾取宝物；勇士前进


将输出结果保存成output.txt文件；放到mota.exe目录下可以自动根据输出进行操作。


*/
