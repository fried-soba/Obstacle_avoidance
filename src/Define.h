#pragma once

#define HUMAN 1			//��ʓ��𓮂��l�Ԃ̐�
#define BLOCKS 10      	//�K���ɔz�u����ÓI��Q���̐�

//InfluenceMap�̌v�Z���ɗp����萔
#define Imax 50
#define Cci 3
#define Ctr 90			//�l�̐i�s�����̊댯�x�̌������i��萔
#define IMw 0.003		//�Z�o����IM�R�X�g�̏d�ݕt��

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
