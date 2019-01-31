#include <stdlib.h>
#include <math.h>
#include <DxLib.h>
#include "AI.h"
void Mover::stop() {
	vx = vy = 0;
}
Player::Player() {
	radius = 10;
	speed = 1;
	/*
	ランダム出現
	x = (float)radius + GetRand(Define::WIN_W - 2 * radius);
	y = (float)radius + GetRand(Define::WIN_H - 2 * radius);
	*/
 	x = (float)GetRand(Define::WIN_W / 5);
	y = (float)GetRand(Define::WIN_H);
	color = GetColor(255, 255, 255);
	hitStatus = false;
	status = unReach;

	//自動化の為の出現位置固定
	x = (float)100;
	y = (float)100;
}
void Player::reset(){
	//元々の一部制限出現範囲
	//x = (float)GetRand(Define::WIN_W / 5);
	//y = (float)GetRand(Define::WIN_H);

	//自動化の為の出現位置固定
	x = (float)100;
	y = (float)100;

	color = GetColor(255, 255, 255);
	hitStatus = false;
	status = unReach;
}
void Player::update(Goal *goal) {
	//停止済みのときは更新しない
	if (hitStatus)
		return;
	

	static unsigned int cnt = 0;
	if (flameCnt == 0 && status == unReach)
		cnt = 0;
	if (!moveAmount->empty() && cnt < moveAmount->size()) {
		{
			x += (*moveAmount)[cnt].x;
			y += (*moveAmount)[cnt].y;
			cnt++;
		}
	}
	checkGoal(goal->x, goal->y);
}

void Player::draw() {
	DrawCircle((int)x, (int)y, radius, color, 1);
	DrawCircle((int)x, (int)y, radius / 2, GetColor(0, 0, 0), 1);
}

//接触判定
bool Player::checkHit(float human_x, float human_y) {
	if (pow(human_x - x, 2.0) + pow(human_y - y, 2.0) < pow(2 * radius, 2.0)) {
		color = GetColor(255, 0, 0);
		hitStatus = true;
		return true;
	}
	else
		return false;
}

//ゴール判定:座標が重なったらゴール
void Player::checkGoal(int goal_x, int goal_y)
{
	if (abs(goal_x - (int)x) == 0 && abs(goal_y - (int)y) == 0) {		//何故か時々座標を突き抜ける
		stop();
		hitStatus = true;
		color = GetColor(0, 255, 255);
	}
}

//playerからゴールまでの距離算出
float Player::distance(Goal goal) {
	return sqrtf(powf(x - (float)goal.x, 2.0) + powf(y - (float)goal.y, 2.0));
}


Human::Human() {
	radius = 10;
	speed = 0.5+(float)GetRand(150)/100;	//速度を1～5の範囲でランダム化
	

	/*
	ランダム出現
	x = (float)radius + GetRand(Define::WIN_W - 2 * radius);
	y = (float)radius + GetRand(Define::WIN_H - 2 * radius);
	*/
	reset();
}

//画面下に消えた後に上から再出現
void Human::reset() {
	x = (float)Define::WIN_W / 3 + GetRand(Define::WIN_W) / 3;
	y = (float)radius;
	angle = (float)(260 + GetRand(20))*Define::PI / 180;
	vx = speed * cosf(angle);
	vy = speed * sinf(angle);
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));

	//自動化用のスピード再設定
	speed = 0.5 + (float)GetRand(150) / 100;
}

void Human::update() {
	//画面下に潜った場合に画面上に戻す
	if (y - radius >= Define::WIN_H)
		reset();

	if (x - radius <= 0 || x + radius >= Define::WIN_W)
		vx = -vx;
	if (y - radius <= 0 /*|| y + radius >= Define::WIN_H*/)
		vy = -vy;
	x += vx;
	y += vy;
}
void Human::draw() {
	DrawCircle((int)x, (int)y, radius, color, TRUE);
	/* 進行ベクトルを追加表示
	//int x2 = x + 30 * cosf(angle - Define::PI);
	int x2 = x + vx * 10;
	int y2 = y + vy * 10;
	DrawLine((int)x, (int)y, x2, y2, color);
	*/
}

Goal::Goal() {
	size = Define::GRID_SIZE;

	//x = Define::WIN_W * 4 / 5+GetRand(Define::WIN_W * 1 / 5);
	//y = GetRand(Define::WIN_H);

	//自動化の為の出現位置固定
	x = (float)700;
	y = (float)500;
}

void Goal::draw() {
	//ゴール座標の周囲を四角で描画
	DrawBox(x - size / 2, y - size / 2, x + size / 2, y + size / 2, GetColor(255, 255, 255), TRUE);
}
