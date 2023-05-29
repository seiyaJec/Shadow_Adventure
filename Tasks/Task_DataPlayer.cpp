//-------------------------------------------------------------------
//�v���C���[�̃f�[�^
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_DataPlayer.h"

namespace  DataPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/Player/PlayerR.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		//�摜�̗D��x
		this->render2D_Priority[1] = 0.48f;
		//�ʒu���
		this->screenPos = ML::Vec2(ge->camera2D.w / 2.0f, ge->camera2D.h / 2.0f);
		//�ݒu
		this->puted = true;
		//�摜�T�C�Y
		this->SetImgSize(100, 74);
		//�T�C�Y���
		this->addSizeMax = 1;
		//��`
		this->hitBase = ML::Box2D(-15, -21, 30, 58);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
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
		//�Q�[�����̏���
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			this->moveMode = false;			//�ړ����[�h���Z�b�g
			this->addedToMoveCount = false; //����������l
			return;
		}

		//���ݒu�̏���
		if (false == this->puted) {
			this->addSize = 0;
			this->moveMode = true;
			this->screenPos.x = -(ge->camera2D.x - ge->qa_Cursor->pos.x);
			this->screenPos.y = -(ge->camera2D.y - ge->qa_Cursor->pos.y);
			if (ge->qa_Cursor->CheckLeftState(BuState::OFF)) {
				if (false == ge->qa_Cursor->inGameMap) {
					this->screenPos = ML::Vec2(ge->camera2D.w / 2.0f, ge->camera2D.h / 2.0f);
				}
				this->puted = true;
			}
		}
		//�ݒu���̏���
		else {
			this->InCursorSizeUp();
			if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				this->moveMode = true;
				if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
					this->puted = false;
				}
			}
		}


		this->pos.x = ge->camera2D.x + screenPos.x;
		this->pos.y = ge->camera2D.y + screenPos.y;
		this->ApplyToMoveCount();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		this->Draw(this->res->img);
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