//-------------------------------------------------------------------
//�O���b�h
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Grid.h"

namespace  Grid
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/Grid.png");
		this->canPutColor = DG::Image::Create("./data/image/MapMaker/canPut.png");
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
		this->render2D_Priority[1] = 0.5f;

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
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

		ML::Box2D drawBase(0, 0, 32, 32);
		//�X�N���[���Ή�
		drawBase.Offset(-ge->camera2D.x, -ge->camera2D.y);
		{
			ML::Box2D src(0, 0, 32, 32);
			for (int y = 0; y < 100; ++y) {
				for (int x = 0; x < 100; ++x) {
					ML::Box2D draw = drawBase.OffsetCopy(x * 32, y * 32);
					this->res->img->Draw(draw, src);
				}
			}
		}

		//�J�[�\���̈ʒu�̃}�X�����F���Ȃ鏈��
		if (ge->qa_Cursor != nullptr) {
			if (false == ge->qa_Cursor->movingObject) {
				if (ge->qa_Cursor->cursorMode == CursorMode::MAP) {
					if (true == ge->qa_Cursor->inGameMap) {
						int gridPosX = (int)ge->qa_Cursor->pos.x / 32;
						int gridPosY = (int)ge->qa_Cursor->pos.y / 32;
						gridPosX *= 32;
						gridPosY *= 32;
						ML::Box2D draw = drawBase.OffsetCopy(gridPosX, gridPosY);
						ML::Box2D src(0, 0, 32, 32);
						this->res->canPutColor->Draw(draw, src);
					}
				}
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