#pragma once

class Define final {
public:
    const static int WIN_W;    //ウィンドウサイズ横
    const static int WIN_H;    //ウィンドウサイズ縦

    const static float PI;    //円周率
	const static int GRID_SIZE;
	//const static int GRID_X_MAX;	//横ノード数（切り上げ）
	//const static int GRID_Y_MAX;	//縦ノード数（切り上げ）


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
