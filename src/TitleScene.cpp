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
	nodeMgr.Initialize(_goal);
	
	//スタートノードの周辺9マスを全てopenリストに入れてpopされる順序を見てみたい
	/*
	int baseX = (int)_player.x, baseY = (int)_player.y;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			nodeMgr.open(_grid[(int)_player.y - 1 + j][(int)_player.x - 1 + i]);
		}
	}
	printfDx("ノード数は%d個です\n", nodeMgr.openList.size());
	*/
	Node first = nodeMgr.search(nodeMgr.grid[(int)_player.y][(int)_player.x]);	//とりあえず確認用にスタートノード周辺を展開
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

	//経路探索で動くようになればここは不要
	_player.update(&_goal);

	//オープンリスト中のノードをtxtファイルへ書き出し
	if (CheckHitKey(KEY_INPUT_W)) {
		printfDx("ノード数は%d個です\n", nodeMgr.openList.size());
		if (!nodeMgr.openList.empty()) {
			FILE* outputfile = fopen("PopResult.txt", "w");
			if (outputfile == NULL) {
				printfDx("ファイルが使用中です。\n");
				exit(1);
			}
			fprintf(outputfile,"コストの昇順でオープンリストをpushします\n");
			fprintf(outputfile, "%d個のノードがあります。\n", nodeMgr.openList.size());
			while (!nodeMgr.openList.empty()) {
				fprintf(outputfile, "(%3d,%3d)のコストは%fです\n", nodeMgr.openList.top().x, nodeMgr.openList.top().y, nodeMgr.openList.top().score);
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
	//DrawFormatString(100, 40, GetColor(255, 255, 255), "%d", _grid[1][1].status);	//ノード開閉検査用
	DrawFormatString(100, 80, GetColor(255, 255, 255), "距離：%d x:%.1f y:%.1f", _player.distance(&_goal),_player.x,_player.y);	//ゴールまでの距離と現在地
	//const static int tmp;
	//tmp = (int)nodeMgr.openList.top().score;
	//DrawFormatString(100, 120, GetColor(255, 255, 255), "Sノードの合計コスト:%d", tmp);
	
}
