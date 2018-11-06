#include "GameScene.h"
#include <DxLib.h>

const char* GameScene::ParameterTagStage = "ParameterTagStage";//パラメータのタグ「ステージ」
const char* GameScene::ParameterTagLevel = "ParameterTagLevel";//パラメータのタグ「レベル」

GameScene::GameScene(IOnSceneChangedListener* impl, const Parameter& parameter) : AbstractScene(impl, parameter)
{
    _level = parameter.get(ParameterTagLevel);
}

void GameScene::update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		Parameter parameter;
		const bool stackClear = true;
		_implSceneChanged->onSceneChanged(eScene::Title, parameter, stackClear);
		return;

	}
}

void GameScene::draw() 
{
	static int r = 255, g = 255, b = 255;
	static int timer = 0;
	timer++;
	if (timer % 7 == 0) {
		r+=5;
		r %= 256;
	}
	if (timer % 13 == 0) {
		g+=5;
		g %= 256;
	}
	if (timer % 17 == 0) {
		b+=5;
		b %= 256;
	}
    DrawFormatString(100, 100, GetColor(r, g, b), "ゲームレベルは %d です", _level);
}
