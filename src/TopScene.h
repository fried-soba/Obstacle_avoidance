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

	int flame_cnt;	//�t���[���̃^�C�~���O����邽�߂̕ϐ�
	eResult searchingResult;
	Node* savePoint;	//���̒T���X�p���ŒH�蒅�����ݒn
	Player _player;
	Human _human[HUMAN];
	Goal _goal;
	NodeManager nodeMgr;
};
