#include "Node.h"
#include <math.h>
#include <DxLib.h>
using namespace std;

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
		//printfDx("ブロックがゴールに重なりそうなので修正したよ\n");
	}
	if (y_end > Define::WIN_H) {
		y_end = Define::WIN_H - 1;
		//printfDx("ブロックがy軸をはみ出したので修正したよ\n");
	}
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));
}

void SquareBlock::draw() {
	DrawBox(x, y, x_end, y_end, color, TRUE);
}