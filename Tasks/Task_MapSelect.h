#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�}�b�v�I��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  MapSelect
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�}�b�v�I��");	//�O���[�v��
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
		XI::GamePad::SP controller;
		enum POSITION { UP, CENTER, DOWN };
		enum SELECTMODE { TITLE = -1, LOAD, PLAY, REMOVE, SAVE };
		int selectMode;		//�I���̃��[�h
		int selectingPos;			//�I�𒆂̃X�e�[�W�i��E���E���j
		int page;					//���݂̃y�[�W
		int pageMax;				//�y�[�W�ő�
		int displayPos;				//�I�𒆂̃X�e�[�W�i�S�z�񒆂̂ǂꂩ�j
		bool saving;
		bool appearLoad;				//���[�h�J�n������
		bool backToTitle;			//�^�C�g���ɖ߂邩
		string folderPath;			//�I�������t�H���_�[�ւ̃p�X
		MapFileData mapData[15];
		string stageNameInput;		//�Z�[�u���̃}�b�v������
		string makerNameInput;		//�Z�[�u���̐���Җ�����

		bool inCheck;				//�m�F��ʂ�\�������@

		void LoadMapFile();
	};
}