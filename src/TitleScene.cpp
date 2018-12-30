#include "TitleScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

//txtへの書き出し用	//最終的には不要にする
#include <stdio.h>
#include <stdlib.h>


TitleScene::TitleScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()のseed値をランダム化

	nodeMgr.Initialize(_player,_goal);
	for (int cnt = 0; cnt < BLOCKS; cnt++){
		nodeMgr.block[cnt].giveGrid(nodeMgr.grid);
	}
	eResult result = nodeMgr.search(&nodeMgr.grid[(int)_player.y][(int)_player.x]);	//とりあえず確認用にスタートノード周辺を展開
	if (result == arrival)
		_player.root = &nodeMgr.root;
}

void TitleScene::update()
{
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

	//経路探索で動くようになればここは不要
	_player.update(&_goal);

	//オープンリスト中のノードをtxtファイルへ書き出し
	if (CheckHitKey(KEY_INPUT_W)) {
		if (!nodeMgr.openList.empty()) {
			printfDx("ノード数は%d個です\n", nodeMgr.openList.size());
			if (!nodeMgr.openList.empty()) {
				FILE* outputfile = fopen("PopResult.txt", "w");
				if (outputfile == NULL) {
					printfDx("ファイルが使用中です。\n");
					exit(1);
				}
				fprintf(outputfile, "ゴール座標は(%4d,%4d)\n", _goal.x, _goal.y);
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

	if (CheckHitKey(KEY_INPUT_R)) {
		_player.reset();
		for (int cnt = 0; cnt < NUM; cnt++) {
			_human[cnt].reset();
		}
	}
}

void TitleScene::draw() {
	_goal.draw();
	_player.draw();
	for (int cnt = 0; cnt < NUM; cnt++) {
		_human[cnt].draw();
	}
	for (unsigned int cnt = 0; cnt < nodeMgr.root.size(); cnt++) {
		int x1 = nodeMgr.root[cnt].x;
		int y1 = nodeMgr.root[cnt].y;
		int x2 = x1 + 1;
		int y2 = y1 + 1;
		DrawBox(x1, y1, x2, y2, GetColor(255, 255, 255), FALSE);
	}
	for (int cnt = 0; cnt < BLOCKS; cnt++)
		nodeMgr.block[cnt].draw();

	DrawFormatString(100, 80, GetColor(255, 255, 255), "距離：%d x:%.0f y:%.0f", _player.distance(&_goal),_player.x,_player.y);	//ゴールまでの距離と現在地	
}
