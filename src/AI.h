#pragma once

/*
DX���C�u�������n�߂Ƃ���DirectX�̎g�p�����ł�FPU�R���g���[�����W�X�^��
DirectX�ɂ���ĒP���x�ɕς����Ă��܂�����double�^�͎g�p���Ȃ�
���������_���g���Ƃ���float�^���g��
*/

class Goal {
public:
	int x, y;
	int size;
	Goal();
	void draw();
};

class Mover {
public:
	int radius,	//���a
		red, green, blue;
	unsigned int color;
	float x, y;		//���S���W
	float angle;	//���ʊp�F�i�s�����̊p�x(���W�A���\�L)
	float speed, xSpeed, ySpeed;	//�ړ����x

	void stop();
};

class Player: public Mover {
private:
	bool hitStatus;
	int red, green, blue;
public:
	Player();
	~Player() = default;
	void update(Goal *goal);
	void draw();
	bool checkHit(float x, float y);
	void checkGoal(int g_x, int g_y);
	int distance(Goal *goal);
};

class Human :public Mover {
private:
	float azimuth;	//���ʊp�F�i�s�����̊p�x(���W�A���\�L)
	bool hitStatus;
	int red, green, blue;
public:
	float x, y;
	bool outside;
	Human();
	void reset();
	void update();
	void draw();
	void exorbitant();
	//To do:�l���m���Ԃ���Ȃ��悤�ɔ��肷��֐������
};



