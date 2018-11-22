#include "TitleScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

//txt�ւ̏����o���p	//�ŏI�I�ɂ͕s�v�ɂ���
#include <stdio.h>
#include <stdlib.h>

TitleScene::TitleScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()��seed�l�������_����
	nodeMgr.Initialize(_goal);
	
	//�X�^�[�g�m�[�h�̎���9�}�X��S��open���X�g�ɓ����pop����鏇�������Ă݂���
	/*
	int baseX = (int)_player.x, baseY = (int)_player.y;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			nodeMgr.open(_grid[(int)_player.y - 1 + j][(int)_player.x - 1 + i]);
		}
	}
	printfDx("�m�[�h����%d�ł�\n", nodeMgr.openList.size());
	*/
	nodeMgr.search(nodeMgr.grid[(int)_player.y][(int)_player.x]);	//�Ƃ肠�����m�F�p�ɃX�^�[�g�m�[�h���ӂ�W�J
}

void TitleScene::update()
{
	if (CheckHitKey(KEY_INPUT_R)) {
        Parameter parameter;
		parameter.set(GameScene::ParameterTagLevel, Define::eLevel::Easy);
        const bool stackClear = false;
        _implSceneChanged->onSceneChanged(eScene::Game, parameter, stackClear);
        return;
    }
	/*
    if (CheckHitKey(KEY_INPUT_N)) {
        Parameter parameter;
        parameter.set(GameScene::ParameterTagLevel, Define::eLevel::Normal);
        const bool stackClear = false;
        _implSceneChanged->onSceneChanged(eScene::Game, parameter, stackClear);
        return;
    }
	*/

	for (int cnt = 0; cnt < NUM; cnt++) {
		_human[cnt].update();
		if (_human[cnt].outside == true) {
			_human[cnt].reset();
		}
		if (_player.checkHit(_human[cnt].x, _human[cnt].y)) {
			_player.stop();
			_human[cnt].stop();
		}
	}

	//�o�H�T���œ����悤�ɂȂ�΂����͕s�v
	_player.update(&_goal);

	//�I�[�v�����X�g���̃m�[�h��txt�t�@�C���֏����o��
	if (CheckHitKey(KEY_INPUT_W)) {
		printfDx("�m�[�h����%d�ł�\n", nodeMgr.openList.size());
		if (!nodeMgr.openList.empty()) {
			FILE* outputfile = fopen("PopResult.txt", "w");
			if (outputfile == NULL) {
				printfDx("�t�@�C�����g�p���ł��B\n");
				exit(1);
			}
			fprintf(outputfile,"�R�X�g�̏����ŃI�[�v�����X�g��push���܂�\n");
			fprintf(outputfile, "%d�̃m�[�h������܂��B\n", nodeMgr.openList.size());
			while (!nodeMgr.openList.empty()) {
				fprintf(outputfile, "(%3d,%3d)�̃R�X�g��%f�ł�\n", nodeMgr.openList.top().x, nodeMgr.openList.top().y, nodeMgr.openList.top().score);
				nodeMgr.output(nodeMgr.openList.top());
			}
			fclose(outputfile);
		}
	}
}

void TitleScene::draw() {
	_goal.draw();
	_player.draw();
	for (int cnt = 0; cnt < NUM; cnt++) {
		_human[cnt].draw();
	}
	//DrawFormatString(100, 40, GetColor(255, 255, 255), "%d", _grid[1][1].status);	//�m�[�h�J�����p
	DrawFormatString(100, 80, GetColor(255, 255, 255), "�����F%d x:%.1f y:%.1f", _player.distance(&_goal),_player.x,_player.y);	//�S�[���܂ł̋����ƌ��ݒn
	//const static int tmp;
	//tmp = (int)nodeMgr.openList.top().score;
	//DrawFormatString(100, 120, GetColor(255, 255, 255), "S�m�[�h�̍��v�R�X�g:%d", tmp);
	
}
