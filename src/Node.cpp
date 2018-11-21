#include "Node.h"
#include <math.h>
using namespace std;


// �D��x�t���L���[�̃q�[�v�\�����ɂĔ�r���Z�q"<" �̃I�[�o�[���[�h
//score�����node�̑召��r���s��
bool operator> (const Node &node1, const Node &node2) {
	return node1.score > node2.score;
};


Node::Node() {
	status = None;
	parent = nullptr;
}

void Node::calcDistance(Goal goal){
	distance = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));
}

void Node::calcScore(){
	score = (float)(g_Cost + distance/*+i_Cost*/);	//�e���x�v�Z�����������獇�v�R�X�g�Ɋ܂߂�
}

void Node::s_None(){
	status = None;
}

void Node::s_Open(){
	status = Open;
}

void Node::s_Close(){
	status = Close;
}

bool Node::IsNone(){
	if (status == None)
		return true;
	else
		return false;
}


NodeManager::NodeManager(){
	
}

Node NodeManager::search(Node node){
	//���R�X�g�̌v�Z
	if (node.parent != nullptr)
		node.g_Cost = node.parent->g_Cost + 1;
	else
		node.g_Cost = 0;	//�e�����Ȃ����X�^�[�g�n�_�Ȃ̂Ŏ��R�X�g0


	//�i�e���x�̌v�Z�j
	
	//���v�R�X�g�̌v�Z
	node.calcScore();

	//���ӂ�8���q�m�[�h�Ƃ��ăI�[�v�����X�g�ɓ����
	extern Node **_grid;											//�������ʃN���X����_grid�ƃ����N����Ȃ�
	for (int cnt_x = -1; cnt_x < 2; cnt_x++) {
		for (int cnt_y = -1; cnt_y < 2; cnt_y++) {
			if (cnt_x != 0 && cnt_y != 0) {
				//���I�[�v���̃m�[�h�̂ݑ��삷��
				if (_grid[node.y + cnt_y][node.x + cnt_x].IsNone()) {
					_grid[node.y + cnt_y][node.x + cnt_x].parent = &node;		//�����̃m�[�h��e�m�[�h�Ƃ��ăZ�b�g
					_grid[node.y + cnt_y][node.x + cnt_x].s_Open();				//�m�[�h�X�e�[�^�X���I�[�v���ɕύX
					openList.push(_grid[node.y + cnt_y][node.x + cnt_x]);
				}
			}
		}
	}


	closeList.push(node);	//�e�m�[�h�̓N���[�Y���X�g�֊i�[
	node.s_Close();
	return openList.top();	//�I�[�v�����X�g���ō��v�R�X�g�̍ŏ��m�[�h��Ԃ�
	//�ċA�I�ɏ����΃S�[���n�_�ɂ��ǂ蒅���܂ŒT���𑱂�����
}

//���݂̃I�[�v�����X�g���̃m�[�h�m�F������p
void NodeManager::output(Node node){
	openList.pop();
	closeList.push(node);
}

void NodeManager::clear(priority_queue<Node, vector<Node>, greater<Node>> list){
	while (!list.empty()){
		list.top().s_None();
		list.pop();
	}
}
