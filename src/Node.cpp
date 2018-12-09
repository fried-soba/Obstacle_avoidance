#include "Node.h"
#include <math.h>
using namespace std;

//書き出し用に追加
#include <DxLib.h>


Node::Node() {
	status = None;
	parent = nullptr;
}

void Node::calcDistance(int goal_x,int goal_y){
	//直線距離
	distance = sqrtf((float)(pow(x - goal_x, 2.0) + pow(y - goal_y, 2.0)));

	//x軸,y軸の差の和をとってみる
	//distance = fabsf(x - goal_x) + fabsf(y - goal_x);

	/*//x軸,y軸の差のうち、大きい方を取る
	float tx = fabsf(goal_x - x);
	float ty = fabsf(goal_y - y);
	if (tx > ty)
		distance = tx;
	distance = ty;*/
}

void Node::calcScore(){
	score = (g_Cost + distance/*+i_Cost*/);	//影響度計算が実装したら合計コストに含める
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
			_grid[cnt_y][cnt_x].s_Close();
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
		for (int cnt_x = x; cnt_x < x_end; cnt_x++) {
			_grid[cnt_y][cnt_x].s_Close();
		}
	}
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

	//画面端の座標は最初からクローズにしておく
	for (int hoge = 0; hoge < Define::WIN_W; hoge++) {
		//横方向
		grid[0][hoge].s_Close();
		grid[Define::WIN_H - 1][hoge].s_Close();
	}
	for (int fuga = 0; fuga < Define::WIN_H; fuga++) {
		//縦方向
		grid[fuga][0].s_Close();
		grid[fuga][Define::WIN_W - 1].s_Close();
	}

	goal_x = goal.x;
	goal_y = goal.y;

	//スタートノードをオープンリストへ
	openList.push(&grid[(int)player.y][(int)player.x]);
	/*
	//静的障害物の設定
	block.x = 600;
	block.x_end = 700;
	block.y = 300;
	block.y_end = 700;
	for (int y = block.y; y < block.y_end; y++){
		for (int x = block.x; x < block.x_end; x++) {
			grid[y][x].s_Close();
		}
	}
	*/

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
	closeList.push(openList.top());
	openList.pop();

	//周辺の8つを子ノードとしてオープンリストに入れる
	for (int cnt_x = -1; cnt_x < 2; cnt_x++) {
		for (int cnt_y = -1; cnt_y < 2; cnt_y++) {
			//子ノードの座標を決定
			int child_x = center->x + cnt_x;
			int child_y = center->y + cnt_y;

			//中央ノードは処理から除外する
			if (!(cnt_x == 0 && cnt_y == 0)) {
				//ステータスがNoneのノードのみ操作する
				if (grid[child_y][child_x].IsNone()) {
					grid[child_y][child_x].s_Open();					//ノードステータスをオープンに変更
					grid[child_y][child_x].parent = center;				//中央ノードポインタを親としてセット
					//grid[child_y][child_x].g_Cost = center->g_Cost + 1;
					
					if (cnt_x == 0 || cnt_y == 0)
						grid[child_y][child_x].g_Cost = center->g_Cost + 1;	//縦横の子は実コストは親に1加算
					else
						grid[child_y][child_x].g_Cost = center->g_Cost + sqrtf(2);//斜めの子は実コストは親にルート2加算
					
					grid[child_y][child_x].calcScore();
					//printfDx("(%3d,%3d)のscoreは%f\n", child_x, child_y, grid[child_y][child_x].score);
 					openList.push(&(grid[child_y][child_x]));
				}
			}
		}
	}

	closeList.push(center);	//親ノードはクローズリストへ格納
	center->s_Close();
	
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
	root.reserve(root.size());
	printfDx("経路を取得・格納しました。\n");
	return;
}

//現在のオープンリスト内のノード確認をする用
void NodeManager::output(Node *node){
	openList.pop();
	closeList.push(node);
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