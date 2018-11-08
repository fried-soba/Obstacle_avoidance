#include "TitleScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include <time.h>

TitleScene::TitleScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	SRand((unsigned int)time(NULL));	//Rand()��seed�l�������_����
	for (int cnt = 0; cnt < Define::WIN_H; cnt++) {
		_grid[cnt] = new Grid[Define::WIN_W];
	}

	//�O���b�h�ɍ��W���w��
	for (int height = 0; height < Define::WIN_H; height++) {
		for (int width = 0; width < Define::WIN_W; width++) {
			_grid[height][width].y = height;
			_grid[height][width].x = width;
		}
	}
	

	//�����p�Ƀm�[�h�I�[�v��
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
	//DrawFormatString(100, 40, GetColor(255, 255, 255), "%d", _grid[1][1].status);	//�m�[�h�J�����p
	DrawFormatString(100, 80, GetColor(255, 255, 255), "�����F%d x:%.1f y:%.1f", _player.distance(&_goal),_player.x,_player.y);	//�S�[���܂ł̋����ƌ��ݒn
	DrawFormatString(100, 120, GetColor(255, 255, 255), "_grid[50][55] y:%d x:%d", _grid[50][55].y, _grid[50][55].x);
	/*for (int i = 0; i < Define::GRID_X_MAX; i++) {
		for (int j = 0; j < Define::GRID_Y_MAX; j++) {
			DrawFormatString(100, 120, GetColor(255, 255, 255), "%d\n%d", i*Define::GRID_SIZE,j*Define::GRID_SIZE);
		}
	}*/
	
}
