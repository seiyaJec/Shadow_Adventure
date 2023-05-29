//-------------------------------------------------------------------
//�G00�i�X���C���j�̃}�b�v�f�[�^
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy.h"
#include  "Task_DataEnemy.h"
#include  "Task_IconEnemy.h"
#include  "MyStructure.h"

namespace  Data_Enemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		this->render2D_Priority[1] = 0.49f;
		this->type = ge->qa_IconEnemy->enemyType;
		this->InitializeByType();
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�Z�[�u���[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode == GameMode::SAVE) {
			return;
		}
		//�Q�[�����[�h�֐؂�ւ���Ƃ��̏���
		if (ge->qa_MapMaker->gamemode == GameMode::Game) {
			//�J�n���ɓG����x�����o��������
			if (false == this->created) {
				this->EnemyAppearByType();
				this->created = true;
				this->moveMode = false;
				this->addedToMoveCount = false;
			}
			return;
		}
			this->DataEnemyUpDate();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		{
			ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->ApplySize(draw);
			ML::Box2D src(0, 0, 1, 1);
			ge->qa_MakeMapUI->res->cantPutColor->Draw(draw, src);
		}
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->ApplySize(draw);
			ML::Box2D src(0, 0, imgSizeW, imgSizeH);
			this->img->Draw(draw, src);
		}

	}

	//�G�̃^�C�v���Ƃɏ�����������
	void Object::InitializeByType()
	{
		//�摜�̗D��x
		this->render2D_Priority[1] = 0.49f;
		//�ݒu����
		this->puted = false;
		//�T�C�Y�A�b�v�ő�
		this->addSizeMax = 2;
		switch (this->type) {
		case 0:
			this->img = ge->qa_IconEnemy->res->img00;
			this->SetImgSize(48, 48);
			this->hitBase = setCenterPoint(44, 36);
			this->hitBase.Offset(0, 6);
			this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
			break;
		case 1:
			this->img = ge->qa_IconEnemy->res->img01;
			this->SetImgSize(80, 80);
			this->hitBase = setCenterPoint(44, 36);
			this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
			break;
		}
	}
	
	//�^�C�v���ƂɈႤ�G���o��������
	void Object::EnemyAppearByType()
	{
		switch (this->type) {
		case 0:
		{
			auto enemy = Enemy00::Object::Create(true);
			enemy->pos = this->pos;
			break;
		}
		case 1:
		{
			auto enemy = Enemy01::Object::Create(true);
			enemy->pos = this->pos;
			break;
		}
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
