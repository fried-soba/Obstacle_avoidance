#pragma once

#include "AbstractScene.h"
#include "AI.h"
#include "Define.h"
#include "Node.h"
#include "NodeManager.h"

class TopScene : public AbstractScene
{
public:
    TopScene(IOnSceneChangedListener *impl, const Parameter& parameter);
    virtual ~TopScene() = default;
    void update() override;
    void draw();
	void writeOpenList();
	int flame_cnt;
	eResult searchingResult;
	NodeManager nodeMgr;
};
