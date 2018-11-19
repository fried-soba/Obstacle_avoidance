#include "Node.h"
#include <math.h>
using namespace std;


// 優先度付きキューのヒープ構造中にて比較演算子"<" のオーバーロード
//scoreを基準にnodeの大小比較を行う
bool operator> (const Node &node1, const Node &node2) {
	return node1.score > node2.score;
};


Node::Node() {
}

void Node::calcDistance(Goal goal){
	//distance = sqrtf(powf(x - goal.x, 2.0) + powf(y - goal.y, 2.0));
	distance = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));
}

void Node::calcScore(){
	score = (float)(g_Cost + distance/*+i_Cost*/);	//影響度計算が実装したら合計コストに含める
}


NodeManager::NodeManager(){
	
}

void NodeManager::open(Node node){
	//実コストの計算
	if (node.parent != nullptr)
		node.g_Cost = node.parent->g_Cost + 1;
	else
		node.g_Cost = 0;	//親がいない＝スタート地点ならコスト0
	//直線距離の計算
	//（影響度の計算）
	//合計コストの計算
	node.calcScore();
	openList.push(node);
}

void NodeManager::close(Node node){
	openList.pop();
	closeList.push(node);
}

void NodeManager::clear(priority_queue<Node, vector<Node>, greater<Node>> list){
	while (!list.empty()){
		list.pop();
	}
}
