#include "Node.h"
#include <math.h>
using namespace std;

//�����o���p�ɒǉ�
#include <DxLib.h>


Node::Node() {
	status = None;
	parent = nullptr;
	g_Cost = i_Cost = 0;
	score = 0;
}

void Node::calcDistance(Node start, Goal goal) {
	//����������f*(n)=g*(h)+h*(n)���v�Z
	fromStartDistance= sqrtf((float)(pow(x - start.x, 2.0) + pow(y - start.y, 2.0)));
	toGoalDistance = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));
	suppositionScore = fromStartDistance + toGoalDistance;

	//x��,y���̍��̘a���Ƃ��Ă݂�
	//toGoalDistance = fabsf(x - goal_x) + fabsf(y - goal_x);

	/*//x��,y���̍��̂����A�傫���������
	float tx = fabsf(goal_x - x);
	float ty = fabsf(goal_y - y);
	if (tx > ty)
		toGoalDistance = tx;
	toGoalDistance = ty;*/
}

void Node::calcScore(){
	score = (g_Cost + toGoalDistance/*+i_Cost*/);	//�e���x�v�Z�����������獇�v�R�X�g�Ɋ܂߂�
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
		printfDx("�u���b�N���S�[���ɏd�Ȃ肻���Ȃ̂ŏC��������\n");
	}
	if (y_end > Define::WIN_H) {
		y_end = Define::WIN_H - 1;
		printfDx("�u���b�N��y�����͂ݏo�����̂ŏC��������\n");
	}
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));
}

void SquareBlock::draw() {
	DrawBox(x, y, x_end, y_end, color, TRUE);
}

void SquareBlock::giveGrid(Node** grid)
{
	_grid = grid;
	for (int cnt_y = y; cnt_y < y_end; cnt_y++) {
		for (int cnt_x = x; cnt_x < x_end; cnt_x++)
			_grid[cnt_y][cnt_x].status = Block;
	}
}

NodeManager::NodeManager() {}

void NodeManager::Initialize(Player player, Goal goal) {
	//Node�̗̈�𓮓I����
	for (int cnt = 0; cnt < Define::WIN_H; cnt++) {
		grid[cnt] = new Node[Define::WIN_W];
	}

	//���z�X�^�[�g�m�[�h
	Node _start;
	_start.x = (int)player.x;	_start.y = (int)player.y;

	//�O���b�h�ɍ��W���w�肵�S�[���n�_�܂ł̋������v�Z
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calcDistance(_start, goal);
		}
	}

	//��ʒ[�̍��W�͍ŏ�����N���[�Y�ɂ��Ă���
	for (int hoge = 0; hoge < Define::WIN_W; hoge++) {
		//������
		grid[0][hoge].status = Close;
		grid[Define::WIN_H - 1][hoge].status = Close;
	}
	for (int fuga = 0; fuga < Define::WIN_H; fuga++) {
		//�c����
		grid[fuga][0].status = Close;
		grid[fuga][Define::WIN_W - 1].status = Close;
	}

	goal_x = goal.x;
	goal_y = goal.y;

	//�X�^�[�g�m�[�h���I�[�v�����X�g��
	openList.push(&grid[_start.y][_start.x]);
}

//�אڃm�[�h�̈ړ��R�X�g
float NodeManager::moveCost(int x_diff, int y_diff) {
	//�c���̃m�[�h�͋���1�A�΂߂Ȃ烋�[�g2
	return (x_diff == 0 || y_diff == 0) ? 1.0 : sqrtf(2.0);
}

eResult NodeManager::search(Node* center) {
	//���R�X�g�̌v�Z
	if (center->parent == nullptr) {
		center->g_Cost = 0;		//�e�����Ȃ����X�^�[�g�n�_�Ȃ̂Ŏ��R�X�g0
		center->calcScore();
	}

	//�i�����ɉe���x�̌v�Z�j


	while (!openList.empty()) {
		//�I�[�v�����X�gtop�ɃS�[�����W������ꍇ�͐���
		if (openList.top()->x == goal_x && openList.top()->y == goal_y) {
			printfDx("�T�����������܂���\n");
			getPath(openList.top());
			return arrival;
		}

		//�e�m�[�h���I�[�v�����X�g����pop
		openList.top()->status = Close;
		openList.pop();

		//���ӂ�8���q�m�[�h�Ƃ��ăI�[�v�����X�g�ɓ����
		for (int cnt_x = -1; cnt_x <= 1; cnt_x++) {
			for (int cnt_y = -1; cnt_y <= 1; cnt_y++) {
				//�����m�[�h�͏������珜�O����
				if (!(cnt_x == 0 && cnt_y == 0)) {
					//�q�m�[�h�̍��W������
					Node* child = &grid[center->y + cnt_y][center->x + cnt_x];
					child->g_Cost = (center->score - center->toGoalDistance) + child->toGoalDistance + moveCost(cnt_x, cnt_y);

					switch (child->status) {
					case None:
						child->status = Open;
						openList.push(child);
						child->parent = center;
						child->score = child->g_Cost;
						//child->g_Cost = center->g_Cost + moveCost(cnt_x, cnt_y);
						break;
					case Open:
						if (child->g_Cost < child->score) {
							child->score = child->g_Cost;
							child->parent = center;
						}
						break;
					case Close:
						if (child->g_Cost < child->score) {
							child->score = child->g_Cost;
							child->status = Open;
							child->parent = center;
						}
						break;
					}
				}
			}
		}
		center = openList.top();
	}
	printfDx("���[�g��������܂���ł���\n");
	return notFound;
}

void NodeManager::getPath(Node* _goal) {
	while (_goal->parent != nullptr) {
		Point _tmp;
		_tmp.x = _goal->x;
		_tmp.y = _goal->y;
		root.push_back(_tmp);
		_goal = _goal->parent;
	}
	printfDx("�o�H���擾�E�i�[���܂����B\n");
	return;
}

//���X�g���̃m�[�h�̃X�e�[�^�X�ƃR�X�g�����������đS��pop����
void NodeManager::clear(priority_queue<Node*, vector<Node*>, NodeCompare> list) {
	Node* node;
	node = list.top();
	while (!list.empty()) {
		list.pop();
	}
	list.push(node);
}