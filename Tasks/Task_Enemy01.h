#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G01�F��
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Enemy01
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G");	//�O���[�v��
	const  string  defName("01");		//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		DG::Image::SP imgLeft;
		DG::Image::SP imgRight;
		Anim		  animIdle;
		Anim		  animRunning;
		Anim		  animAttack;
		Anim		  animDamaged;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
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
		void  UpDate()		override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()	override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
	//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����
		enum class AnimState { IDLE, RUNNING, ATTACK, DAMAGED };
		AnimState	animState;
		float		defaultHeight;	//���̍���
		float		attackSpeedRate;//�v���C���[���U�����鑬��
		float		attackAngle;	//�U���̌���
		float		attackCnt;		//�U���ړ��̎���
		int			stayTimeToAttack;	//�v���C���[�𔭌���U������܂ł̑ҋ@����
		ML::Vec2	playerPos;		//�v���C���[�̍��W�L��
		ML::Vec2	targetVec;		//�v���C���[�Ɍ������x�N�g��
		ML::Vec2	moveAttack;		//�U���̈ړ����x

		void Anim(ML::Box2D& draw_);
		void ChangeAnim(AnimState anim_);	//�A�j���[�V�����i���[�V�����j�ύX����

	};
}