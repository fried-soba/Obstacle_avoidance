#pragma once

class Define final {
public:
    const static int WIN_W;    //�E�B���h�E�T�C�Y��
    const static int WIN_H;    //�E�B���h�E�T�C�Y�c

    const static float PI;    //�~����
	const static int GRID_SIZE;
	//const static int GRID_X_MAX;	//���m�[�h���i�؂�グ�j
	//const static int GRID_Y_MAX;	//�c�m�[�h���i�؂�グ�j


    enum eStage {
        Stage1,
        Stage2,
        Stage3,
        Stage4,
        Stage5,
        StageEX,
        StageNum,
    };

    enum eLevel {
        Easy,
        Normal,
		Hard,
		Lunatic,
        LevelNum
    };

};
