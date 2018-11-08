#include <stdlib.h>
#include <math.h>
#include <DxLib.h>
#include "Define.h"
#include "AI.h"

void Mover::stop() {
	xSpeed = ySpeed = 0;
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
	xSpeed = (float)speed * cosf(angle);
	ySpeed = (float)speed * sinf(angle);
	color = GetColor(255, 255, 255);
	hitStatus = false;
}
void Player::update(Goal *goal) {
	//��~�ς݂̂Ƃ��͍X�V���Ȃ�
	if (hitStatus)
		return;
	checkGoal((*goal).x, (*goal).y);
	float degree = 180/Define::PI*atanf(((*goal).y - y) / ((*goal).x - x));
	if (degree > 90)
		degree += 180;
	
	angle = degree*Define::PI / 180;
	/*
	if (x - radius <= 0 || x + radius >= Define::WIN_W)
		xSpeed = -xSpeed;
	if (y - radius <= 0 || y + radius >= Define::WIN_H)
		ySpeed = -ySpeed;
	*/
	xSpeed = (float)speed * cosf(angle);
	ySpeed = (float)speed * sinf(angle);
	x += xSpeed;
	y += ySpeed;

}
void Player::draw() {
	DrawCircle((int)x, (int)y, radius, color, 1);
	DrawCircle((int)x, (int)y, radius / 2, GetColor(0, 0, 0), 1);
	//printfDx("%6.1lf,%6.1lf", x, y);
	//DrawFormatString(100, 0, GetColor(255, 255, 255), "%5.2lf,%5.2lf", xSpeed, ySpeed);
	//DrawFormatString(100, 100, GetColor(255, 255, 255), "%6.1lf,%6.1lf", x, y);
}

//�ڐG����
bool Player::checkHit(float human_x, float human_y) {
	//if(human_x*human_x+human_y*human_y < (2 * radius)*(2 * radius))
	if (pow(human_x - x, 2.0) + pow(human_y - y, 2.0) < pow(2 * radius, 2.0)) {
		color = GetColor(255, 0, 0);
		hitStatus = true;
		return true;
	}
	else
		return false;
	
}

//�S�[������:���W���قڏd�Ȃ�����S�[��
void Player::checkGoal(int goal_x, int goal_y)
{
	if (abs(goal_x - (int)x) < 2 && abs(goal_y - (int)y) < 2) {
		stop();
		hitStatus = true;
		color = GetColor(0, 255, 255);
	}
}

//����R�X�g�Z�o�Ɏg�p
int Player::distance(Goal * goal)
{
	float distance;
	distance = sqrt(pow(x - (float)(*goal).x, 2.0) + pow(y - (float)(*goal).y, 2.0));
	return (int)distance;
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
	xSpeed = (float)speed * cosf(angle);
	ySpeed = (float)speed * sinf(angle);
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));
	outside = false;
}

void Human::update() {
	//��ʉ��ɐ������ꍇ�ɏ�������
	if (y - radius >= Define::WIN_H)
		outside = true;

	if (x - radius <= 0 || x + radius >= Define::WIN_W)
		xSpeed = -xSpeed;
	if (y - radius <= 0 /*|| y + radius >= Define::WIN_H*/)
		ySpeed = -ySpeed;
	x += xSpeed;
	y += ySpeed;
}
void Human::draw() {
	DrawCircle((int)x, (int)y, radius, color, 1);
	//���x,���W�`��p
	//DrawFormatString(100, 0, GetColor(255, 255, 255), "%5.2lf,%5.2lf", xSpeed, ySpeed);
	//DrawFormatString(100, 100, GetColor(255, 255, 255), "%6.1lf,%6.1lf", x, y);
}

void Human::exorbitant(){
	outside = true;
}

Grid::Grid() { 
	status = false;
}
Grid::Grid(int x,int y) {
	//x=
	parent = nullptr;
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