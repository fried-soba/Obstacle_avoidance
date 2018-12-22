#include "Node.h"
#include <math.h>
using namespace std;

//書き出し用に追加
#include <DxLib.h>


Node::Node() {
	status = None;
	parent = nullptr;
}

void Node::calcDistance(Node start, Goal goal) {
	//直線距離
	fromStartDistance= sqrtf((float)(pow(x - start.x, 2.0) + pow(y - start.y, 2.0)));
	toGoalDistance = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));
	suppositionScore = fromStartDistance + toGoalDistance;

	//x軸,y軸の差の和をとってみる
	//toGoalDistance = fabsf(x - goal_x) + fabsf(y - goal_x);

	/*//x軸,y軸の差のうち、大きい方を取る
	float tx = fabsf(goal_x - x);
	float ty = fabsf(goal_y - y);
	if (tx > ty)
		toGoalDistance = tx;
	toGoalDistance = ty;*/
}

void Node::calcScore(){
	score = (g_Cost + toGoalDistance/*+i_Cost*/);	//影響度計算が実装したら合計コストに含める
}

SquareBlock::SquareBlock() {
	x = Define::WIN_W * 1 / 5 + GetRand(Define::WIN_W * 2 / 5);		//スタート-ゴール間のどこか
	y = 100 + GetRand(Define::WIN_H - 200);							//上下端100は空くようにする
	thickness = GetRand(200);
	length = GetRand(500);
	bool _direction = GetRand(1);
	if (_direction) {
		x_end = x + thickness;
		y_end = y + length;
	}
	else {
		x_end = x + length;
		y_end = y + thickness;
	}
	if (x_end > Define::WIN_W*4/5) {
		x_end = Define::WIN_W*4/5 - 1;
		printfDx("ブロックがゴールに重なりそうなので修正したよ\n");
	}
	if (y_end > Define::WIN_H) {
		y_end = Define::WIN_H - 1;
		printfDx("ブロックがy軸をはみ出したので修正したよ\n");
	}
	/*
	for (int cnt_y = y; cnt_y < y_end; y++) {
		for (int cnt_x = x; cnt_x < x_end; cnt_x++){
			_grid[cnt_y][cnt_x].status = Close;
		}
	}
	*/
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));
}

void SquareBlock::draw() {
	DrawBox(x, y, x_end, y_end, color, TRUE);
}

void SquareBlock::giveGrid(Node ** grid)
{
	_grid = grid;
	for (int cnt_y = y; cnt_y < y_end; cnt_y++) {
		for (int cnt_x = x; cnt_x < x_end; cnt_x++)
			_grid[cnt_y][cnt_x].status = Block;
	}
}


NodeManager::NodeManager() {}

void NodeManager::Initialize(Player player,Goal goal){
	//Nodeの領域を動的生成
	for (int cnt = 0; cnt < Define::WIN_H; cnt++) {
		grid[cnt] = new Node[Define::WIN_W];
	}

	//仮想スタートノード
	Node _start;
	_start.x = player.x;	_start.y = player.y;

	//グリッドに座標を指定しゴール地点までの距離を計算
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calcDistance(_start,goal);
		}
	}

	//画面端の座標は最初からクローズにしておく
	for (int hoge = 0; hoge < Define::WIN_W; hoge++) {
		//横方向
		grid[0][hoge].status = Close;
		grid[Define::WIN_H - 1][hoge].status = Close;
	}
	for (int fuga = 0; fuga < Define::WIN_H; fuga++) {
		//縦方向
		grid[fuga][0].status = Close;
		grid[fuga][Define::WIN_W - 1].status = Close;
	}

	goal_x = goal.x;
	goal_y = goal.y;

	//スタートノードをオープンリストへ
	openList.push(&grid[(int)player.y][(int)player.x]);

	block[0]._grid = grid;
}

Node NodeManager::search(Node* center){
	//実コストの計算
	if (center->parent == nullptr) {
		center->g_Cost = 0;	//親がいない＝スタート地点なので実コスト0
		center->calcScore();
	}

	//（ここに影響度の計算）
	

	//親ノードをオープンリストからpop
	openList.pop();
	center->status = Close;

	//周辺の8つを子ノードとしてオープンリストに入れる
	for (int cnt_x = -1; cnt_x <= 1; cnt_x++) {
		for (int cnt_y = -1; cnt_y <= 1; cnt_y++) {
			//中央ノードは処理から除外する
			if (!(cnt_x == 0 && cnt_y == 0)) {
				
				//子ノードの座標を決定
				Node* child = &grid[center->y + cnt_y][center->x + cnt_x];

				if (child->status == None) {
					child->status = Open;
					child->parent = center;					
					if (cnt_x == 0 || cnt_y == 0)
						child->g_Cost = center->g_Cost + 1;			//縦横の子は実コストは親に1加算
					else
						child->g_Cost = center->g_Cost + sqrtf(2);	//斜めの子は実コストは親にルート2加算
					openList.push(child);
				}
				/*追加思案中
				if (child->status = Open) {
					//g_Costを弄るべきかは経路見て今後判断
					if (center->suppositionScore > child->suppositionScore)
						child->parent = center->parent;
				}
				*/
				/*追加思案中
				if (child->status == Close) {
					if (center->suppositionScore > child->suppositionScore) {
						child->parent = center->parent;
						center->suppositionScore = child->suppositionScore;
						child->status = Open;
					}
				}
				*/


				/*旧アルゴリズム
				//ステータスがNoneのノードのみ操作する
				if (child->status == None) {
					child->status = Open;					//ノードステータスをオープンに変更
					child->parent = center;				//中央ノードポインタを親としてセット
					if (cnt_x == 0 || cnt_y == 0)
						child->g_Cost = center->g_Cost + 1;	//縦横の子は実コストは親に1加算
					else
						child->g_Cost = center->g_Cost + sqrtf(2);//斜めの子は実コストは親にルート2加算
					
					child->calcScore();
					//printfDx("(%3d,%3d)のscoreは%f\n", child_x, child_y, child->score);
 					openList.push(&(grid[child_y][child_x]));
				}
				*/
			}
		}
	}
	
	//スタック領域のオーバーフロー対策	//追記実行中のメモリ使用量を見ると対策になっていない模様
	static int cnt = 0;		//再帰できる深さをcntで制限する
	cnt++;
	if (cnt>30000) {
		cnt = 0;
		//return (*openList.top());
		clear(openList);	//スタック領域のオーバーフロー対策
	}
	
	if (openList.size() == 0) {
		printfDx("探索に失敗しました。オープンリストが空です\n");
		//失敗した場合は返却なにも返したくないが、何を返すべきか検討
		//return (Node*)nullptr;
	}

	if (goal_x == openList.top()->x && openList.top()->y == goal_y) {
		get_goal = true;
		printfDx("ゴール地点が見つかりました\n");
		Node _tmp = (*openList.top());	//オープンリストの要素に対するconst回避の為のバッファ
		getPath(&_tmp);
		return (*openList.top());
	}
	int top_x = openList.top()->x;
	int top_y = openList.top()->y;
	return search(&(grid[top_y][top_x]));	//オープン済みの中で合計コストが最小のノードを返す
	//再帰的に書けばゴール地点にたどり着くまで探索を続けられる
}

void NodeManager::getPath(Node* _goal){
	while (_goal->parent!=nullptr) {
		Point _tmp;
		_tmp.x = _goal->x;
		_tmp.y = _goal->y;
		root.push_back(_tmp);
		return getPath(_goal->parent);
	}
	printfDx("経路を取得・格納しました。\n");
	return;
}

//現在のオープンリスト内のノード確認をする用
void NodeManager::output(Node *node){
	openList.pop();
}

//リスト中のノードのステータスとコストを初期化して全てpopする
void NodeManager::clear(priority_queue<Node*, vector<Node*>, NodeCompare> list){
	Node* node;
	node = list.top();
	while (!list.empty()){
		list.pop();
	}
	list.push(node);
}