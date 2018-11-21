#include "Node.h"
#include <math.h>
using namespace std;


// 優先度付きキューのヒープ構造中にて比較演算子"<" のオーバーロード
//scoreを基準にnodeの大小比較を行う
bool operator> (const Node &node1, const Node &node2) {
	return node1.score > node2.score;
};


Node::Node() {
	status = None;
	parent = nullptr;
}

void Node::calcDistance(Goal goal){
	distance = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));
}

void Node::calcScore(){
	score = (float)(g_Cost + distance/*+i_Cost*/);	//影響度計算が実装したら合計コストに含める
}

void Node::s_None(){
	status = None;
}

void Node::s_Open(){
	status = Open;
}

void Node::s_Close(){
	status = Close;
}

bool Node::IsNone(){
	if (status == None)
		return true;
	else
		return false;
}


NodeManager::NodeManager(){
	
}

Node NodeManager::search(Node node){
	//実コストの計算
	if (node.parent != nullptr)
		node.g_Cost = node.parent->g_Cost + 1;
	else
		node.g_Cost = 0;	//親がいない＝スタート地点なので実コスト0


	//（影響度の計算）
	
	//合計コストの計算
	node.calcScore();

	//周辺の8つを子ノードとしてオープンリストに入れる
	extern Node **_grid;											//ここが別クラス中の_gridとリンクされない
	for (int cnt_x = -1; cnt_x < 2; cnt_x++) {
		for (int cnt_y = -1; cnt_y < 2; cnt_y++) {
			if (cnt_x != 0 && cnt_y != 0) {
				//未オープンのノードのみ操作する
				if (_grid[node.y + cnt_y][node.x + cnt_x].IsNone()) {
					_grid[node.y + cnt_y][node.x + cnt_x].parent = &node;		//中央のノードを親ノードとしてセット
					_grid[node.y + cnt_y][node.x + cnt_x].s_Open();				//ノードステータスをオープンに変更
					openList.push(_grid[node.y + cnt_y][node.x + cnt_x]);
				}
			}
		}
	}


	closeList.push(node);	//親ノードはクローズリストへ格納
	node.s_Close();
	return openList.top();	//オープンリスト中で合計コストの最小ノードを返す
	//再帰的に書けばゴール地点にたどり着くまで探索を続けられる
}

//現在のオープンリスト内のノード確認をする用
void NodeManager::output(Node node){
	openList.pop();
	closeList.push(node);
}

void NodeManager::clear(priority_queue<Node, vector<Node>, greater<Node>> list){
	while (!list.empty()){
		list.top().s_None();
		list.pop();
	}
}
