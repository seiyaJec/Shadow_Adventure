#pragma once
#include "GameEngine_Ver3_83.h"
#include "BChara.h"
#include "MyStructure.h"
#include "Task_KeyConfig.h"
#include "Task_Player.h"
#include "Task_Play.h"
#include "Task_Title.h"
#include "Task_TitlePlayer.h"
#include "Task_Tutorial.h"
#include "Task_Result.h"
#include "Task_KeyBoard.h"
#include "Task_MapSelect.h"
#include "Task_Map2D.h"
#include "Task_MakeMapUI.h"
#include "Task_MapMaker.h"
#include "Task_IconEnemy.h"
#include "Task_IconMapchip.h"
#include "Task_Cursor.h"
#include "Task_GameDebug.h"
#include "Task_GamePause.h"
#include <fstream>
#include <sstream>

//�Q�[�����S��Ŏg�p����\���̂Ȃǂ��`����
//-----------------------------------------------------------------------------------------------

#include "fpscounter.h"

#include "task_Effect00.h"

//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//�J������{�`
	class Camera
	{
		Camera() { }
		Camera(
			const ML::Vec3& tg_,	//	��ʑ̂̈ʒu
			const ML::Vec3& pos_,	//	�J�����̈ʒu
			const ML::Vec3& up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
			float				fov_,	//	����p
			float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
			float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
			float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j			
	public:
		//	�r���[���i�����֘A�j
		ML::Vec3 target;			//	��ʑ̂̈ʒu
		ML::Vec3 pos;			//	�J�����̈ʒu
		ML::Vec3 up;				//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
	//	�ˉe���i����͈͊֘A�j
		float fov;					//	����p
		float nearPlane;			//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
		float forePlane;			//	��N���b�v���ʁi��������͉f��Ȃ��j
		float aspect;				//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j
	//	�s����
		ML::Mat4x4  matView, matProj;
		~Camera();
		using SP = shared_ptr<Camera>;
		//	�J�����𐶐�����
		static SP Create(
			const ML::Vec3& tg_,	//	��ʑ̂̈ʒu
			const ML::Vec3& pos_,	//	�J�����̈ʒu
			const ML::Vec3& up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
			float				fov_,	//	����p
			float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
			float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
			float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
//	�J�����̐ݒ�
		void UpDate();
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine();
		//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
		bool Initialize(HWND wnd_);
		~MyGameEngine();
		//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
		void UpDate();

		//3DPG1�Ή��ɂ��ǉ�
			//2D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set2DRenderState(DWORD l_);
		//3D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set3DRenderState(DWORD l_);

		//�Q�[���G���W���ɒǉ����������͉̂��ɉ�����
		//----------------------------------------------
		MyPG::Camera::SP		camera[4];		//	�J����

		XI::Mouse::SP  mouse;
		XI::GamePad::SP  in1, in2, in3, in4;	//��荇�����S�{
		//�Q�c�J������`
		ML::Box2D				camera2D;	//  �Q�c�X�N���[������p
		map<string, float> evFlags;

		ML::Point				camera2DMovePos;	//�J�������ړ�������l

		//�L�[�R���t�B�O�̌��o�����Ȃ����邽�߂�
		KeyConfig::Object::SP				qa_KeyConfig;

		//�G�̌��o���������Ȃ����邽�߂�
		shared_ptr<vector<BChara::SP>>		qa_Enemys;
		//�}�b�v�̌��o���������Ȃ����邽�߂�
		Map2D::Object::SP					qa_Map;
		//�v���C���̌��o���������Ȃ����邽�߂�
		Player::Object::SP					qa_Player;


		//�I�u�W�F�N�g�̌��o���������炷
		shared_ptr<vector<BObject::SP>>		qa_DataObjects;
		//�}�b�vUI�̌��o���������炷
		MakeMapUI::Object::SP				qa_MakeMapUI;
		//�J�[�\���̌��o���������炷
		Cursor::Object::SP					qa_Cursor;
		//�}�b�v���상�C���^�X�N�̌��o���������炷
		MapMaker::Object::SP				qa_MapMaker;
		//�A�C�R���̌��o���������炷
		IconEnemy::Object::SP				qa_IconEnemy;
		IconMapchip::Object::SP				qa_IconMapchip;


		//�^�C�g��
		Title::Object::SP					qa_Title;
		TitlePlayer::Object::SP				qa_TPlayer;

		//�{�Q�[��
		Play::Object::SP					qa_Play;

		//�`���[�g���A��
		Tutorial::Object::SP				qa_Tutorial;

		//���U���g
		Result::Object::SP					qa_Result;

		//�}�b�v�I��
		MapSelect::Object::SP				qa_MapSelect;

		//�L�[�{�[�h
		KeyBoard::Object::SP				qa_KeyBoard;

		//���[�h�^�X�N���o���A���Ŋ����������i�����l��true�Ɂj
		bool finishedLoadAppear = true;
		bool finishedLoadDisappear = true;

		//�S�[�������i�����V�[���Ŏg�p���邽�߂����Œ�`�j
		bool goalFlag = false;
		//���S����
		bool gameOverflag = false;
		//���g���C�t���O
		bool retryFlag = false;
		//���f�t���O
		bool pauseFlag = false;

		//�`���[�g���A���t���O
		bool inTutorial = false;

		//�|�[�Y��ʂőI�𒆂̃{�^��
		int pauseSelectingButton = 0;

		//�X�e�[�W�t�@�C����
		string stageFilePath = "";


		//�f�o�b�O�p
		DG::Image::SP	imgDebug;
		const bool debugFlagAll = false;
		const float gravity = ML::Gravity(32) * 5;


		//----------------------------------------------
		FPSCounter* c;

		int stage;

		void CreateEffect(int no, ML::Vec2 pos);
		void Dbg_ToConsole(const char* str, ...);
		void Dbg_ToDisplay(int x, int y, const char* str, ...);
		void Dbg_ToTitle(const char* str, ...);
		void Dbg_ToBox(const char* str, ...);
		void Dbg_ToFileOut(const char* str, ...);

		//�f�o�b�O��`
		//��������
		DG::Image::SP rectImage;

		enum DEBUGRECTMODE {
			RED, BLUE, GREEN, ORANGE,
			REDFRAME, BLUEFRAME, GREENFRAME, ORANGEFRAME,
		};
		struct DebugRectInfo {
			ML::Box2D	r;
			int			flag;
		};

		std::vector<DebugRectInfo> debugRectArray;
		//�������p
		void debugRectLoad();
		void debugRectReset();
		void debugRectDraw();
		//�\����������`�ƕ\�����[�h��n��
		//RED,BLUE,GREEN,ORANGE�̋�`��FRAME�͘g�̂�
		void debugRect(ML::Box2D r_, int flag = 4, int offsetx = 0, int offsety = 0);

		enum COUNTER_FLAGS {
			NONE,
			ACTIVE,//�ғ���
			LIMIT,//�I������
			END,//�I��
		};

		struct COUNTER {
			COUNTER_FLAGS flag;
			int count;
		};
		std::map < std::string, COUNTER > counterMap;


		void InitCounter();
		void ActCounter();
		void AllEndCounter();
		COUNTER_FLAGS getCounterFlag(string str);
		void StartCounter(string str, int count);


	};
}
extern MyPG::MyGameEngine* ge;


