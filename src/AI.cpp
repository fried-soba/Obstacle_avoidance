#include <stdlib.h>
#include <math.h>
#include <DxLib.h>
#include "Define.h"
#include "AI.h"
void Mover::stop() {
	vx = vy = 0;
}
Player::Player() {
	radius = 20;
	speed = 1;
	/*
	�����_���o��
	x = (float)radius + GetRand(Define::WIN_W - 2 * radius);
	y = (float)radius + GetRand(Define::WIN_H - 2 * radius);
	*/
 	x = (float)GetRand(Define::WIN_W / 5);
	y = (float)GetRand(Define::WIN_H);
	angle = (float)(330 + GetRand(30))*Define::PI / 180;
	vx = (float)speed * cosf(angle);
	vy = (float)speed * sinf(angle);
	color = GetColor(255, 255, 255);
	hitStatus = false;

	//moveAmount = new vector<Point>;
}
void Player::reset(){
	x = (float)GetRand(Define::WIN_W / 5);
	y = (float)GetRand(Define::WIN_H);
	angle = (float)(330 + GetRand(30))*Define::PI / 180;
	vx = speed * cosf(angle);
	vy = speed * sinf(angle);
	color = GetColor(255, 255, 255);
	hitStatus = false;
}
void Player::update(Goal *goal) {
	//��~�ς݂̂Ƃ��͍X�V���Ȃ�
	if (hitStatus)
		return;
	checkGoal(goal->x, goal->y);

	static int cnt = 0;
	if (flameCnt == 0)
		cnt = 0;
	if (!moveAmount->empty() && cnt < moveAmount->size()) {
		{
			x += (*moveAmount)[cnt].x;
			y += (*moveAmount)[cnt].y;
			cnt++;
		}
	}
}

void Player::draw() {
	DrawCircle((int)x, (int)y, radius, color, 1);
	DrawCircle((int)x, (int)y, radius / 2, GetColor(0, 0, 0), 1);
}

//�ڐG����
bool Player::checkHit(float human_x, float human_y) {
	if (pow(human_x - x, 2.0) + pow(human_y - y, 2.0) < pow(2 * radius, 2.0)) {
		color = GetColor(255, 0, 0);
		hitStatus = true;
		return true;
	}
	else
		return false;
}

//�S�[������:���W���d�Ȃ�����S�[��
void Player::checkGoal(int goal_x, int goal_y)
{
	if (abs(goal_x - (int)x) == 0 && abs(goal_y - (int)y) == 0) {
		stop();
		hitStatus = true;
		color = GetColor(0, 255, 255);
	}
}

//player����S�[���܂ł̋����Z�o
float Player::distance(Goal goal) {
	float distance;
	distance = sqrtf(powf(x - (float)goal.x, 2.0) + powf(y - (float)goal.y, 2.0));
	return distance;
}


Human::Human() {
	radius = 20;
	speed = 1+(float)GetRand(400)/100;	//���x��1�`5�͈̔͂Ń����_����
	/*
	�����_���o��
	x = (float)radius + GetRand(Define::WIN_W - 2 * radius);
	y = (float)radius + GetRand(Define::WIN_H - 2 * radius);
	*/
	reset();
}

//��ʉ��ɏ�������ɏォ��ďo��
void Human::reset() {
	x = (float)Define::WIN_W / 3 + GetRand(Define::WIN_W) / 3;
	y = (float)radius;
	angle = (float)(260 + GetRand(20))*Define::PI / 180;
	vx = speed * cosf(angle);
	vy = speed * sinf(angle);
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));
}

void Human::update() {
	//��ʉ��ɐ������ꍇ�ɉ�ʏ�ɖ߂�
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
	DrawCircle((int)x, (int)y, radius, color, 1);
}

Goal::Goal() {
	size = Define::GRID_SIZE;
	x = Define::WIN_W * 4 / 5+GetRand(Define::WIN_W * 1 / 5);
	y = GetRand(Define::WIN_H);
}

void Goal::draw() {
	//�S�[�����W�̎��͂��l�p�ŕ`��
	DrawBox(x - size / 2, y - size / 2, x + size / 2, y + size / 2, GetColor(255, 255, 255), TRUE);
}
