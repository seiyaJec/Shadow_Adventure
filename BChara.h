#pragma once
#pragma warning(disable:4996)
#pragma once
#include "GameEngine_Ver3_83.h"
#include "MyStructure.h"

//--------------------------------------------------------------------------------------------
// �L�����N�^�ėp�X�[�p�[�N���X
//--------------------------------------------------------------------------------------------

class BChara :public BTask
{
	//�ύX�s����������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������
	//�萔�Ƃ��ėp�ӂ���ϐ�
	//�L�����N�^���ʃ����o�ϐ�
		//���E�̌����i2D�����_�Q�[����p�j
	enum class Angle_LR { Left, Right };
	Angle_LR	angle_LR;
	ML::Vec2	pos;					//�L�����N�^�ʒu
	ML::Box2D	hitBase;				//�����蔻��͈�
	ML::Box2D   attackBase;				//�U���p�̓����蔻��͈�
	ML::Box2D	drawBase;				//�`�掞��`�̃X�P�[��
	ML::Box2D	srcBase;				//�`�挳�͈�
	ML::Box2D	playerSerchBox;			//�v���C���[�T�m�͈�
	ML::Vec2	moveVec;				//�ړ��x�N�g��
	ML::Vec2	damagedVecMax;			//��_���[�W���̈ړ����x�ő�
	int			moveCnt;				//�s���J�E���^
	int			moveTime;				//�ړ����Ԑݒ�
	int			statHP;					//�̗�
	int			statHPMax;				//�̗͂̏��
	int			statAttack;				//�U����
	int			animCnt;				//�A�j���[�V�����J�E���^
	float		jumpPow;				//�W�����v���̏���
	float		fallSpeed;				//�������x
	float		fallSpeedMax;			//�������x�ő�
	bool		hitFlagFoot;			//���n���Ă��邩
	bool		damaged;				//�U�����󂯂���
	bool		touchToSpike;			//�j�ɂ������Ă��邩
	int			damagedCntMax;				//��_���[�W���̃J�E���^
	float		damagedSlowdownX;		//X�̌����iY�͏d�͂��g�p�j
	Angle_LR	damagedAngle_LR;		//������΂�������
	bool		debugFlag;				//�ʂɃf�o�b�O���[�h�L���ɂ������Ƃ��ɗ��p				
	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
	BChara()
		:pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, attackBase(0, 0, 0, 0)
		, drawBase(0, 0, 0, 0)
		, srcBase(0, 0, 0, 0)
		, playerSerchBox(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, statHPMax(0)
		, statHP(0)
		, statAttack(0)
		, animCnt(0)
		, jumpPow(0)
		, fallSpeed(0)
		, fallSpeedMax(10)
		, moveTime(0)
		, hitFlagFoot(false)
		, damaged(false)
		, damagedCntMax(0)
		, damagedSlowdownX(0.1f)
		, damagedAngle_LR(Angle_LR::Right)
		, debugFlag(false)
		, angle_LR(Angle_LR::Right)
	{
	}
	virtual ~BChara() {}

	//�L�����N�^���ʃ��\�b�h
	//�߂荞�܂Ȃ��ړ�����
	void CheckMove(ML::Vec2& est_);
	//�A���O���𐔒l�Ƃ��ĕԂ�
	int Angle_LR_ChangeAngleToInt(Angle_LR ang_);
	//int�^���A���O���Ƃ��ĕԂ�
	void Angle_LR_ChangeIntToAngle(Angle_LR& ang_, int aInt_);
	//�A���O���𔽓]
	void Angle_LR_Reverse(Angle_LR& ang_);
	//�G�L�����Ƃ̓����蔻��
	float CheckDamageToEnemy(ML::Box2D hitBox_);
	//�v���C���[�Ƃ̓����蔻��
	bool  CheckDamageToPlayer();
	//�����ڐG����
	bool CheckFoot();
	//���ڐG����
	bool CheckHead();
	//�ǐڐG����
	bool CheckFront();
	//�������𓥂܂��Ĉʒu�ϊ�
	void MovePosToAngle(ML::Box2D& box_, int x_, int y_);
	//��_���[�W���̈ړ����Ȃ��Ȃ�����
	bool IsZeroDamagedMove();
	//�v���C���[�T�m�̏���
	bool SerchPlayer();
	//�f�o�b�O���[�h�F�����蔻���`�\��
	void DebugDrawBox(ML::Box2D const box_);
	//�A�j���[�V�����摜�I��
	void AnimSet(Anim anim_, int cnt_);
};

