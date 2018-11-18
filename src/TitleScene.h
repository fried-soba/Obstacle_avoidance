#pragma once

#include "AbstractScene.h"
#include "AI.h"
#include "Define.h"
#include "Node.h"
#define NUM 1

class TitleScene : public AbstractScene
{
public:
    TitleScene(IOnSceneChangedListener *impl, const Parameter& parameter);
    virtual ~TitleScene() = default;
    void update() override;
    void draw() ;

	Player _player;
	Human _human[NUM];
	Node **_grid = new Node*[Define::WIN_H];
	Goal _goal;
	NodeManager nodeMgr;
};
