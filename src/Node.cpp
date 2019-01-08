#include "Node.h"
#include <math.h>
using namespace std;

//書き出し用に追加
#include <DxLib.h>


Node::Node() {
	status = None;
	parent = nullptr;
	score = g = i_Cost = 0;
}

//推定コストの計算
void Node::calc_hCost(Goal goal) {
	//ユークリッド距離の場合
	h = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));

	//マンハッタン距離の場合
	//h = fabsf(x - goal.x) + fabsf(y - goal.y);
}

void Node::calcScore(){
	score = (g + h /* + i_Cost */);	//影響度計算が実装したら合計コストに含める
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

	//ブロック内の全てのノードのステータスを進入不可にする
	for (int column = y; column < y_end; column++) {
		for (int row = x; row < x_end; row++)
			_grid[column][row].status = Block;
	}
}

NodeManager::NodeManager() {}

void NodeManager::Initialize(Player player, Goal goal) {
	//Nodeの領域を動的生成
	for (int column = 0; column < Define::WIN_H; column++) {
		grid[column] = new Node[Define::WIN_W];
	}

	//仮想スタートノード
	Node _start;
	_start.x = (int)player.x;	_start.y = (int)player.y;

	//グリッドに座標を指定しゴール地点までの距離を計算
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calc_hCost(goal);
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
	return (x_diff == 0 || y_diff == 0) ? (float)1 : sqrtf(2.0);
}

eResult NodeManager::search(Node* center) {
	float ng;	//子ノードの合計コスト候補

	//（ここで影響度の計算）


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
		for (int diff_row = -1; diff_row <= 1; diff_row++) {
			for (int diff_column = -1; diff_column <= 1; diff_column++) {
				//中央ノードは処理から除外する
				if (!(diff_row == 0 && diff_column == 0)) {
					//子ノードの座標を決定
					Node* child = &grid[center->y + diff_column][center->x + diff_row];

					//子ノードの合計コスト候補の計算
					ng = (center->score - center->h) + child->h + moveCost(diff_row, diff_column);

					switch (child->status) {
					case None:
						child->status = Open;
						openList.push(child);
						child->parent = center;
						child->score = ng;
						break;
					case Open:
						if (ng < child->score) {
							child->score = ng;
							child->parent = center;

							//コストが変更されたのでオープンリスト内を再ソート
							openList.pop();
							openList.push(child);
						}
						break;
					case Close:
						if (ng < child->score) {
							child->score = ng;
							child->parent = center;

							//オープンリストに戻す
							child->status = Open;
							openList.push(child);
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