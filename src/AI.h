#pragma once
#include<vector>

using namespace std;

/*
DXライブラリを始めとするDirectXの使用環境下ではFPUコントロールレジスタが
DirectXによって単精度に変えられてしまうためdouble型は使用しない
浮動小数点を使うときはfloat型を使う
*/

//配列にして探索の完了した座標を格納する用の構造体
typedef struct coordinates {
	int x,y;
} Point;

class Goal {
public:
	int x, y;
	int size;
	Goal();
	void draw();
};

class Mover {
public:
	int radius,	//半径
		red, green, blue;
	unsigned int color;
	float x, y;		//中心座標
	float angle;	//方位角：進行方向の角度(ラジアン表記)
	float speed, vx, vy;	//移動速度
	void stop();
};

class Player: public Mover {
private:
	bool hitStatus;
	int red, green, blue;
	
public:
	Player();
	~Player() = default;
	void reset();
	void update(Goal *goal);
	void draw();
	bool checkHit(float x, float y);
	void checkGoal(int g_x, int g_y);
	float distance(Goal goal);

	int flameCnt;
	vector<Point> moveAmount;
	reverse_iterator<vector<Point>::iterator> itr;	//ルートの現在地を保持する逆イテレータ
};

class Human :public Mover {
private:
	float azimuth;	//方位角：進行方向の角度(ラジアン表記)
	bool hitStatus;
	int red, green, blue;
public:
	float x, y;
	Human();
	void reset();
	void update();
	void draw();
	//To do:人同士がぶつからないように判定する関数を作る
};


