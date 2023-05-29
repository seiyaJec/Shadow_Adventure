#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�t�H���g�����ݒ�^�X�N
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Font
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�t�H���g�����ݒ�");	//�O���[�v��
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
		DG::Font::SP font;				//�t�H���g
		ML::Vec2	 pos;				//�ʒu
		ML::Color	 color;				//�F

		void SetPos(float x_, float y);		//�ʒu�̎w��
		void SetPos(const ML::Vec2& pos_);	//�ʒu�̎w��
		void SetSize(int w_, int h_);	//�T�C�Y�̐ݒ�
		void SetString(const string& str_);	//�\�����镶����̐ݒ�
	private:
		//�Ԉ���ĕύX�ł��Ȃ��悤�ɂ���ϐ��͂�����
		bool finishSet;					//�T�C�Y���ݒ�ς݂�
		int fontSizeW;					//1�����̉���
		int fontSizeH;					//1�����̏c��
		ML::Box2D drawBase;				//�`���`
		string displayStr;				//�\�����镶����
	};
}