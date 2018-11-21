#pragma once

#include "AI.h"
#include <queue>
using namespace std;

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
	int x, y;
	int g_Cost;			//���R�X�g
	float distance;		//����R�X�g����1�F�S�[���n�_�܂ł̒�������
	int i_Cost;			//����R�X�g����2�FInfluenceMap�Ōv�Z����
	float score;			//�R�X�g���v�l

	Node *parent;			//�e�m�[�h�̃|�C���^
	void calcDistance(Goal goal);
	void calcScore();		//���v�X�R�A���v�Z
	void s_None();
	void s_Open();
	void s_Close();
	bool IsNone();
};

class NodeManager{
public:
	NodeManager();
	~NodeManager() = default;
	
	Node search(Node node);
	void output(Node node);
	void clear(priority_queue<Node, vector<Node>, greater<Node>> list);
//private:
	priority_queue<Node, vector<Node>, greater<Node>> openList, closeList;	//�m�[�h���i�[����D��x�t���L���[�A�\�[�g�͏���
};