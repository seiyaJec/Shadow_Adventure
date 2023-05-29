//-------------------------------------------------------------------
//�^�C�g���p�Q�[���Z���N�^�[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameSelector.h"
#include  "Task_Button.h"
#include  "Task_Effect.h"

namespace  GameSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Title/GameSelector.png");
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
		this->render2D_Priority[1] = 0.29f;
		this->SetImgSize(208, 130);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		this->animDistance = 40;
		this->pressStartFlag = false;
		this->pos = ge->qa_Title->qa_ButtonPlay->pos;
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
		if (false == this->pressStartFlag) {
			switch (ge->qa_Title->gameMode) {
			case 0:
			{
				this->pos = ge->qa_Title->qa_ButtonPlay->pos;
				break;
			}
			case 1:
			{
				this->pos = ge->qa_Title->qa_ButtonMake->pos;
				break;
			}
			case 2:
			{
				this->pos = ge->qa_Title->qa_ButtonEndless->pos;
				break;
			}
			}


			//�A�j���[�V�����i�摜�g�k�j
			if (this->animCnt < this->animDistance) {
				this->addSize = 0;
			}
			else if (this->animCnt < this->animDistance * 2) {
				this->addSize = 1;
			}
			else {
				this->animCnt = 0;
			}

			if (ge->qa_Title->startingGame == true) {
				this->pressStartFlag = true;
				auto efct = Effect03::Object::Create(true);
				efct->Circle(this->pos, 50, 8);
			}
		}
		else {
		}

		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (false == this->pressStartFlag) {
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			this->ApplySizeMove(draw);
			ML::Box2D src = this->srcBase;
			this->res->img->Draw(draw, src);
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