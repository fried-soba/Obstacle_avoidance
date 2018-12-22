#pragma once

#include "AI.h"
#include "Define.h"
#include <queue>
#define BLOCKS 10      	//�K���ɔz�u����ÓI��Q���̐�



//�eGrid(=����1�̐����`�̃m�[�h)�̃X�e�[�^�X
class Node {
public:
	Node();
	~Node() = default;
	enum eStatus{
		None,
		Open,
		Close
	};
	eStatus status;
	int x, y;			//�m�[�h���W
	float g_Cost;			//���R�X�g
	float distance;		//����R�X�g����1�F�S�[���n�_�܂ł̒�������
	int i_Cost;			//����R�X�g����2�FInfluenceMap�Ōv�Z����
	float score;			//�R�X�g���v�l

	Node *parent;			//�e�m�[�h�̃|�C���^
	void calcDistance(int goal_x,int goal_y);
	void calcScore();		//���v�X�R�A���v�Z
	void s_None();
	void s_Open();
	void s_Close();
	bool IsNone();
};

struct NodeCompare {
	bool operator()(const Node* a, const Node* b) const {
		return a->distance > b->distance;
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
	priority_queue<Node*, vector<Node*>, NodeCompare> openList, closeList;	//�m�[�h�̃|�C���^���i�[����D��x�t���L���[�A�\�[�g�͏���
	vector<Point> root;														//�T���o�H���i�[����z��
	Node **grid = new Node*[Define::WIN_H];
	SquareBlock block[BLOCKS];
};