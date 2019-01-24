#pragma once

#include "AI.h"
#include "Define.h"

enum eStatus {
	None,
	Open,
	Close,
	Block
};

//各Grid(=長さ1の正方形のノード)のステータス
class Node {
public:
	Node();
	~Node() = default;
	eStatus status;
	int x, y;					//ノード座標
	float g;					//実コスト
	float h;					//推定コスト
	float i_Cost;				//推定コストその2：InfluenceMapで計算する
	float score;				//コスト合計値 f

	Node *parent;			//親ノードのポインタ
	void calc_hCost(Goal goal);
	void calcScore();		//合計スコアを計算
};

class SquareBlock {
public:
	int x, x_end;
	int y,  y_end;
	int thickness,length;
	unsigned int color;
	SquareBlock();
	void draw();
};