//-------------------------------------------------------------------
//�}�b�v�I���A�C�R��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_IconMapchip.h"
#include  "Task_DataMapChip.h"
#include  "Task_Map2D.h"

namespace  IconMapchip
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgMapchip = ge->qa_Map->img;
		this->cantPutColor = ge->qa_MakeMapUI->res->cantPutColor;
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
		this->render2D_Priority[1] = 0.25f;
		this->SetImgSize(32, 32);
		//��`
		this->hitBase = setCenterPoint(32, 32);
		this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
		this->srcBase = ML::Box2D(0, 0, 32, 32);
		//���݂̃}�b�v�`�b�v�̎��
		this->mapchipMode = MapchipMode::GROUND;
		this->mapchipModeSize = 4;
		//���^�X�N�̐���
		DataMapChip::Object::Create(true);

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
		auto inp = ge->qa_MapMaker->controller->GetState();
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		this->pos = ge->qa_MakeMapUI->pos;
		this->pos += ML::Vec2(50, 0);

		//�N���b�N���ꂽ�烂�[�h�ύX
		if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				ge->qa_Cursor->cursorMode = CursorMode::MAP;
			}
		}

		//B3�ł��ꂼ��^�C�v�؂�ւ�
		if (true == inp.B3.down) {
			switch (ge->qa_Cursor->cursorMode) {
			case CursorMode::MAP:
				++this->mapchipMode;
				if (this->mapchipMode == this->mapchipModeSize) {
					this->mapchipMode = 1;
				}
				break;
			}
		}
		
		//�A�C�R���̉摜���^�C�v���ƂɕύX
		this->srcBase.x = (ge->qa_Map->chipArr[this->mapchipMode][0] % ge->qa_Map->columns)
							* ge->qa_Map->length;
		this->srcBase.y = (ge->qa_Map->chipArr[this->mapchipMode][0] / ge->qa_Map->columns)
							* ge->qa_Map->length;

		//�T�C�Y�̕ύX
		this->InCursorSizeUp();

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		this->Draw(this->res->imgMapchip);
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