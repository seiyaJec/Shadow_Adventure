#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�^�C�g��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "Task_TitlePlayer.h"
#include "Task_Button.h"
#include "Task_Font.h"

namespace  Title
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�^�C�g��");	//�O���[�v��
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
		DG::Font::SP fontDebug;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		void  UpDate()			override;	//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		vector<BResource::SP> residentResorce;		//�펞�ǂݍ��ރ��\�[�X
		TitlePlayer::Object::SP qa_Shadow;			//�e�̃|�C���^
		XI::GamePad::SP controller;					//�R���g���[���[
		ML::Box2D logoData[15];						//�^�C�g�����S
		int timeCnt;								//���Ԍv��
		int logoPosX;								//���S�̈ʒu
		bool startingGame;							//�J�n����

		void LoadLogoData();
	};
}