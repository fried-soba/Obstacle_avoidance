#pragma once

#include "AI.h"
#include "Define.h"
#include <queue>
using namespace std;

//各Grid(=長さ1の正方形のノード)のステータス
class Node {
public:
	Node();
	~Node() = default;
	enum eStatus{
		None,
		Open,
		Close
	};
	eStatus status;
	int x, y;			//ノード座標
	int g_Cost;			//実コスト
	float distance;		//推定コストその1：ゴール地点までの直線距離
	int i_Cost;			//推定コストその2：InfluenceMapで計算する
	float score;			//コスト合計値

	Node *parent;			//親ノードのポインタ
	void calcDistance(int goal_x,int goal_y);
	void calcScore();		//合計スコアを計算
	void s_None();
	void s_Open();
	void s_Close();
	bool IsNone();
};

//配列にして探索の完了したルートを格納する用の構造体
typedef struct coordinates {
	int x;
	int y;
} Root;

class NodeManager{
public:
	NodeManager();
	void Initialize(Player player, Goal goal);
	~NodeManager() = default;
	int goal_x, goal_y;
	int get_goal = false;
	Node search(Node *node);
	void getPath(Node *goal);
	void output(Node node);
	void clear(priority_queue<Node, vector<Node>, greater<Node>> list);
//private:
	priority_queue<Node, vector<Node>, greater<Node>> openList, closeList;	//ノードを格納する優先度付きキュー、ソートは昇順
	vector<Root> root_array;														//探索経路を格納する配列
	Node **grid = new Node*[Define::WIN_H];
};