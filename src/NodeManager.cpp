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
}

//�אڃm�[�h�̈ړ��R�X�g
float NodeManager::moveCost(int x_diff, int y_diff) {
	//�c���̃m�[�h�͋���1�A�΂߂Ȃ烋�[�g2
	return (x_diff == 0 || y_diff == 0) ? (float)1 : sqrtf(2.0);
}

eResult NodeManager::search() {
	float ng;												//�q�m�[�h�̍��v�R�X�g���
	int loop_cnt = 0;										//�T���̃��[�v��
	int limit = 40 * 40;									//���[�v�񐔂̏���i�v�����j
	Node* center = &grid[(int)player.y][(int)player.x];		//�T���J�n���_�̐e�m�[�h
	openList.push(center);

	while (!openList.empty()) {
		//�I�[�v�����X�gtop�ɃS�[�����W������ꍇ�͐���
		if (openList.top()->x == goal.x && openList.top()->y == goal.y) {
			printfDx("�T�����������܂���\n");
			root.clear();
			getPath(*openList.top());
			return arrival;
		}

		//�T���񐔂�����ɒB�����猋�ʂ�ۑ����Ē��f
		if (loop_cnt > limit) {
			getPath(*openList.top());
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
							//�D��x�t���L���[�̓q�[�v�\�[�g�ł��邽���Y��Ƀ\�[�g����ĂȂ��F�v�����ۑ�
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

void NodeManager::getPath(Node _goal) {
	while (_goal.parent != nullptr) {
		Point _tmp;
		_tmp.x = _goal.x;
		_tmp.y = _goal.y;
		root.push_back(_tmp);
		_goal = *_goal.parent;
	}
	printfDx("�o�H���擾�E�i�[���܂����B\n");
	
	//�������t���ɓ������܂܂Ȃ̖߂�
	Point _tmp;
	for (int cnt = 0; cnt < root.size() / 2; cnt++) {
		_tmp = root[cnt];
		root[cnt] = root[root.size() - cnt - 1];
		root[root.size() - cnt - 1] = _tmp;
	}
	//�q�m�[�h�ֈڂ�ړ��ʂ�e�ɋL�^����
	for (int cnt = 0; cnt < root.size() - 2; cnt++) {
		_tmp.x= root[cnt + 1].x - root[cnt].x;
		_tmp.y= root[cnt + 1].y - root[cnt].y;
		player.moveAmount.push_back(_tmp);
	}
	return;
}

//InfluenceMap�ɂ��R�X�g�v�Z
float NodeManager::calcIM_cost(Node* node) {
	//�~�`�̈�ƎO�p�`�̈�̊댯�x
	float Dci = 0, Dtr = 0;
	const float Ctr2 = tanf(20);	//�O�p�`�̈�̊p�x�����߂�萔

	for (int cnt = 0; cnt < HUMAN; cnt++) {
		//�l�Ǝ��@�̑��΋����̎������̋���
		static float dx, dy;
		dx = human[cnt].x - node->x;
		dy = human[cnt].y - node->y;
		//�~�`�̈�̃R�X�g
		Dci += Imax * max(0.0, 1 - (pow(dx, 2.0) + pow(dy, 2.0))
			/ (pow((player.radius + human[cnt].radius), 2.0) + pow(Cci, 2.0)));
		//�O�p�`�̈�̃R�X�g
		static float m = 0.0, s = 0.0;
		static float vx2vy2 = powf(human[cnt].vx, 2.0) + powf(human[cnt].vy, 2.0);
		m = (human[cnt].vx*dx + human[cnt].vy*dy) / vx2vy2;
		s = (-human[cnt].vy*dx + human[cnt].vx*dy) / vx2vy2;
		Dtr += Imax * max(0, 1 - m / Dtr)*max(0, 1 - abs(s) / m * Ctr2);
	}

	return (float)IMw*max(Dci, Dtr);
}

void NodeManager::update() {
	//�t���[�������^�C�~���O�̎󂯓n��
	flame_cnt++;
	flame_cnt %= FREQUENCY;
	player.flameCnt = flame_cnt;
	
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
	for (unsigned int cnt = 0; cnt < root.size(); cnt++) {
		int x = root[cnt].x;
		int y = root[cnt].y;
		DrawBox(x, y, ++x, ++y, GetColor(255, 255, 255), FALSE);
	}
	//�S�[���܂ł̋����ƌ��ݒn��\��
	DrawFormatString(100, 80, GetColor(255, 255, 255), "�����F%.1f x:%.0f y:%.0f", player.distance(goal), player.x, player.y);
}

//���X�g���̃m�[�h�̃X�e�[�^�X�ƃR�X�g�����������đS��pop����
void NodeManager::clearList() {
	Node *tmp;
	while (!openList.empty()) {
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