#include "Node.h"
#include <math.h>
using namespace std;

//�����o���p�ɒǉ�
#include <DxLib.h>


Node::Node() {
	status = None;
	parent = nullptr;
}

void Node::calcDistance(int goal_x,int goal_y){
	//��������
	distance = sqrtf((float)(pow(x - goal_x, 2.0) + pow(y - goal_y, 2.0)));

	//x��,y���̍��̘a���Ƃ��Ă݂�
	//distance = fabsf(x - goal_x) + fabsf(y - goal_x);

	/*//x��,y���̍��̂����A�傫���������
	float tx = fabsf(goal_x - x);
	float ty = fabsf(goal_y - y);
	if (tx > ty)
		distance = tx;
	distance = ty;*/
}

void Node::calcScore(){
	score = (g_Cost + distance/*+i_Cost*/);	//�e���x�v�Z�����������獇�v�R�X�g�Ɋ܂߂�
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

NodeManager::NodeManager() {}

void NodeManager::Initialize(Player player,Goal goal){
	//Node�̗̈�𓮓I����
	for (int cnt = 0; cnt < Define::WIN_H; cnt++) {
		grid[cnt] = new Node[Define::WIN_W];
	}

	//�O���b�h�ɍ��W���w�肵�S�[���n�_�܂ł̋������v�Z
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calcDistance(goal.x,goal.y);
		}
	}

	//��ʒ[�̍��W�͍ŏ�����N���[�Y�ɂ��Ă���
	for (int hoge = 0; hoge < Define::WIN_W; hoge++) {
		//������
		grid[0][hoge].s_Close();
		grid[Define::WIN_H - 1][hoge].s_Close();
	}
	for (int fuga = 0; fuga < Define::WIN_H; fuga++) {
		//�c����
		grid[fuga][0].s_Close();
		grid[fuga][Define::WIN_W - 1].s_Close();
	}

	goal_x = goal.x;
	goal_y = goal.y;

	//�X�^�[�g�m�[�h���I�[�v�����X�g��
	openList.push(&grid[(int)player.y][(int)player.x]);
}

Node NodeManager::search(Node* node){
	//���R�X�g�̌v�Z
	if (node->parent == nullptr) {
		node->g_Cost = 0;	//�e�����Ȃ����X�^�[�g�n�_�Ȃ̂Ŏ��R�X�g0
		node->calcScore();
	}

	//�i�����ɉe���x�̌v�Z�j
	

	//�e�m�[�h���I�[�v�����X�g����pop
	closeList.push(openList.top());
	openList.pop();

	//���ӂ�8���q�m�[�h�Ƃ��ăI�[�v�����X�g�ɓ����
	for (int cnt_x = -1; cnt_x < 2; cnt_x++) {
		for (int cnt_y = -1; cnt_y < 2; cnt_y++) {
			//�q�m�[�h�̍��W������
			int child_x = node->x + cnt_x;
			int child_y = node->y + cnt_y;

			//�����m�[�h�͏������珜�O����
			if (!(cnt_x == 0 && cnt_y == 0)) {
				//�X�e�[�^�X��None�̃m�[�h�̂ݑ��삷��
				if (grid[child_y][child_x].IsNone()) {
					grid[child_y][child_x].s_Open();					//�m�[�h�X�e�[�^�X���I�[�v���ɕύX
					grid[child_y][child_x].parent = node;				//�����̃m�[�h��e�m�[�h�Ƃ��ăZ�b�g
					//grid[child_y][child_x].g_Cost = node->g_Cost + 1;
					
					if (cnt_x == 0 || cnt_y == 0)
						grid[child_y][child_x].g_Cost = node->g_Cost + 1;	//�c���̎q�͎��R�X�g�͐e��1���Z
					else
						grid[child_y][child_x].g_Cost = node->g_Cost + sqrtf(2);//�΂߂̎q�͎��R�X�g�͐e�Ƀ��[�g2���Z
					
					grid[child_y][child_x].calcScore();
					//printfDx("(%3d,%3d)��score��%f\n", child_x, child_y, grid[child_y][child_x].score);
 					openList.push(&(grid[child_y][child_x]));
				}
			}
		}
	}

	closeList.push(node);	//�e�m�[�h�̓N���[�Y���X�g�֊i�[
	node->s_Close();
	
	static int cnt = 0;		//�ċA�ł���[����cnt�Ő�������
	cnt++;
	if (cnt>3000) {
		cnt = 0;
		return (*openList.top());
		clear(openList);	//�X�^�b�N�̈�̃I�[�o�[�t���[�΍�
	}
	if (goal_x == openList.top()->x && openList.top()->y == goal_y) {
		get_goal = true;
		printfDx("�S�[���n�_��������܂���\n");
		Node _tmp = (*openList.top());	//�I�[�v�����X�g�̗v�f�ɑ΂���const����ׂ̈̃o�b�t�@
		getPath(&_tmp);
		return (*openList.top());
	}
	//return (*openList.top());
	int top_x = openList.top()->x;
	int top_y = openList.top()->y;
	return search(&(grid[top_y][top_x]));	//�I�[�v���ς݂̒��ō��v�R�X�g���ŏ��̃m�[�h��Ԃ�
	//�ċA�I�ɏ����΃S�[���n�_�ɂ��ǂ蒅���܂ŒT���𑱂�����
}

void NodeManager::getPath(Node* _goal){
	while (_goal->parent!=nullptr) {
		Point _tmp;
		_tmp.x = _goal->x;
		_tmp.y = _goal->y;
		root.push_back(_tmp);
		return getPath(_goal->parent);
	}
	root.reserve(root.size());
	printfDx("�o�H���擾�E�i�[���܂����B\n");
	return;
}

//���݂̃I�[�v�����X�g���̃m�[�h�m�F������p
void NodeManager::output(Node *node){
	openList.pop();
	closeList.push(node);
}

//���X�g���̃m�[�h�̃X�e�[�^�X�ƃR�X�g�����������đS��pop����
void NodeManager::clear(priority_queue<Node*, vector<Node*>, NodeCompare> list){
	Node* node;
	node = list.top();
	while (!list.empty()){
		list.pop();
	}
	list.push(node);
}
