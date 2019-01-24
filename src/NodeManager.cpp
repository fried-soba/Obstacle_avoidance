#include "NodeManager.h"
#include <DxLib.h>

NodeManager::NodeManager() {
	flame_cnt = 0;

	//Node�̗̈�𓮓I����
	for (int column = 0; column < Define::WIN_H; column++) {
		grid[column] = new Node[Define::WIN_W];
	}
	//�X�^�[�g�m�[�h�ݒ�
	start = &grid[(int)player.y][(int)player.x];

	//�O���b�h�ɍ��W���w�肵�S�[���n�_�܂ł̋������v�Z
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			grid[height][width].y = height;
			grid[height][width].x = width;
			grid[height][width].calc_hCost(goal);
		}
	}

	//��ʒ[�̍��W�͍ŏ�����N���[�Y�ɂ��Ă���
	//������
	for (int hoge = 0; hoge < Define::WIN_W; hoge++) {
		//������
		grid[0][hoge].status = Block;
		grid[Define::WIN_H - 1][hoge].status = Block;
	}
	//�c����
	for (int fuga = 0; fuga < Define::WIN_H; fuga++) {
		grid[fuga][0].status = Block;
		grid[fuga][Define::WIN_W - 1].status = Block;
	}

	//�u���b�N���̑S�Ẵm�[�h�̃X�e�[�^�X��i���s�ɂ���
	for (int cnt = 0; cnt < BLOCKS; cnt++) {
		for (int column = block[cnt].y; column < block[cnt].y_end; column++) {
			for (int row = block[cnt].x; row < block[cnt].x_end; row++)
				grid[column][row].status = Block;
		}
	}

	//�X�^�[�g�m�[�h���I�[�v�����X�g��
	openList.push(start);

	//���[�g�̊i�[����z��𓮓I�m��
	root = new vector<Point>;
	player.moveAmount = new vector<Point>;
}

//�אڃm�[�h�̈ړ��R�X�g
float NodeManager::moveCost(int x_diff, int y_diff) {
	//�c���̃m�[�h�͋���1�A�΂߂Ȃ烋�[�g2
	return (x_diff == 0 || y_diff == 0) ? (float)1 : sqrtf(2.0);
}

eResult NodeManager::search() {
	float ns;												//�q�m�[�h�̍��v�R�X�g���
	int loop_cnt = 0;										//�T���̃��[�v��
	int limit = (int)(FREQUENCY * FREQUENCY*Define::PI);			//���[�v�񐔂̏��:�T�������ɔ�Ⴓ���ĎZ�o���[�g���Z���Ȃ�Ȃ��悤�ɂ���
	int child_x, child_y;									//�q�m�[�h�̍��W
	Node* center;											//�T�����̐e�m�[�h

	clearList();											//�O��T�����̃m�[�h�Ɋւ���Ԃ�S�ăN���A
	Node* start = &grid[(int)player.y][(int)player.x];		//�T���J�n���_�ł̓X�^�[�g�m�[�h
	openList.push(start);

	while (!openList.empty()) {
		//�I�[�v�����X�gtop�ɃS�[�����W������ꍇ�͐���
		if (openList.top()->x == goal.x && openList.top()->y == goal.y) {
			printfDx("�T�����������܂���\n");
			getPath(*openList.top());
			player.status = arrival;
			return arrival;
		}
		//�T���񐔂�����ɒB�����猋�ʂ�ۑ����Ē��f
		if (loop_cnt > limit) {
			getPath(*openList.top());
			//clearList();
			return unReach;
		}

		center = openList.top();	//�T�����̒��S�m�[�h

		//�e�m�[�h���I�[�v�����X�g����pop
		openList.top()->status = Close;
		closeList.push(openList.pop());

		//���ӂ�8���q�m�[�h�Ƃ��ăI�[�v�����X�g�ɓ����
		for (int diff_row = -1; diff_row <= 1; diff_row++) {
			for (int diff_column = -1; diff_column <= 1; diff_column++) {
				child_y = center->y + diff_column;
				child_x = center->x + diff_row;
				//�����m�[�h�͏������珜�O����
				if (!(diff_row == 0 && diff_column == 0) && child_x < Define::WIN_W && child_y < Define::WIN_H) {
					//�q�m�[�h�̍��W
					Node* child = &grid[center->y + diff_column][center->x + diff_row];
					//�e�m�[�h�̎��R�X�g�̊m��
					center->g = center->score - center->h;
					//InfluenceMap�ɂ��ǉ��R�X�g���v�Z
					child->i_Cost = calcIM_cost(child);

					//�q�m�[�h�̍��v�R�X�g�̌v�Z
					ns = center->g + moveCost(diff_row, diff_column) + child->h + child->i_Cost;

					switch (child->status) {
					case None:
						child->status = Open;
						child->parent = center;
						child->score = ns;
						openList.push(child);
						break;
					case Open:
						if (ns < child->score) {
							child->score = ns;
							child->parent = center;

							//�R�X�g���ύX���ꂽ�̂ōă\�[�g
							openList.sortList();
						}
						break;
					case Close:
						if (ns < child->score) {
							child->score = ns;
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

void NodeManager::getPath(Node _goal) {
	//�T���̂��тɃ��������Ċm��
	delete root;
	root = new vector<Point>;

	//�e��k���Čo�H�擾
	while (_goal.parent != nullptr) {
		Point _tmp;
		_tmp.x = _goal.x;
		_tmp.y = _goal.y;
		(*root).push_back(_tmp);
		_goal = *_goal.parent;
	}
	//printfDx("�o�H���擾�E�i�[���܂����B\n");
	
	//�������t���ɓ������܂܂Ȃ̖߂�
	Point _tmp;
	for (unsigned int cnt = 0; cnt < root->size() / 2; cnt++) {
		_tmp = (*root)[cnt];
		(*root)[cnt] = (*root)[root->size() - cnt - 1];
		(*root)[(*root).size() - cnt - 1] = _tmp;
	}

	//�O��T�����̃��[�g�����������Ɣj���������Ă���L�^����
	delete player.moveAmount;
	player.moveAmount = new vector<Point>;
	//�q�m�[�h�ֈڂ�ړ��ʂ�e�ɋL�^����
	if ((*root).size() > 2) {
		for (unsigned int cnt = 0; cnt < root->size() - 2; cnt++) {
			_tmp.x = (*root)[cnt + 1].x - (*root)[cnt].x;
			_tmp.y = (*root)[cnt + 1].y - (*root)[cnt].y;
			(*player.moveAmount).push_back(_tmp);
		}
	}
	return;
}

//InfluenceMap�ɂ��R�X�g�v�Z
float NodeManager::calcIM_cost(Node* node) {
	//�~�`�̈�ƎO�p�`�̈�̊댯�x
	float Dci = 0, Dtr = 0;
	const float Ctr2 = tanf(20 * Define::PI / 180);	//�O�p�`�̈�̊p�x�����߂�萔 �����̓��W�A���\�L

	for (int cnt = 0; cnt < HUMAN; cnt++) {
		//�l�Ǝ��@�̑��΋����̎������̋���
		static double dx, dy;
		dx = human[cnt].x - node->x;
		dy = human[cnt].y - node->y;
		//�~�`�̈�̃R�X�g
		Dci += Imax * max(0.0, 1 - (pow(dx, 2.0) + pow(dy, 2.0))
			/ (pow((player.radius + human[cnt].radius), 2.0) + pow(Cci, 2.0)));
		//�O�p�`�̈�̃R�X�g
		static double m = 0.0;	//���x�x�N�g���ɉ�������
		static double s = 0.0;	//���x�x�N�g���ɒ��s���鐬��
		static double vx2vy2 = powf(human[cnt].vx, 2.0) + powf(human[cnt].vy, 2.0);
		m = (human[cnt].vx*dx + human[cnt].vy*dy) / vx2vy2;
		s = (-human[cnt].vy*dx + human[cnt].vx*dy) / vx2vy2;
		Dtr += Imax * max(0, 1 - m / Ctr)*max(0, 1 - abs(s) / m * Ctr2);
	}
	printfDx("Dci = %.1f ,Dtr = %.1f\n", Dci, Dtr);		//IM�v�Z�R�X�g���_���v
	return (float)IMw*max(Dci, Dtr);
}

void NodeManager::update() {
	//�t���[�������^�C�~���O�̎󂯓n��
	flame_cnt++;
	flame_cnt %= FREQUENCY;
	player.flameCnt = flame_cnt;
	
	//���I�I�u�W�F�N�g�̈ʒu�X�V
	player.update(&goal);
	for (int cnt = 0; cnt < HUMAN; cnt++) {
		human[cnt].update();
		if (player.checkHit(human[cnt].x, human[cnt].y)) {
			player.stop();
			human[cnt].stop();
		}
	}
}

//�S�Ă�`��
void NodeManager::draw() {
	goal.draw();
	player.draw();
	for (int cnt = 0; cnt < HUMAN; cnt++)
		human[cnt].draw();
	for (int cnt = 0; cnt < BLOCKS; cnt++)
		block[cnt].draw();

	//�T���o�H�̃��[�g�\��
	for (unsigned int cnt = 0; cnt < root->size(); cnt++) {
		int x = (*root)[cnt].x;
		int y = (*root)[cnt].y;
		DrawBox(x, y, ++x, ++y, GetColor(255, 255, 255), FALSE);
	}
	//�S�[���܂ł̋����ƌ��ݒn��\��
	DrawFormatString(100, 80, GetColor(255, 255, 255), "�����F%.1f x:%.0f y:%.0f", player.distance(goal), player.x, player.y);
}

//���X�g���̃m�[�h��������Ԃɖ߂��ă|�b�v����
void NodeManager::clearList() {
	Node *tmp;
	while (!openList.empty()) {
		DrawBox(openList.top()->x, openList.top()->y, openList.top()->x + 1, openList.top()->y + 1, GetColor(0, 0, 255), TRUE);	//pop���Ƀ��X�g���̃m�[�h��`��
		tmp = openList.top();
		tmp->score = tmp->g = tmp->i_Cost = 0;
		tmp->status = None;
		tmp->parent = nullptr;
		openList.pop();
	}
	while (!closeList.empty()) {
		tmp = closeList.top();
		tmp->score = tmp->g = tmp->i_Cost = 0;
		tmp->status = None;
		tmp->parent = nullptr;
		closeList.pop();
	}
}