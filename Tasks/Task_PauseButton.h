#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�|�[�Y��ʃ{�^��
//-------------------------------------------------------------------
#include "BObject.h"

namespace  PauseButton
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�|�[�Y");	//�O���[�v��
	const  string  defName("�{�^��");	//�^�X�N��
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
		DG::Font::SP font;
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
		int orderNum;				//�����̏���
		int fontSizeW;				//�t�H���g����
		int fontSizeH;				//�t�H���g�c��
		int appearWait;				//�o����҂���
		int appearTime;				//�o���ɂ����鎞��
		int appearTimeMax;			//�o�����ԍő�
		int appearSpeed;			//�o���X�s�[�h
		int selectingMove;			//�{�^���I�����̈ړ���
		int selectingMoveMax;		//�{�^���I�����̈ړ��ʍő�
		int selectingMoveSpeed;		//�{�^���I�����̈ړ��X�s�[�h
		int selectingAlphaDec;		//�����x���Z�̒l
		int selectingAlphaDecSpeed;	//�����x���Z�̃X�s�[�h
		int selectingAlphaDecMax;	//�����x���Z�̍ő�
		bool isAlphaDec;			//true�Ȃ瓧���x���Z�Afalse�Ȃ���Z
		bool finishedAppear;		//�o������������
		string displayStr;		//�{�^���ɕ\�����镶����
	};
}