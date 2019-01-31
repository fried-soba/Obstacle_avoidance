#pragma once

#define HUMAN 4			//��ʓ��𓮂��l�Ԃ̐�
#define BLOCKS 1     	//�K���ɔz�u����ÓI��Q���̐�
#define FREQUENCY 30	//�T���̃t���[���p�x

//InfluenceMap�̌v�Z���ɗp����萔
#define Imax 50			//�Z�o����IM�R�X�g�̏d�ݕt��
#define Cci 100			//
#define Ctr1 90			//�l�̐i�s�����̊댯�x�̌������i��萔
#define ANGLE 30		//�O�p�`�̈�̊p�x Ctr2��degree�\��
#define IMw 0.03		//�Z�o����IM�R�X�g�S�̂̏d�ݕt��

#define TRIAL 50			//�������̍ۂ̒T����

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
