#pragma once

#include "AI.h"
#include "Define.h"
#include <queue>
#define BLOCKS 11      	//�K���ɔz�u����ÓI��Q���̐�

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
	int x, y;			//�m�[�h���W
	float g_Cost;			//���R�X�g
	float suppositionScore;		//���zf(n)
	float fromStartDistance;	//�X�^�[�g�n�_����̒��������i ���zg(n) �j
	float toGoalDistance;		//����R�X�g����1�F�S�[���n�_�܂ł̒��������i ���zh(n) �j
	int i_Cost;			//����R�X�g����2�FInfluenceMap�Ōv�Z����
	float score;			//�R�X�g���v�l

	Node *parent;			//�e�m�[�h�̃|�C���^
	void calcDistance(Node start, Goal goal);
	void calcScore();		//���v�X�R�A���v�Z
};

struct NodeCompare {
	bool operator()(const Node* a, const Node* b) const {
		return a->toGoalDistance > b->toGoalDistance;
	}
};

class SquareBlock {
	int x, x_end;
	int y,  y_end;
	int thickness,length;
	unsigned int color;
public:
	SquareBlock();
	void draw();
	void giveGrid(Node** grid);
	Node **_grid;
};

class NodeManager{
public:
	NodeManager();
	void Initialize(Player player, Goal goal);
	~NodeManager() = default;
	int goal_x, goal_y;
	int get_goal = false;
	Node search(Node *node);
	void getPath(Node *goal);
	void output(Node *node);
	void clear(priority_queue<Node*, vector<Node*>, NodeCompare> list);
//private:
	priority_queue<Node*, vector<Node*>, NodeCompare> openList;	//�m�[�h�̃|�C���^���i�[����D��x�t���L���[�A�\�[�g�͏���
	vector<Point> root;														//�T���o�H���i�[����z��
	Node **grid = new Node*[Define::WIN_H];
	SquareBlock block[BLOCKS];
};