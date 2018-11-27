#include "Node.h"
#include <math.h>
using namespace std;

//書き出し用に追加
#include <DxLib.h>

// 優先度付きキューのヒープ構造中にて比較演算子"<" のオーバーロード
//scoreを基準にnodeの大小比較を行う
bool operator> (const Node &node1, const Node &node2) {
	return node1.score > node2.score;
};


Node::Node() {
	status = None;
	parent = nullptr;
}

void Node::calcDistance(int goal_x,int goal_y){
	distance = sqrtf((float)(pow(x - goal_x, 2.0) + pow(y - goal_y, 2.0)));
}

//ここで何故かdistanceが空の値になってる
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

NodeManager::NodeManager() {}

void NodeManager::Initialize(Player player,Goal goal){
	//Nodeの領域を動的生成
	for (int cnt = 0; cnt < Define::WIN_H; cnt++) {
		grid[cnt] = new Node[Define::WIN_W];
	}

	//グリッドに座標を指定しゴール地点までの距離を計算
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calcDistance(goal.x,goal.y);
		}
	}

	goal_x = goal.x;
	goal_y = goal.y;

	//スタートノードをオープンリストへ
	openList.push(grid[(int)player.y][(int)player.x]);
}

Node NodeManager::search(Node* node){
	//実コストの計算
	if ((*node).parent == nullptr) {
		(*node).g_Cost = 0;	//親がいない＝スタート地点なので実コスト0
		(*node).calcScore();
	}

	//（影響度の計算）
	

	//親ノードをオープンリストからpop
	openList.pop();

	//周辺の8つを子ノードとしてオープンリストに入れる
	for (int cnt_x = -1; cnt_x < 2; cnt_x++) {
		for (int cnt_y = -1; cnt_y < 2; cnt_y++) {
			//子ノードの座標を決定
			int child_x = (*node).x + cnt_x;
			int child_y = (*node).y + cnt_y;

			//中央ノードは処理から除外する
			if (!(cnt_x == 0 && cnt_y == 0)) {
				//ステータスがNoneのノードのみ操作する
				if (grid[child_y][child_x].IsNone()) {
					grid[child_y][child_x].s_Open();					//ノードステータスをオープンに変更
					grid[child_y][child_x].parent = node;				//中央のノードを親ノードとしてセット
					grid[child_y][child_x].g_Cost = (*node).g_Cost + 1;	//親の実コストに1加算
					grid[child_y][child_x].calcScore();
					printfDx("(%3d,%3d)のscoreは%f\n", child_x, child_y, grid[child_y][child_x].score);
					openList.push(grid[child_y][child_x]);
				}
			}
		}
	}

	closeList.push(*node);	//親ノードはクローズリストへ格納
	(*node).s_Close();
	if (goal_x == openList.top().x && openList.top().y == goal_y) {
		get_goal = true;
		printfDx("ゴール地点が見つかりました\n");
		return openList.top();
	}
	return openList.top();
	//return search(openList.top());	//オープンリスト中で合計コストの最小ノードを返す
	//再帰的に書けばゴール地点にたどり着くまで探索を続けられる
}

//現在のオープンリスト内のノード確認をする用
void NodeManager::output(Node node){
	openList.pop();
	closeList.push(node);
}

//リスト中のノードのステータスとコストを初期化して全てpopする
void NodeManager::clear(priority_queue<Node, vector<Node>, greater<Node>> list){
	while (!list.empty()){
		//To do ：const外ししつつステータスをNoneに戻したい
		Node* node;
		//node = &(list.top());


		list.pop();
	}
}
