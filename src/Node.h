#pragma once

#include "AI.h"
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
	int x, y;
	int g_Cost;			//実コスト
	float distance;		//推定コストその1：ゴール地点までの直線距離
	int i_Cost;			//推定コストその2：InfluenceMapで計算する
	float score;			//コスト合計値

	Node *parent;			//親ノードのポインタ
	void calcDistance(Goal goal);
	void calcScore();		//合計スコアを計算
	void s_None();
	void s_Open();
	void s_Close();
	bool IsNone();
};

class NodeManager{
public:
	NodeManager();
	~NodeManager() = default;
	
	Node search(Node node);
	void output(Node node);
	void clear(priority_queue<Node, vector<Node>, greater<Node>> list);
//private:
	priority_queue<Node, vector<Node>, greater<Node>> openList, closeList;	//ノードを格納する優先度付きキュー、ソートは昇順
};