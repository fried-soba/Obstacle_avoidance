#include "Node.h"
#include <math.h>
using namespace std;

//書き出し用に追加
#include <DxLib.h>


Node::Node() {
	status = None;
	parent = nullptr;
	g_Cost = i_Cost = 0;
	score = 0;
}

void Node::calcDistance(Node start, Goal goal) {
	//直線距離でf*(n)=g*(h)+h*(n)を計算
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
	y = 100 + GetRand(Define::WIN_H - 200);							//上端100は空くようにする
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
	if (x_end > Define::WIN_W * 4 / 5) {
		x_end = Define::WIN_W * 4 / 5 - 1;
		printfDx("ブロックがゴールに重なりそうなので修正したよ\n");
	}
	if (y_end > Define::WIN_H) {
		y_end = Define::WIN_H - 1;
		printfDx("ブロックがy軸をはみ出したので修正したよ\n");
	}
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));
}

void SquareBlock::draw() {
	DrawBox(x, y, x_end, y_end, color, TRUE);
}

void SquareBlock::giveGrid(Node** grid)
{
	_grid = grid;
	for (int cnt_y = y; cnt_y < y_end; cnt_y++) {
		for (int cnt_x = x; cnt_x < x_end; cnt_x++)
			_grid[cnt_y][cnt_x].status = Block;
	}
}

NodeManager::NodeManager() {}

void NodeManager::Initialize(Player player, Goal goal) {
	//Nodeの領域を動的生成
	for (int cnt = 0; cnt < Define::WIN_H; cnt++) {
		grid[cnt] = new Node[Define::WIN_W];
	}

	//仮想スタートノード
	Node _start;
	_start.x = (int)player.x;	_start.y = (int)player.y;

	//グリッドに座標を指定しゴール地点までの距離を計算
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calcDistance(_start, goal);
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
	openList.push(&grid[_start.y][_start.x]);
}

//隣接ノードの移動コスト
float NodeManager::moveCost(int x_diff, int y_diff) {
	//縦横のノードは距離1、斜めならルート2
	return (x_diff == 0 || y_diff == 0) ? 1.0 : sqrtf(2.0);
}

eResult NodeManager::search(Node* center) {
	//実コストの計算
	if (center->parent == nullptr) {
		center->g_Cost = 0;		//親がいない＝スタート地点なので実コスト0
		center->calcScore();
	}

	//（ここに影響度の計算）


	while (!openList.empty()) {
		//オープンリストtopにゴール座標が来る場合は成功
		if (openList.top()->x == goal_x && openList.top()->y == goal_y) {
			printfDx("探索が完了しました\n");
			getPath(openList.top());
			return arrival;
		}

		//親ノードをオープンリストからpop
		openList.top()->status = Close;
		openList.pop();

		//周辺の8つを子ノードとしてオープンリストに入れる
		for (int cnt_x = -1; cnt_x <= 1; cnt_x++) {
			for (int cnt_y = -1; cnt_y <= 1; cnt_y++) {
				//中央ノードは処理から除外する
				if (!(cnt_x == 0 && cnt_y == 0)) {
					//子ノードの座標を決定
					Node* child = &grid[center->y + cnt_y][center->x + cnt_x];
					child->g_Cost = (center->score - center->toGoalDistance) + child->toGoalDistance + moveCost(cnt_x, cnt_y);

					switch (child->status) {
					case None:
						child->status = Open;
						openList.push(child);
						child->parent = center;
						child->score = child->g_Cost;
						//child->g_Cost = center->g_Cost + moveCost(cnt_x, cnt_y);
						break;
					case Open:
						if (child->g_Cost < child->score) {
							child->score = child->g_Cost;
							child->parent = center;
						}
						break;
					case Close:
						if (child->g_Cost < child->score) {
							child->score = child->g_Cost;
							child->status = Open;
							child->parent = center;
						}
						break;
					}
				}
			}
		}
		center = openList.top();
	}
	printfDx("ルートが見つかりませんでした\n");
	return notFound;
}

void NodeManager::getPath(Node* _goal) {
	while (_goal->parent != nullptr) {
		Point _tmp;
		_tmp.x = _goal->x;
		_tmp.y = _goal->y;
		root.push_back(_tmp);
		_goal = _goal->parent;
	}
	printfDx("経路を取得・格納しました。\n");
	return;
}

//リスト中のノードのステータスとコストを初期化して全てpopする
void NodeManager::clear(priority_queue<Node*, vector<Node*>, NodeCompare> list) {
	Node* node;
	node = list.top();
	while (!list.empty()) {
		list.pop();
	}
	list.push(node);
}