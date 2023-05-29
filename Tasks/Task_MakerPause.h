#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���f
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "Task_PauseButton.h"

namespace  MakerPause
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�|�[�Y");	//�O���[�v��
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
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		XI::GamePad::SP  controller;
		enum ButtonType { CONTINUE, TUTORIAL, SELECT, TITLE };

		int buttonMax;				//�{�^���̐�
		int timeCnt;				//���Ԍv��
		int buttonDistanceX;		//�{�^����\������Ԋu(X����)
		int buttonDistanceY;		//�{�^����\������Ԋu(Y�����j
		int buttonGenerateDistance;	//�{�^�����\�������Ԋu�i���ԁj
		bool loadingTutorial;		//�`���[�g���A����ǂݍ��ݒ�
		bool finishedAppearTutorial;//�`���[�g���A���o���I���t���O
		bool loadingSelect;			//�Z���N�g��ǂݍ��ݒ�
		bool loadingTitle;			//�^�C�g����ǂݍ��ݒ�
		bool appeardLastButton;		//�Ō�̃{�^�����\�����ꂽ��
		PauseButton::Object::SP		lastButton;	//�Ō�̃{�^��

		void GenerateButton(ML::Vec2 startPos_, float addPosX_, float addPosY_, int generateDistance_);		//�{�^���̐���
		void SuspendMakerTask();			//����^�X�N���~���疳����Ԃ�
		void StopMakerTask();			//����^�X�N�𖳌������~��Ԃ�
		void RestartMakerTask();	//����ĊJ
		void KillMakerTask();		//����L��
	};
}