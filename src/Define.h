#pragma once

#define HUMAN 3			//画面内を動く人間の数
#define BLOCKS 2     	//適当に配置する静的障害物の数
#define FREQUENCY 30	//探索のフレーム頻度

//InfluenceMapの計算式に用いる定数
#define Imax 50
#define Cci 100
#define Ctr 300			//人の進行方向の危険度の減衰を司る定数
#define ANGLE 45		//三角形領域の角度
#define IMw 0.03		//算出したIMコストの重み付け

enum eResult {
	unReach,
	notFound,
	arrival
};

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
