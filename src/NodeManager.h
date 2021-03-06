#pragma once

#include "Node.h"
#include "AscendingNodeQueue.h"

class NodeManager {
public:
	NodeManager();
	~NodeManager() = default;
	int get_goal = false;
	float moveCost(int x_diff, int y_diff);
	eResult search();
	void getPath(Node goal);
	void clearList();
	float calcIM_cost(Node* node);
	void update();
	void draw();

	int flame_cnt;
	//priority_queue<Node*, vector<Node*>, NodeCompare> openList, closeList;	//ノードのポインタを格納する優先度付きキュー、ソートは昇順
	NodeList openList, closeList;												//優先度付きキューで利用する機能に加えて任意タイミングでのソート機能を加えた自作クラス
	vector<Point> *root;														//探索経路を格納する配列
	Node **grid = new Node*[Define::WIN_H];
	Node* start;																//スタートノード
	Human human[HUMAN];
	Player player;
	Goal goal;
	SquareBlock block[BLOCKS];
};
