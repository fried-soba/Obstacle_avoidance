#pragma once

#include "AI.h"
#include "Define.h"
#include <queue>

enum eStatus {
	None,
	Open,
	Close,
	Block
};

enum eResult {
	unReach,
	notFound,
	arrival
};

//各Grid(=長さ1の正方形のノード)のステータス
class Node {
public:
	Node();
	~Node() = default;
	eStatus status;
	int x, y;					//ノード座標
	float g;					//実コスト
	float h;					//推定コスト
	float i_Cost;				//推定コストその2：InfluenceMapで計算する
	float score;				//コスト合計値 f

	Node *parent;			//親ノードのポインタ
	void calc_hCost(Goal goal);
	void calcScore();		//合計スコアを計算
};

//オープンリストの優先度付きキューに用いるソート基準
struct NodeCompare {
	bool operator()(const Node* a, const Node* b) const {
		return a->score > b->score;
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
	~NodeManager() = default;
	int goal_x, goal_y;
	int get_goal = false;
	void Initialize(Player *player, Goal goal,Human *human);
	float moveCost(int x_diff, int y_diff);
	eResult search(Node* node);
	void getPath(Node *goal);
	void clearList();
	float calcIM_cost(Node* node);

	priority_queue<Node*, vector<Node*>, NodeCompare> openList, closeList;	//ノードのポインタを格納する優先度付きキュー、ソートは昇順
	vector<Point> root;														//探索経路を格納する配列
	Node **grid = new Node*[Define::WIN_H];
	Human *human;
	Player *player;
	SquareBlock block[BLOCKS];
};