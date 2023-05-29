//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapMakerGame.h"
#include  "Task_Player.h"
#include  "Task_GameUIPlayerHP.h"
#include  "Task_DataPlayer.h"
#include  "Task_Cursor.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_Enemy.h"
#include  "Task_Map2D.h"
#include  "Task_Ending.h"
#include  "Task_MakerSetTimeDisplay.h"
#include  "Task_GameTimeDisplay.h"
#include  "Task_MakerSetPlayerHPDisplay.h"

namespace  MapMakerGame
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
		ge->imgDebug = DG::Image::Create("./data/image/DebugColor.png");
		ge->gameOverflag = false;
		ge->goalFlag = false;
		ge->retryFlag = false;
		//����
		this->timeReseted = false;
		this->timeCntGameMax = this->translateTimeDisplay();
		this->timeCntGame = timeCntGameMax;
		this->timeCntGameDistance = 60;
		//�t���O
		this->stoppedTask = false;

		
		
		//��肠���������l�ݒ�
		//���\�[�X���풓������
		this->residentResorce.push_back(Enemy00::Resource::Create());
		this->residentResorce.push_back(Effect00::Resource::Create());
		this->residentResorce.push_back(Effect01::Resource::Create());
		this->residentResorce.push_back(Attack00::Resource::Create());
		this->residentResorce.push_back(Attack01::Resource::Create());
		this->residentResorce.push_back(Attack02::Resource::Create());
		//���^�X�N�̐���
		auto dataPlayer = ge->GetTask<DataPlayer::Object>("�I�u�W�F�N�g�f�[�^", "�v���C��");
		auto display = ge->GetTask<MakerSetPlayerHPDisplay::Object>("�}�b�v����UI", "�v���C���[HP�ݒ�f�B�X�v���C");
		//�v���C���[�̐���
		auto player = Player::Object::Create(true);
		//���쎞�̃v���C���[�I�u�W�F�N�g���}�b�v�͈͓��ɂ���΍��W�ݒ�
		if (ge->qa_Map->hitBase.Hit(dataPlayer->hitBase.OffsetCopy(dataPlayer->pos))) {
			player->pos = dataPlayer->pos;
		}
		else {
			player->pos = ML::Vec2(100, 100);
		}
		player->statHPMax = display->myNumber * 10;
		player->statHP = player->statHPMax;
		ge->qa_Player = ge->GetTask<Player::Object>("�v���C��", "��");
		//�v���C���[��HP�̐ݒ�
		GameUIPlayerHP::Object::Create(true);
		//�������ԃf�B�X�v���C�̐ݒ�
		GameTimeDisplay::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�Q�[�����[�h");
		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�Q�[���I�[�o�[�V���h�E");
		ge->KillAll_G("�G");
		//ge->KillAll_G("�G�t�F�N�g");
		ge->KillAll_G("�U��");
		ge->KillAll_G("�Q�[��UI");
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
		auto inp = ge->in1->GetState( );

		//�A�N�Z�X�������̂��߁i�G�̌��o�����炷�j
		ge->qa_Enemys = ge->GetTasks<BChara>("�G");
		ge->qa_Player = ge->GetTask<Player::Object>("�v���C��", "��");

		//�w�肵���t���[�������Ƃ�1�J�E���g
		if (this->timeCnt % this->timeCntGameDistance == 0) {
			--this->timeCntGame;
		}
		//0�ɂȂ�����Q�[���I�[�o�[
		if (this->timeCntGame < 0) {
			ge->gameOverflag = true;
			this->timeCntGame = 0;
		}

		++this->timeCnt;
		//�}�b�v����ɐ؂�ւ��鏈��
		if (this->timeCnt >= 10 && inp.B4.down
			 || true == ge->retryFlag) {
			//�}�b�v�̃T�C�Y���ő�ɖ߂�
			ge->qa_Map->ChangeMapSize(100,100);

			ge->qa_MapMaker->gamemode = GameMode::Maker;
			ge->qa_MapMaker->timeCnt = 0;
			Cursor::Object::Create(true);
			//���g�ɏ��ŗv��
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�ݒ肵���������Ԃ��������鏈��
	int Object::translateTimeDisplay()
	{
		int gameTime = 0;

		auto displays = ge->GetTasks<MakerSetTimeDisplay::Object>("�}�b�v����UI", "���Ԑݒ�f�B�X�v���C");
		for (auto it = displays->begin();
			it != displays->end();
			++it) 
		{
			gameTime += (*it)->myNumber * (*it)->numberPlace;
		}
		return gameTime;
	}
	
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