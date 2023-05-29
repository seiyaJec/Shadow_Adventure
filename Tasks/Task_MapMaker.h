#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�}�b�v����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "Task_DataEnemy.h"

namespace  MapMaker
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�}�b�v����");	//�O���[�v��
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
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		XI::GamePad::SP  controller;
		GameMode		 gamemode;			//�Q�[���A���[�J�[�A�Z�[�u���[�h�̐؂�ւ��p
		GameMode		 gamemodeBeforeLoad;	//���[�h�O�̃Q�[�����[�h
		int				 timeCnt;			//�J�E���g
		DataForPlay dataForPlay;			//�Q�[���p�f�[�^

		string		folderPath;				//�ǂݍ��ރt�H���_�̃p�X
		bool startSave;						//�Z�[�u�J�n������

		ML::Point playMapSize;				//���C���Q�[�����̃}�b�v�T�C�Y

		bool SaveToFile(const string& stageName_,const string& makerName_);
		int SaveEnemyToFile(const string& fpath_, string& mFileName_);		//������G�t�@�C���̐���Ԃ�l�Ƃ���
		void LoadDataForPlay();
		void SetPlayMapSize();				//�v���C���}�b�v�T�C�Y�̐ݒ�
	};
}