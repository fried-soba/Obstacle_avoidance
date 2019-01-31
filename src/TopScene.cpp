#include "TopScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

//メモリdump用
#include <stdio.h>
#include <stdlib.h>


TopScene::TopScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()のseed値をランダム化
	flame_cnt = 0;
	searchingResult = unReach;
	
	//回数蓄積用
	success = 0;
	failed = 0;
}

void TopScene::update()
{
	//探索のフレームタイミングを取る
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

	//オープンリスト中のノードをtxtファイルへ書き出し
	if (CheckHitKey(KEY_INPUT_D)) {
		dumpOpenList();
	}

	//Rキーでリセット
	if (CheckHitKey(KEY_INPUT_R)) {
		searchingResult = unReach;
		nodeMgr.player.reset();
		for (int cnt = 0; cnt < HUMAN; cnt++) {
			nodeMgr.human[cnt].reset();
		}
	}

	//衝突orゴールでリセット
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
		printfDx("ノード数は%d個です\n", nodeMgr.openList.size());
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
			DrawBox(nodeMgr.openList.top()->x, nodeMgr.openList.top()->y, nodeMgr.openList.top()->x + 1,
				nodeMgr.openList.top()->y + 1, GetColor(0, 0, 255), TRUE);	//オープンリスト内のノードを青色で表示させる
			nodeMgr.openList.pop();
		}
		fclose(outputfile);
		nodeMgr.openList = tmp;
	}	
	else
		printfDx("opneListは空です\n");
}

void TopScene::summurize() {
	FILE* printsum = fopen("試行結果.txt", "w");
	if (printsum == NULL) {
		printfDx("ファイルが使用中です。\n");
		exit(1);
	}
	float rate = (float)success / (float)((success + failed)) * 100;
	fprintf(printsum, "ゴール:%d回　衝突%d回　ゴール率:%3.1f\%\n", success, failed ,rate);
	printfDx("結果の出力が完了しました\n");
	fclose(printsum);
}
