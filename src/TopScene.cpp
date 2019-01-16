#include "TopScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

//txt�ւ̏����o���p	//�ŏI�I�ɂ͕s�v�ɂ���
#include <stdio.h>
#include <stdlib.h>


TopScene::TopScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()��seed�l�������_����
	flame_cnt = 0;
	searchingResult = unReach;
}

void TopScene::update()
{
	//�T���̃t���[���^�C�~���O�����
	flame_cnt %= FREQUENCY;
	if (flame_cnt  == 0 && searchingResult == unReach) {
		searchingResult = nodeMgr.search();
	}
	flame_cnt++;

	if (CheckHitKey(KEY_INPUT_G)) {
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

	nodeMgr.update();

	//�I�[�v�����X�g���̃m�[�h��txt�t�@�C���֏����o��
	if (CheckHitKey(KEY_INPUT_W)) {
		writeOpenList();
	}

	if (CheckHitKey(KEY_INPUT_R)) {
		nodeMgr.player.reset();
		for (int cnt = 0; cnt < HUMAN; cnt++) {
			nodeMgr.human[cnt].reset();
		}
	}
}

void TopScene::draw() {
	nodeMgr.draw();	
}

void TopScene::writeOpenList() {
	if (!nodeMgr.openList.empty()) {
		printfDx("�m�[�h����%d�ł�\n", nodeMgr.openList.size());
		if (!nodeMgr.openList.empty()) {
			FILE* outputfile = fopen("PopResult.txt", "w");
			if (outputfile == NULL) {
				printfDx("�t�@�C�����g�p���ł��B\n");
				exit(1);
			}
			fprintf(outputfile, "�S�[�����W��(%4d,%4d)\n", nodeMgr.goal.x, nodeMgr.goal.y);
			fprintf(outputfile, "�R�X�g�̏����ŃI�[�v�����X�g��push���܂�\n");
			fprintf(outputfile, "%d�̃m�[�h������܂��B\n", nodeMgr.openList.size());
			while (!nodeMgr.openList.empty()) {
				fprintf(outputfile, "(%3d,%3d)�̃R�X�g��%f�ł�\n", nodeMgr.openList.top()->x, nodeMgr.openList.top()->y, nodeMgr.openList.top()->score);
				nodeMgr.openList.pop();
			}
			fclose(outputfile);
		}
	}
}
