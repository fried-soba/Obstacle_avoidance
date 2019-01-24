#pragma once

#include "AI.h"
#include "Define.h"

enum eStatus {
	None,
	Open,
	Close,
	Block
};

//�eGrid(=����1�̐����`�̃m�[�h)�̃X�e�[�^�X
class Node {
public:
	Node();
	~Node() = default;
	eStatus status;
	int x, y;					//�m�[�h���W
	float g;					//���R�X�g
	float h;					//����R�X�g
	float i_Cost;				//����R�X�g����2�FInfluenceMap�Ōv�Z����
	float score;				//�R�X�g���v�l f

	Node *parent;			//�e�m�[�h�̃|�C���^
	void calc_hCost(Goal goal);
	void calcScore();		//���v�X�R�A���v�Z
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