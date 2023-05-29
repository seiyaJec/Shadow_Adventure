#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�L�[�R���t�B�O
//-------------------------------------------------------------------
#include "BObject.h"

namespace  KeyConfig
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�L�[�R���t�B�O");	//�O���[�v��
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
		enum GameMode { Select, SelectSave, KeyBoard, Play, Maker, Pause, ResultCheck, Tutorial };
		int gamemode;	//���݂̃��[�h
		float defaultSpeed;	//�ړ��X�s�[�h
		float speedRate;	//���݂̑����i�{���j
		float speedAttenuationRate;//�o���̃X�s�[�h������
		float speedAcceleration;   //���ł̃X�s�[�h������
		float attenuations;	//�X�s�[�h�����񐔁i�摜�T�C�Y���ϓ������čs����j


		bool triggerDowned;	//�g���K�[�������ꂽ��
		bool appearFlag;	//�o��
	};
}