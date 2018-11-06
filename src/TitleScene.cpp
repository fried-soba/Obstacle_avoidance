#include "TitleScene.h"
#include <DxLib.h>
#include "GameScene.h"
#include "Define.h"
#include "AI.h"
#include <time.h>
#define NUM 1

TitleScene::TitleScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
	//SRand((unsigned int)time(NULL));	//Rand()��seed�l�������_����
	extern Grid **g_grid;					//AI.cpp�̑��Œ�`
	for (int cnt = 0; cnt < Define::WIN_W; cnt++) {
		g_grid[cnt] = new Grid[Define::WIN_H];
	}

	//�����p�Ƀm�[�h�I�[�v��
	g_grid[1][1].status = true;
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
	if (CheckHitKey(KEY_INPUT_H)) {
		Parameter parameter;
		parameter.set(GameScene::ParameterTagLevel, Define::eLevel::Hard);
		const bool stackClear = false;
		_implSceneChanged->onSceneChanged(eScene::Game, parameter, stackClear);
		return;
	}
	if (CheckHitKey(KEY_INPUT_L)) {
		Parameter parameter;
		parameter.set(GameScene::ParameterTagLevel, Define::eLevel::Lunatic);
		const bool stackClear = false;
		_implSceneChanged->onSceneChanged(eScene::Game, parameter, stackClear);
		return;
	}
	*/
	
}

void TitleScene::draw() {
	static Human human[NUM];
	static Player player;
	extern Grid **g_grid;
	extern Goal g_goal;
	player.update();
	for (int cnt = 0; cnt < NUM; cnt++) {
		human[cnt].update();
		if (human[cnt].outside == true) {
			human[cnt].reset();
		}
		if (player.checkHit(human[cnt].x, human[cnt].y)) {
			player.stop();
			human[cnt].stop();
		}
	}
	player.checkGoal(g_goal.x, g_goal.y);
	g_goal.draw();
	player.draw();
	for (int cnt = 0; cnt < NUM; cnt++) {
		human[cnt].draw();
	}
	//DrawFormatString(100, 60, GetColor(255, 255, 255), "%d", grid[1][1].status);	//�m�[�h�J�����p

	/*for (int i = 0; i < Define::GRID_X_MAX; i++) {
		for (int j = 0; j < Define::GRID_Y_MAX; j++) {
			DrawFormatString(100, 120, GetColor(255, 255, 255), "%d\n%d", i*Define::GRID_SIZE,j*Define::GRID_SIZE);
		}
	}*/
	
}
