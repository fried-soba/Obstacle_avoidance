#include "Node.h"
#include <math.h>
using namespace std;


// �D��x�t���L���[�̃q�[�v�\�����ɂĔ�r���Z�q"<" �̃I�[�o�[���[�h
//score�����node�̑召��r���s��
bool operator> (const Node &node1, const Node &node2) {
	return node1.score > node2.score;
};


Node::Node() {
}

void Node::calcDistance(Goal goal){
	//distance = sqrtf(powf(x - goal.x, 2.0) + powf(y - goal.y, 2.0));
	distance = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));
}

void Node::calcScore(){
	score = (float)(g_Cost + distance/*+i_Cost*/);	//�e���x�v�Z�����������獇�v�R�X�g�Ɋ܂߂�
}


NodeManager::NodeManager(){
	
}

void NodeManager::open(Node node){
	//���R�X�g�̌v�Z
	if (node.parent != nullptr)
		node.g_Cost = node.parent->g_Cost + 1;
	else
		node.g_Cost = 0;	//�e�����Ȃ����X�^�[�g�n�_�Ȃ�R�X�g0
	//���������̌v�Z
	//�i�e���x�̌v�Z�j
	//���v�R�X�g�̌v�Z
	node.calcScore();
	openList.push(node);
}

void NodeManager::close(Node node){
	openList.pop();
	closeList.push(node);
}

void NodeManager::clear(priority_queue<Node, vector<Node>, greater<Node>> list){
	while (!list.empty()){
		list.pop();
	}
}
