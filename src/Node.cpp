#include "Node.h"
#include <math.h>
#include <DxLib.h>
using namespace std;

Node::Node() {
	status = None;
	parent = nullptr;
	score = g = i_Cost = 0;
}

//����R�X�g�̌v�Z
void Node::calc_hCost(Goal goal) {
	//���[�N���b�h�����̏ꍇ
	h = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));

	//�}���n�b�^�������̏ꍇ
	//h = fabsf(x - goal.x) + fabsf(y - goal.y);
}

void Node::calcScore(){
	score = (g + h /* + i_Cost */);	//�e���x�v�Z�����������獇�v�R�X�g�Ɋ܂߂�
}

SquareBlock::SquareBlock() {
	x = Define::WIN_W * 1 / 5 + GetRand(Define::WIN_W * 2 / 5);		//�X�^�[�g-�S�[���Ԃ̂ǂ���
	y = 100 + GetRand(Define::WIN_H - 200);							//��[100�͋󂭂悤�ɂ���
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
		//printfDx("�u���b�N���S�[���ɏd�Ȃ肻���Ȃ̂ŏC��������\n");
	}
	if (y_end > Define::WIN_H) {
		y_end = Define::WIN_H - 1;
		//printfDx("�u���b�N��y�����͂ݏo�����̂ŏC��������\n");
	}
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));
}

void SquareBlock::draw() {
	DrawBox(x, y, x_end, y_end, color, TRUE);
}