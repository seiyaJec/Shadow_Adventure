#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G�t�F�N�g3�@��юU����̗�
//-------------------------------------------------------------------
#include "BObject.h"

namespace  Effect03
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G�t�F�N�g");	//�O���[�v��
	const  string  defName("03");	//�^�X�N��
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
		Anim anim;
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
		float angle;		//����
		int   animCnt;		//�A�j���[�V�����p�J�E���g
		int	  animDistance;  //�A�j���[�V�����̊Ԋu

		void Set(ML::Vec2 pos_, float angle = 0, int animDistance = 10);
		void Circle(ML::Vec2 pos_, float distance_, int efctSize_);		//�~�`�ɔ�юU��(�A���O���ݒ��Ɏg�p����悤�Ɂj

	};
}