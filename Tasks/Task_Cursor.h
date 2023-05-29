#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�}�E�X�J�[�\��
//-------------------------------------------------------------------
#include "BObject.h"

namespace  Cursor
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�J�[�\��");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��

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
		DG::Image::SP img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BObject
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
		XI::GamePad::SP  controller;		//�R���g���[���[
		BuState leftButton;					//���N���b�N
		BuState rightButton;				//�E�N���b�N
		CursorMode cursorMode;				//�J�[�\���̃��[�h
		ML::Color color;					//�F
		ML::Vec2 screenPos;					//�X�N���[����̍��W
		ML::Vec2 preMouseCursorPos;			//1�t���[���O�̃}�E�X�̍��W
		bool inGameMap;						//�}�b�v���ɃJ�[�\�������邩
		int  moveCount;						//1�ȏ�Ȃ�ړ����[�h
		bool movingObject;					//�ړ����[�h����


		void MoveMouseCursor(XI::VMouse& ms_);
		void LinkToInput(XI::VMouse& ms_,XI::VGamePad& inp_);
		bool CheckLeftState(BuState state_);
		bool CheckRightState(BuState state_);
		bool Hit(ML::Box2D hitBox_);
		void IsInGameMap();

	};
}