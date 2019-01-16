#include "TopScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

//txtへの書き出し用	//最終的には不要にする
#include <stdio.h>
#include <stdlib.h>


TopScene::TopScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()のseed値をランダム化
	flame_cnt = 0;
	searchingResult = unReach;
}

void TopScene::update()
{
	//探索のフレームタイミングを取る
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

	//オープンリスト中のノードをtxtファイルへ書き出し
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
		printfDx("ノード数は%d個です\n", nodeMgr.openList.size());
		if (!nodeMgr.openList.empty()) {
			FILE* outputfile = fopen("PopResult.txt", "w");
			if (outputfile == NULL) {
				printfDx("ファイルが使用中です。\n");
				exit(1);
			}
			fprintf(outputfile, "ゴール座標は(%4d,%4d)\n", nodeMgr.goal.x, nodeMgr.goal.y);
			fprintf(outputfile, "コストの昇順でオープンリストをpushします\n");
			fprintf(outputfile, "%d個のノードがあります。\n", nodeMgr.openList.size());
			while (!nodeMgr.openList.empty()) {
				fprintf(outputfile, "(%3d,%3d)のコストは%fです\n", nodeMgr.openList.top()->x, nodeMgr.openList.top()->y, nodeMgr.openList.top()->score);
				nodeMgr.openList.pop();
			}
			fclose(outputfile);
		}
	}
}
