#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�`���[�g���A���p���{�v���C��
//-------------------------------------------------------------------
#include "BChara.h"
#include "MyStructure.h"

namespace  TutorialPlayer
{

	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�v���C��");	//�O���[�v��
	const  string  defName("�`���[�g���A��");	//�^�X�N��

	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP	imgLeft;
		DG::Image::SP   imgRight;
		DG::Image::SP	imgShadowLeft;
		DG::Image::SP	imgShadowRight;
		Anim			animIdle;
		Anim			animRunning;
		Anim			animJumping;
		Anim			animDoubleJump;
		Anim			animOnWall;
		Anim			animDamaged;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɏ������Ă��Ȃ��ϐ��E���\�b�h�݂̂����ɏ���
		Angle_LR wallJumpAngle;
		enum class AnimState { IDLE, RUNNING, JUMPING, DOUBLEJUMP, ONWALL, DAMAGED };
		AnimState animState;
		const float		wallJumpSpeed = 4;		//�ǃL�b�N�̉��ړ����x
		const float		fallSpeedMaxOnWall = 3;	//�Ǎۂ̗������x�ő�
		const int		wallJumpTime = 25;		//�ǃL�b�N���Œ肳��鎞��(�t���[��)
		const int		inputJumpTimeMax = 5;		//�W�����v�̗\�����͂��󂯕t���鎞�ԁi�t���[���j
		int				attackTime;				//�U���̑��t���[����
		int				attackCnt;				//�U���p�J�E���^
		float			walkSpeed;				//��������
		float			dashRate;				//�_�b�V���ő����Ȃ�{��
		bool			firstJump;				//�W�����v�ς݂�
		int				firstJumpTime;			//��x�ڂ̃W�����v�̒���
		int				inputJump;				//�W�����v�̗\������
		bool			doubleJump;				//��i�W�����v�ς݂�
		float			doubleJumpPow;			//��i�W�����v�̍���
		int				cameraShakeTime;		//��ʂ�h�炷����
		int				cameraShakeInterval;	//��ʂ�h�炷�Ԋu
		ML::Point		cameraShakePosMax;		//��ʂ��ő�ǂꂭ�炢�h�炷��
		bool			isWallJump;				//�ǃL�b�N��Ԃ�
		float			wallJumpPow;			//�ǃL�b�N�̍���
		bool			hitFlagFront;			//���ʂ��ǂɐڐG���Ă��邩
		bool			hitFlagHead;			//�����n�`�ɐڐG���Ă��邩
		bool			attackFlag;				//�U������
		int				damagedCount;			//��_���[�W���̖��G����
		bool			damagedStopMove;		//��_���[�W���̓��͋���

		Angle_LR		ctrlAng;				//�^�C�g����p�A�v���C���[�𓮂�������
		bool			moveLRFlag;				//�ړ��t���O
		bool			dashFlag;				//�_�b�V���t���O
		bool			attackFlag_T;			//�U���t���O
		int				attackNum;				//�ǂ̍U�����o����
		int				pushJumpCnt;			//�W�����v�{�^���i1�ȏ�ŉ�����Ă��锻��j
		int				tutorialMoveCnt;
		int				tutorialAnimation;		//�v���C���[�̓���
		bool			imgSwap;				//�G���V���h�E�ɕύX


		KeyInput playerAction;				//�v���C���[�ւ̓���
		bool			jumpFlag;			//�W�����v�����͂��ꂽ��
		ML::Color		color;				//�������ɂ����邽��
		bool			preAttack;			//1�t���[���O�ɍU����������



		void Anim(ML::Box2D& drawBox_);
		void StartCameraShake(int time_, int interval_, int xMax_, int yMax_);				//��ʂ�h�炷
		void MoveLR(Angle_LR ang_, ML::Vec2& est_);
		void Jump();
		bool IsCanDoubleJump();
		void DoubleJump();
		void WallAction(Angle_LR ang_);
		void MoveWallJump(ML::Vec2& est_);
		void WallJumpStart(Angle_LR ang_);		//�ǃW�����v�̊J�n
		void WallFaling();						//�Ǎۂ̗�������
		void IsOnGround();
		void IsOnCeiling();						//
		void Attack();
		void DamagedMove(ML::Vec2& est_);
		void AttackInitialize(BChara::SP atk_);
	};
}