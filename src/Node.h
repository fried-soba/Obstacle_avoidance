#pragma once

#include "AI.h"
#include "Define.h"
#include <queue>

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
	int x, y;					//�m�[�h���W
	float g;					//���R�X�g
	float h;					//����R�X�g
	float i_Cost;				//����R�X�g����2�FInfluenceMap�Ōv�Z����
	float score;				//�R�X�g���v�l f

	Node *parent;			//�e�m�[�h�̃|�C���^
	void calc_hCost(Goal goal);
	void calcScore();		//���v�X�R�A���v�Z
};

//�I�[�v�����X�g�̗D��x�t���L���[�ɗp����\�[�g�
struct NodeCompare {
	bool operator()(const Node* a, const Node* b) const {
		return a->score > b->score;
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
	~NodeManager() = default;
	int goal_x, goal_y;
	int get_goal = false;
	void Initialize(Player *player, Goal goal,Human *human);
	float moveCost(int x_diff, int y_diff);
	eResult search(Node* node);
	void getPath(Node *goal);
	void clearList();
	float calcIM_cost(Node* node);

	priority_queue<Node*, vector<Node*>, NodeCompare> openList, closeList;	//�m�[�h�̃|�C���^���i�[����D��x�t���L���[�A�\�[�g�͏���
	vector<Point> root;														//�T���o�H���i�[����z��
	Node **grid = new Node*[Define::WIN_H];
	Human *human;
	Player *player;
	SquareBlock block[BLOCKS];
};