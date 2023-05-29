//-------------------------------------------------------------------
//�^�C�g��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_TitlePlayer.h"
#include  "Task_TitleLogo.h"
#include  "Task_TitleMessage.h"
#include  "Task_LoadBG.h"
#include  "Task_Button.h"
#include  "Task_GameSelector.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_Enemy.h"
#include  "Task_MapSelect.h"
#include  "Task_Map2D.h"
#include  "Task_Ending.h"
#include  "Task_KeyConfig.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->fontDebug = DG::Font::Create("�l�r �S�V�b�N", 16, 32);
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
		//�L�[�R���t�B�O���Ȃ���
		ge->qa_KeyConfig = nullptr;
		ge->KillAll_G("�L�[�R���t�B�O");

		//�R���g���[���[
		this->controller = ge->in1;
		ge->qa_Title = ge->GetTask<Title::Object>("�^�C�g��", "NoName");
		ge->imgDebug = DG::Image::Create("./data/image/DebugColor.png");
		ge->camera2D = ML::Box2D(
			0,
			0,
			ge->screen2DWidth,
			ge->screen2DHeight
		);//��肠���������l�ݒ�


		//���S�f�[�^�ǂݍ���
		this->LoadLogoData();
		//���S�̈ʒu�ݒ�
		this->logoPosX = ge->screen2DWidth / 15;

		//�Q�[���J�n����
		this->startingGame = false;

		//���\�[�X���풓������
		this->residentResorce.push_back(Enemy00::Resource::Create());
		this->residentResorce.push_back(Effect00::Resource::Create());
		this->residentResorce.push_back(Effect01::Resource::Create());
		this->residentResorce.push_back(Effect02::Resource::Create());
		this->residentResorce.push_back(Attack00::Resource::Create());
		this->residentResorce.push_back(Attack01::Resource::Create());
		this->residentResorce.push_back(Attack02::Resource::Create());
		//���^�X�N�̐���
		//�}�b�v�̐���
		auto map = Map2D::Object::Create(true);
		map->Load("./data/map/Titlemap.txt");
		//�v���C���̐���
		auto pl = TitlePlayer::Object::Create(true);
		pl->pos.x = 30;
		pl->pos.y = 440;
		ge->qa_TPlayer = pl;
		//�v���C���i�e�j�̐���
		auto sdw = TitlePlayer::Object::Create(true);
		sdw->render2D_Priority[1] = 0.51f;
		sdw->imgSwap = true;
		sdw->pos.x = 30;
		sdw->pos.y = 130;
		this->qa_Shadow = sdw;
		//�G�̐���
		auto enemy = Enemy00::Object::Create(true);
		enemy->Angle_LR_Reverse(enemy->angle_LR);
		enemy->pos.x = 800;
		enemy->pos.y = 440;


		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���\�[�X�̏풓����������i�����Ȃ��Ă�����ɉ��������j
		this->residentResorce.clear();
		//���f�[�^���^�X�N���
		ge->KillAll_G("�^�C�g�����S");
		ge->KillAll_GN("�v���C��","�^�C�g��");
		ge->KillAll_G("�Q�[���Z���N�^�[");
		ge->KillAll_G("�{�^��");
		ge->KillAll_G("�t�B�[���h");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			//auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		//�A�N�Z�X�������̂��߁i�G�̌��o�����炷�j
		ge->qa_Enemys = ge->GetTasks<BChara>("�G");
		ge->qa_Map = ge->GetTask<Map2D::Object>("�t�B�[���h", "�}�b�v");

		//�^�C�g���\���E�Q�[�����[�h�I��
		if (false == this->startingGame) {
			if (this->timeCnt <= 350) {
				if (this->timeCnt % 25 == 0) {
					int logoNum = timeCnt / 25;
					//�^�C�g�����S�o��
					auto logo = TitleLogo::Object::Create(true);
					logo->srcBase = logoData[logoNum];
					logo->drawBase.w = logoData[logoNum].w;
					logo->drawBase.h = logoData[logoNum].h;
					logo->pos.x = (float)(logoPosX);

					//�G�t�F�N�g�\��
					auto efct = Effect02::Object::Create(true);
					efct->pos.x = logo->pos.x + (logoData[logoNum].w / 2);
					efct->pos.y = logo->pos.y + (logoData[logoNum].h / 2);

					this->logoPosX += logoData[logoNum].w;
				}
				//�\���V�[���ȗ�
				if (inp.B1.down) {
					++this->timeCnt;
					while (this->timeCnt <= 350) {
						if (this->timeCnt % 25 == 0) {
							int logoNum = timeCnt / 25;

							auto logo = TitleLogo::Object::Create(true);
							logo->srcBase = logoData[logoNum];
							logo->drawBase.w = logoData[logoNum].w;
							logo->drawBase.h = logoData[logoNum].h;
							logo->pos.x = (float)(logoPosX);
							logo->moveCnt = 50;

							this->logoPosX += logoData[logoNum].w;
						}
						++this->timeCnt;
					}
					--this->timeCnt;
				}

			}
			else{
				if (this->timeCnt == 351) {
					//���b�Z�[�W�o��
					TitleMessage::Object::Create(true);
				}
				//����{�^������������Q�[���X�^�[�g����
				if (inp.B1.down) {
					AppearLoad(10, 3, 20, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
					this->startingGame = true;
				}
			}
		}
		else {
			if (true == ge->finishedLoadAppear)
			{
				MapSelect::Object::Create(true);
				ge->qa_KeyConfig = KeyConfig::Object::Create(true);
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->GameMode::Select;
				
				//���[�h�����ł�����
				DisappearLoad(3, 20);

				this->Kill();
				ge->KillAll_G("�^�C�g�����S");
				ge->KillAll_GN("�v���C��", "�^�C�g��");
				ge->KillAll_G("�Q�[���Z���N�^�[");
				ge->KillAll_G("�{�^��");
				ge->KillAll_G("�t�B�[���h");
				ge->KillAll_G("�G");
				ge->KillAll_G("�^�C�g�����b�Z�[�W");
			}
		}


		++this->timeCnt;
		if (inp.ST.down) {
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
	//�^�C�g���̃��S��ǂݍ���
	void Object::LoadLogoData()
	{
		ifstream fin("./data/title/logoData.txt");
		//�ǂݍ��߂Ȃ������炷�ׂ�0�ŏ�����
		if (!fin) {
			for (int i = 0; i < 15; ++i) {
				this->logoData[i] = ML::Box2D(0, 0, 0, 0);
			}
			return;
		}

		for (int i = 0; i < 15; ++i) {
			fin >> this->logoData[i].x;
			fin >> this->logoData[i].y;
			fin >> this->logoData[i].w;
			fin >> this->logoData[i].h;
		}
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