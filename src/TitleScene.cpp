#include "TitleScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

TitleScene::TitleScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()のseed値をランダム化
	for (int cnt = 0; cnt < Define::WIN_W; cnt++) {
		_grid[cnt] = new Grid[Define::WIN_H];
	}

	//検査用にノードオープン
	_grid[1][1].status = true;
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
	_player.update(&_goal);
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
	
}

void TitleScene::draw() {
	_goal.draw();
	_player.draw();
	for (int cnt = 0; cnt < NUM; cnt++) {
		_human[cnt].draw();
	}
	//DrawFormatString(100, 60, GetColor(255, 255, 255), "%d", grid[1][1].status);	//ノード開閉検査用

	/*for (int i = 0; i < Define::GRID_X_MAX; i++) {
		for (int j = 0; j < Define::GRID_Y_MAX; j++) {
			DrawFormatString(100, 120, GetColor(255, 255, 255), "%d\n%d", i*Define::GRID_SIZE,j*Define::GRID_SIZE);
		}
	}*/
	
}
