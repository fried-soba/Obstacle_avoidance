#pragma once

#include "AbstractScene.h"
#include "AI.h"
#include "Define.h"
#define NUM 20

class TitleScene : public AbstractScene
{
public:
    TitleScene(IOnSceneChangedListener *impl, const Parameter& parameter);
    virtual ~TitleScene() = default;
    void update() override;
    void draw() ;

	Player _player;
	Human _human[NUM];
	Grid **_grid = new Grid*[Define::WIN_W];
	Goal _goal;
};
