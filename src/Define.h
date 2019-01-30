#pragma once

#define HUMAN 3			//��ʓ��𓮂��l�Ԃ̐�
#define BLOCKS 2     	//�K���ɔz�u����ÓI��Q���̐�
#define FREQUENCY 30	//�T���̃t���[���p�x

//InfluenceMap�̌v�Z���ɗp����萔
#define Imax 50
#define Cci 100
#define Ctr 300			//�l�̐i�s�����̊댯�x�̌������i��萔
#define ANGLE 45		//�O�p�`�̈�̊p�x
#define IMw 0.03		//�Z�o����IM�R�X�g�̏d�ݕt��

enum eResult {
	unReach,
	notFound,
	arrival
};

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
