#pragma once

#include "AbstractScene.h"
#include "AI.h"
#include "Define.h"
#include "Node.h"

class TopScene : public AbstractScene
{
public:
    TopScene(IOnSceneChangedListener *impl, const Parameter& parameter);
    virtual ~TopScene() = default;
    void update() override;
    void draw();
	void writeOpenList();

	int flame_cnt;	//フレームのタイミングを取るための変数
	eResult searchingResult;
	Node* savePoint;	//一回の探索スパンで辿り着く現在地
	Player _player;
	Human _human[HUMAN];
	Goal _goal;
	NodeManager nodeMgr;
};
