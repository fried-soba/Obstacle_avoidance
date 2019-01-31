#include "TopScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

//������dump�p
#include <stdio.h>
#include <stdlib.h>


TopScene::TopScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()��seed�l�������_����
	flame_cnt = 0;
	searchingResult = unReach;
	
	//�񐔒~�ϗp
	success = 0;
	failed = 0;
}

void TopScene::update()
{
	//�T���̃t���[���^�C�~���O�����
	flame_cnt %= FREQUENCY;
	if (flame_cnt == 0 && searchingResult == unReach && nodeMgr.player.hitStatus == false) {
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
	if (CheckHitKey(KEY_INPUT_D)) {
		dumpOpenList();
	}

	//R�L�[�Ń��Z�b�g
	if (CheckHitKey(KEY_INPUT_R)) {
		searchingResult = unReach;
		nodeMgr.player.reset();
		for (int cnt = 0; cnt < HUMAN; cnt++) {
			nodeMgr.human[cnt].reset();
		}
	}

	//�Փ�or�S�[���Ń��Z�b�g
	if (searchingResult==arrival || nodeMgr.player.hitStatus) {
		(searchingResult == arrival) ? success++ : failed++;
		searchingResult = unReach;
		nodeMgr.player.reset();
		for (int cnt = 0; cnt < HUMAN; cnt++) {
			nodeMgr.human[cnt].reset();
		}
		if (success + failed >= 3)
			summurize();
	}

}

void TopScene::draw() {
	nodeMgr.draw();	
}

void TopScene::dumpOpenList() {
	NodeList tmp = nodeMgr.openList;

	if (!nodeMgr.openList.empty()) {
		printfDx("�m�[�h����%d�ł�\n", nodeMgr.openList.size());
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
			DrawBox(nodeMgr.openList.top()->x, nodeMgr.openList.top()->y, nodeMgr.openList.top()->x + 1,
				nodeMgr.openList.top()->y + 1, GetColor(0, 0, 255), TRUE);	//�I�[�v�����X�g���̃m�[�h��F�ŕ\��������
			nodeMgr.openList.pop();
		}
		fclose(outputfile);
		nodeMgr.openList = tmp;
	}	
	else
		printfDx("opneList�͋�ł�\n");
}

void TopScene::summurize() {
	FILE* printsum = fopen("���s����.txt", "w");
	if (printsum == NULL) {
		printfDx("�t�@�C�����g�p���ł��B\n");
		exit(1);
	}
	float rate = (float)success / (float)((success + failed)) * 100;
	fprintf(printsum, "�S�[��:%d��@�Փ�%d��@�S�[����:%3.1f\%\n", success, failed ,rate);
	printfDx("���ʂ̏o�͂��������܂���\n");
	fclose(printsum);
}
