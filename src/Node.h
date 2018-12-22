#pragma once

#include "AI.h"
#include "Define.h"
#include <queue>
#define BLOCKS 11      	//適当に配置する静的障害物の数

enum eStatus {
	None,
	Open,
	Close,
	Block
};

//各Grid(=長さ1の正方形のノード)のステータス
class Node {
public:
	Node();
	~Node() = default;
	eStatus status;
	int x, y;			//ノード座標
	float g_Cost;			//実コスト
	float suppositionScore;		//仮想f(n)
	float fromStartDistance;	//スタート地点からの直線距離（ 仮想g(n) ）
	float toGoalDistance;		//推定コストその1：ゴール地点までの直線距離（ 仮想h(n) ）
	int i_Cost;			//推定コストその2：InfluenceMapで計算する
	float score;			//コスト合計値

	Node *parent;			//親ノードのポインタ
	void calcDistance(Node start, Goal goal);
	void calcScore();		//合計スコアを計算
};

struct NodeCompare {
	bool operator()(const Node* a, const Node* b) const {
		return a->toGoalDistance > b->toGoalDistance;
	}
};

class SquareBlock {
	int x, x_end;
	int y,  y_end;
	int thickness,length;
	unsigned int color;
public:
	SquareBlock();
	void draw();
	void giveGrid(Node** grid);
	Node **_grid;
};

class NodeManager{
public:
	NodeManager();
	void Initialize(Player player, Goal goal);
	~NodeManager() = default;
	int goal_x, goal_y;
	int get_goal = false;
	Node search(Node *node);
	void getPath(Node *goal);
	void output(Node *node);
	void clear(priority_queue<Node*, vector<Node*>, NodeCompare> list);
//private:
	priority_queue<Node*, vector<Node*>, NodeCompare> openList;	//ノードのポインタを格納する優先度付きキュー、ソートは昇順
	vector<Point> root;														//探索経路を格納する配列
	Node **grid = new Node*[Define::WIN_H];
	SquareBlock block[BLOCKS];
};