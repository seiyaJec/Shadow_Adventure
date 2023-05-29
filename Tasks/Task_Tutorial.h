#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�`���[�g���A��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "Task_Player.h"
#include "Task_Map2D.h"
#include "Task_TutorialPlayer.h"
#include "Task_TutorialKeyDisplay.h"
#include "Task_TutorialKeyStickDisplay.h"
#include "Task_Font.h"

namespace  Tutorial
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�`���[�g���A��");	//�O���[�v��
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
		ML::Box2D screen;				//�`���[�g���A���p�̃v���C���
		float screenScaleRateW;			//���ۂ̉�ʂ���̏k�����Q����
		float screenScaleRateH;			//���ۂ̉�ʂ���̏k�����Q�c��
		int categories;		//�`���[�g���A���̍��ڐ�
		int selectingCategories;		//���ݑI�𒆂̍���
		int categoryDistance;			//���ړ��m�̊Ԋu
		TutorialPlayer::Object::SP tutoPlayer;	//���{�v���C���[�̃|�C���^
		Player::Object::SP player;		//�v���C���[�ւ̃|�C���^
		string* categoryName;			//���ږ����i�[����z��ւ̃|�C���^

		int inputDataSize;				//���͂̐�
		InputData* inputData;			//���͏����i�[����z��ւ̃|�C���^
		KeyInput inputNow;				//���݂̓��͏��
		DataForPlay dataForPlay;		//�Q�[���p�f�[�^

		bool retryFlagMemory;			//�`���[�g���A���O�̃��g���C�̏�Ԃ��L��
		bool gameOverFlagMemory;		//�`���[�g���A���O�̃Q�[���I�[�o�[�̏�Ԃ��L��
		Player::Object::SP qaPlayerMemory;	//�`���[�g���A���O�̃v���C���[�ւ̃|�C���^
		Map2D::Object::SP qaMapMemory;		//�`���[�g���A���O�̃}�b�v�ւ̃|�C���^
		shared_ptr<vector<BChara::SP>>qaEnemyMemory;	//�`���[�g���A���O�̓G�ւ̃|�C���^

		TutorialKeyDisplay::Object::SP keyB1, keyB3, keyRT, keyA, keyD, keyM, keyL, keyShift;	//�e�L�[�̃f�B�X�v���C��p��
		TutorialKeyStickDisplay::Object::SP keyStick;

		shared_ptr<vector<TutorialKeyDisplay::Object::SP>> keys;	//�X�e�B�b�N�������L�[�̃f�B�X�v���C�i�ꊇ�ݒ�p�j

		int waitNextInput;				//���̓��͂܂ł̃J�E���g
		int inputCnt;					//���݂̓��͂������J�E���g
		int timeCnt;					//���Ԍv��
		bool finishTutorial;			//�`���[�g���A���I���t���O

		void SetToScreen(ML::Box2D& drawBox_, ML::Box2D& srcBox_);
		void LoadInputData();														//���͏��̓ǂݍ���
		void LoadCategoryName();		//���ږ��̓ǂݍ���
		void CheckInput(int cnt_, const string& inp_,ifstream& fin_);				//�ǂ̃{�^���̓��͂�
		void reflectInput(KeyInput& in_, XI::VGamePad& inp_);			//���͏��̔��f
		void CreateKey(TutorialKeyDisplay::Object::SP& key_, int srcOfsY_, float posX_, float posY_);				//�L�[�̏�����
		void AppearSwitchEffect();		//���ڐ؂�ւ����̃G�t�F�N�g�\��
		void ResetPlayer();		//���{�v���C���[���Z�b�g
		void ResetScreen();		//��ʂ��ƃ��Z�b�g
		void FinishTask();		//�I������
		string FolderPath();		//���ݑI�𒆂̍��ڂ̃p�X��Ԃ�
	};
}