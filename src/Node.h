#pragma once

#include "AI.h"
#include "Define.h"
#include <queue>
#define BLOCKS 4      	//�K���ɔz�u����ÓI��Q���̐�

enum eStatus {
	None,
	Open,
	Close,
	Block
};

enum eResult {
	unReach,
	notFound,
	arrival
};

//�eGrid(=����1�̐����`�̃m�[�h)�̃X�e�[�^�X
class Node {
public:
	Node();
	~Node() = default;
	eStatus status;
	int x, y;			//�m�[�h���W
	float g_Cost;			//���R�X�g
	float fromStartDistance;	//�X�^�[�g�n�_����̒��������i ���zg(n) �j
	float toGoalDistance;		//����R�X�g����1�F�S�[���n�_�܂ł̒��������i ���zh(n) �j
	float suppositionScore;		//�m�[�h���o�R����S-G�Ԃ̋����i���zf(n)�j
	int i_Cost;			//����R�X�g����2�FInfluenceMap�Ōv�Z����
	float score;			//�R�X�g���v�l

	Node *parent;			//�e�m�[�h�̃|�C���^
	void calcDistance(Node start, Goal goal);
	void calcScore();		//���v�X�R�A���v�Z
};

//�I�[�v�����X�g�̗D��x�t���L���[�ɗp����\�[�g�
struct NodeCompare {
	bool operator()(const Node* a, const Node* b) const {
		return a->suppositionScore > b->suppositionScore;
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
	eResult search(Node* node);
	void getPath(Node *goal);
	void clear(priority_queue<Node*, vector<Node*>, NodeCompare> list);
	priority_queue<Node*, vector<Node*>, NodeCompare> openList;	//�m�[�h�̃|�C���^���i�[����D��x�t���L���[�A�\�[�g�͏���
	vector<Point> root;														//�T���o�H���i�[����z��
	Node **grid = new Node*[Define::WIN_H];
	SquareBlock block[BLOCKS];
};