#include "Node.h"
#include <math.h>
using namespace std;

//�����o���p�ɒǉ�
#include <DxLib.h>


Node::Node() {
	status = None;
	parent = nullptr;
	score = g = i_Cost = 0;
}

//����R�X�g�̌v�Z
void Node::calc_hCost(Goal goal) {
	//���[�N���b�h�����̏ꍇ
	h = sqrtf((float)(pow(x - goal.x, 2.0) + pow(y - goal.y, 2.0)));

	//�}���n�b�^�������̏ꍇ
	//h = fabsf(x - goal.x) + fabsf(y - goal.y);
}

void Node::calcScore(){
	score = (g + h /* + i_Cost */);	//�e���x�v�Z�����������獇�v�R�X�g�Ɋ܂߂�
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
		//printfDx("�u���b�N���S�[���ɏd�Ȃ肻���Ȃ̂ŏC��������\n");
	}
	if (y_end > Define::WIN_H) {
		y_end = Define::WIN_H - 1;
		//printfDx("�u���b�N��y�����͂ݏo�����̂ŏC��������\n");
	}
	color = GetColor(GetRand(255), GetRand(255), GetRand(255));
}

void SquareBlock::draw() {
	DrawBox(x, y, x_end, y_end, color, TRUE);
}

void SquareBlock::giveGrid(Node** grid)
{
	_grid = grid;

	//�u���b�N���̑S�Ẵm�[�h�̃X�e�[�^�X��i���s�ɂ���
	for (int column = y; column < y_end; column++) {
		for (int row = x; row < x_end; row++)
			_grid[column][row].status = Block;
	}
}

NodeManager::NodeManager() {}

void NodeManager::Initialize(Player *_player, Goal goal, Human *_human) {
	//�ʃN���X�̎��@�Ɛl���g���ׂɃ|�C���^���󂯎��
	human = _human;
	player = _player;

	//Node�̗̈�𓮓I����
	for (int column = 0; column < Define::WIN_H; column++) {
		grid[column] = new Node[Define::WIN_W];
	}
	//���z�X�^�[�g�m�[�h
	Node _start;
	_start.x = (int)player->x;	_start.y = (int)player->y;

	//�O���b�h�ɍ��W���w�肵�S�[���n�_�܂ł̋������v�Z
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calc_hCost(goal);
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
	return (x_diff == 0 || y_diff == 0) ? (float)1 : sqrtf(2.0);
}

eResult NodeManager::search(Node* center) {
	float ng;				//�q�m�[�h�̍��v�R�X�g���
	int loop_cnt = 0;		//�T���̃��[�v��
	int limit = 40 * 40;	//���[�v�񐔂̏���i�v�����j

	while (!openList.empty()) {
		//�I�[�v�����X�gtop�ɃS�[�����W������ꍇ�͐���
		if (openList.top()->x == goal_x && openList.top()->y == goal_y) {
			printfDx("�T�����������܂���\n");
			root.clear();
			getPath(openList.top());
			return arrival;
		}

		//�T���񐔂�����ɒB�����猋�ʂ�ۑ����Ē��f
		if (loop_cnt > limit) {
			getPath(openList.top());
			clearList();
			return unReach;
		}

		//�e�m�[�h���I�[�v�����X�g����pop
		openList.top()->status = Close;
		openList.pop();

		//���ӂ�8���q�m�[�h�Ƃ��ăI�[�v�����X�g�ɓ����
		for (int diff_row = -1; diff_row <= 1; diff_row++) {
			for (int diff_column = -1; diff_column <= 1; diff_column++) {
				//�����m�[�h�͏������珜�O����
				if (!(diff_row == 0 && diff_column == 0)) {
					//�q�m�[�h�̍��W������
					Node* child = &grid[center->y + diff_column][center->x + diff_row];

					//�q�m�[�h�̍��v�R�X�g���̌v�Z
					ng = (center->score - center->h) + child->h + moveCost(diff_row, diff_column);

					//InfluenceMap�ɂ��ǉ��R�X�g���v�Z
					ng += calcIM_cost(child);

					switch (child->status) {
					case None:
						child->status = Open;
						openList.push(child);
						child->parent = center;
						child->score = ng;
						break;
					case Open:
						if (ng < child->score) {
							child->score = ng;
							child->parent = center;

							//�R�X�g���ύX���ꂽ�̂ōđ}���ɂ��ă\�[�g
							openList.pop();
							openList.push(child);
						}
						break;
					case Close:
						if (ng < child->score) {
							child->score = ng;
							child->parent = center;

							//�I�[�v�����X�g�ɖ߂�
							child->status = Open;
							openList.push(child);
						}
						break;
					}
				}
			}
		}
		center = openList.top();
		loop_cnt++;
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
	player->root = &root;
	player->itr = root.rbegin();
	return;
}

//InfluenceMap�ɂ��R�X�g�v�Z
float NodeManager::calcIM_cost(Node* node){
	//�~�`�̈�ƎO�p�`�̈�̊댯�x
	float Dci = 0, Dtr = 0;
	const float Ctr2 = tan(20);	//�O�p�`�̈�̊p�x�����߂�萔

	for (int cnt = 0; cnt < HUMAN; cnt++) {
		//�l�Ǝ��@�̑��΋����̎������̋���
		static float dx, dy;
		dx = human[cnt].x - node->x;
		dy = human[cnt].y - node->y;
		//�~�`�̈�̃R�X�g
		Dci += Imax * max(0.0, 1 - (pow(dx, 2.0) + pow(dy, 2.0))
			/ (pow((player->radius + human[cnt].radius), 2.0) + pow(Cci, 2.0)));
		//�O�p�`�̈�̃R�X�g
		static float m = 0.0, s = 0.0;
		static float vx2vy2 = pow(human[cnt].vx, 2.0) + pow(human[cnt].vy, 2.0);
		m = (human[cnt].vx*dx + human[cnt].vy*dy) / vx2vy2;
		s = (-human[cnt].vy*dx + human[cnt].vx*dy) / vx2vy2;
		Dtr += Imax * max(0, 1 - m / Dtr)*max(0, 1 - abs(s) / m * Ctr2);
	}

	return IMw*max(Dci, Dtr);
}

//���X�g���̃m�[�h�̃X�e�[�^�X�ƃR�X�g�����������đS��pop����
void NodeManager::clearList() {
	Node *playerLocation, *tmp;
	playerLocation = openList.top();
	while (!openList.empty()) {
		tmp = openList.top();
		tmp->score = tmp->g = tmp->i_Cost = 0;
		tmp->status = None;
		openList.pop();
	}
	while (!closeList.empty()) {
		tmp = closeList.top();
		tmp->score = tmp->g = tmp->i_Cost = 0;
		tmp->status = None;
		closeList.pop();
	}
	openList.push(playerLocation);
}