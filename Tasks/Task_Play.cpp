//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Play.h"
#include  "Task_Player.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_Enemy.h"
#include  "Task_Map2D.h"
#include  "Task_Ending.h"
#include  "Task_GameDebug.h"
#include  "Task_Result.h"
#include  "Task_BeforePlay.h"
#include  "Task_GameUIPlayerHP.h"
#include  "Task_GameUIGoalDisplay.h"
#include  "Task_GameRetry.h"
#include  "Task_GameTimeDisplay.h"
#include  "Task_GamePause.h"

namespace  Play
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->fontDebug = DG::Font::Create("�l�r �S�V�b�N",16,32);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->fontDebug.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		//�t���O
		ge->goalFlag = false;
		ge->gameOverflag = false;
		ge->imgDebug = DG::Image::Create("./data/image/DebugColor.png");
		ge->camera2D = ML::Box2D(
			ge->screen2DWidth/3,
			ge->screen2DHeight/3,
			ge->screen2DWidth,
			ge->screen2DHeight
		);//��肠���������l�ݒ�
		//�Q�[���p�t�@�C���ǂݍ���
		this->dataForPlay.Load(ge->stageFilePath);
		//���Ԑݒ�
		this->timeReseted = false;
		this->timeCntGameMax = this->dataForPlay.gameTime;
		this->timeCntGame = timeCntGameMax;
		this->timeCntGameDistance = 60;
		//�t���O
		this->stoppedTask = false;

		//���\�[�X���풓������
		this->residentResorce.push_back(Enemy00::Resource::Create());
		this->residentResorce.push_back(Effect00::Resource::Create());
		this->residentResorce.push_back(Effect01::Resource::Create());
		this->residentResorce.push_back(Attack00::Resource::Create());
		this->residentResorce.push_back(Attack01::Resource::Create());
		this->residentResorce.push_back(Attack02::Resource::Create());
		//���^�X�N�̐���

		//�ŏ��̏����i���g���C����Ȃ������j
		if (false == ge->retryFlag) {
			//�}�b�v�̐���
			auto map = Map2D::Object::Create(true);
			map->Load(ge->stageFilePath);
			//�v���C���̐���
			auto pl = Player::Object::Create(true);
			pl->pos = this->dataForPlay.playerStartPos;
			pl->statHPMax = this->dataForPlay.playerHPMax;
			pl->statHP = this->dataForPlay.playerHPMax;
			//���g���C�p�^�X�N����
			GameRetry::Object::Create(true);
		}
		//���g���C����
		else {
			auto retry = ge->GetTask < GameRetry::Object>("���g���C");
			++retry->retryCount;
			//�}�b�v�̐���
			auto map = Map2D::Object::Create(true);
			map->Load(ge->stageFilePath);
			//�v���C���̐���
			auto pl = Player::Object::Create(true);
			pl->SetRetryPos(this->dataForPlay.playerStartPos);
			pl->statHPMax = this->dataForPlay.playerHPMax;
			pl->statHP = this->dataForPlay.playerHPMax;
			pl->retryAppear = true;
		}
		//�v���C���[HPUI
		GameUIPlayerHP::Object::Create(true);
		//��������
		GameTimeDisplay::Object::Create(true);
		//�_
		for (int i = 0; i < 20; ++i) {
			auto effect = Effect04::Object::Create(true);
			int basePosX = i * 150;
			effect->pos = ML::Vec2((float)(basePosX + rand() % 100), (float)(rand() % 300));
		}
		//���g���C�t���O������
		ge->retryFlag = false;


		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		//���\�[�X�̏풓����������i�����Ȃ��Ă�����ɉ��������j
		this->residentResorce.clear();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{

		//���g���C����
		if (true == ge->retryFlag) {
			ge->KillAll_G("�{��");
			ge->KillAll_G("�t�B�[���h");
			ge->KillAll_G("�v���C��");
			ge->KillAll_G("�G");
			ge->KillAll_G("�G�t�F�N�g");
			ge->KillAll_G("�U��");
			ge->KillAll_G("�Q�[��UI");
			ge->KillAll_G("�Q�[���I�[�o�[�V���h�E");
			Play::Object::Create(true);
			return;
		}



		auto inp = ge->in1->GetState( );

		//�A�N�Z�X�������̂��߁i�G�̌��o�����炷�j
		ge->qa_Play = ge->GetTask<Play::Object>("�{��", "����");
		ge->qa_Enemys = ge->GetTasks<BChara>("�G");
		ge->qa_Map = ge->GetTask<Map2D::Object>("�t�B�[���h", "�}�b�v");
		ge->qa_Player = ge->GetTask<Player::Object>("�v���C��", "��");

		


		//�S�[������
		if (true == ge->goalFlag) {
			//��x�������ԃ��Z�b�g
			if (false == this->timeReseted) {
				this->timeCnt = 0;
				this->timeReseted = true;
			}

			//�w�肵�����ԂɂȂ�����V�[���؂�ւ�
			if (this->timeCnt >= 200) {
				if (this->timeCnt == 200) {
					AppearLoad(10, 3, 20, ML::Color(1.0f, 1.0f, 1.0f, 0.5f));
				}
				if (true == ge->finishedLoadAppear) {
					auto retry = ge->GetTask<GameRetry::Object>("���g���C");
					auto result = Result::Object::Create(true);
					result->clearTime = this->timeCntGameMax - this->timeCntGame;
					result->retryCount = retry->retryCount;
					result->filePath = ge->stageFilePath;
					ge->retryFlag = false;
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->ResultCheck;
					ge->KillAll_G("�{��");
					ge->KillAll_G("�t�B�[���h");
					ge->KillAll_G("�v���C��");
					ge->KillAll_G("�G");
					ge->KillAll_G("�G�t�F�N�g");
					ge->KillAll_G("�U��");
					ge->KillAll_G("�Q�[��UI");
					ge->KillAll_G("�Q�[���I�[�o�[�V���h�E");
					ge->KillAll_G("���g���C");
					DisappearLoad(3, 20);
				}
			}

		}
		//�S�[�����ĂȂ���Ύ��Ԍv��
		else {
			//�w�肵���t���[�������Ƃ�1�J�E���g
			if (this->timeCnt % this->timeCntGameDistance == 0) {
				--this->timeCntGame;
			}
			//0�ɂȂ�����Q�[���I�[�o�[
			if (this->timeCntGame < 0) {
				ge->gameOverflag = true;
				this->timeCntGame = 0;
			}

			//�X�^�[�g�{�^���������ꂽ��Q�[�����f
			if (inp.ST.down) {
				ge->pauseFlag = true;
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Pause;
				this->stoppedTask = false;
				//�^�X�N���ꎞ��~
				ge->StopAll_G("�{��");
				ge->StopAll_G("�t�B�[���h");
				ge->StopAll_G("�v���C��");
				ge->StopAll_G("�G");
				ge->StopAll_G("�G�t�F�N�g");
				ge->StopAll_G("�U��");
				ge->StopAll_G("�Q�[��UI");
				ge->StopAll_G("�Q�[���I�[�o�[�V���h�E");
				ge->StopAll_G("���g���C");
				GamePause::Object::Create(true);
			}



		}
		++this->timeCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}

	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------




	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}