#include "TitleScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

//txtへの書き出し用
#include <fstream>

TitleScene::TitleScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()のseed値をランダム化
	for (int cnt = 0; cnt < Define::WIN_H; cnt++) {
		_grid[cnt] = new Node[Define::WIN_W];
	}

	//グリッドに座標を指定しゴール地点までの距離を計算
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			_grid[height][width].y = height;
			_grid[height][width].x = width;
			_grid[height][width].calcDistance(_goal);
		}
	}
	
	//スタートノードの周辺9マスを全てopenリストに入れてpopされる順序を見てみたい
	int baseX = (int)_player.x, baseY = (int)_player.y;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			nodeMgr.open(_grid[(int)_player.y - 1 + i][(int)_player.x - 1 + j]);
		}
	}
	
	nodeMgr.open(_grid[(int)_player.x][(int)_player.y]);
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

	//オープンリストの中身をtxtファイルへ書き出し
	if (CheckHitKey(KEY_INPUT_W)) {
		ofstream outputfile("PopResult.txt");
		
		while (!nodeMgr.openList.empty()) {
			outputfile << "("<<nodeMgr.openList.top().x<<","<< nodeMgr.openList.top().y<<") "
				<<"のコストは"<< nodeMgr.openList.top().distance;
			//nodeMgr.openList.pop();
			break;
		}
		
		outputfile.close();
	}
	


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

}

void TitleScene::draw() {
	_goal.draw();
	_player.draw();
	for (int cnt = 0; cnt < NUM; cnt++) {
		_human[cnt].draw();
	}
	//DrawFormatString(100, 40, GetColor(255, 255, 255), "%d", _grid[1][1].status);	//ノード開閉検査用
	DrawFormatString(100, 80, GetColor(255, 255, 255), "距離：%d x:%.1f y:%.1f", _player.distance(&_goal),_player.x,_player.y);	//ゴールまでの距離と現在地
	static int tmp;
	tmp = (int)nodeMgr.openList.top().score;
	DrawFormatString(100, 120, GetColor(255, 255, 255), "Sノードの合計コスト:%d", tmp);
	
}
