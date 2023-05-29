//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Result.h"
#include  "Task_ResultBG.h"
#include  "Task_ResultScoreBoard.h"
#include  "Task_ResultButton.h"

namespace  Result
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
		ge->qa_Result = ge->GetTask<Result::Object>("���U���g");
		//�R���g���[���[
		this->controller = ge->in1;
		//���Ԍv��
		this->timeCnt = 0;
		this->clearTime = 0;
		//���g���C��
		this->retryCount = 0;
		//�{�^��
		this->selectingButton = 0;
		this->buttonDistanceClose = 80;
		//�t�@�C���p�X
		this->filePath = "";
		//���^�X�N�̐���
		ResultBG::Object::Create(true);
		auto score = ResultScoreBoard::Object::Create(true);
		score->pos.x = ge->screen2DWidth / 2.0f;
		score->pos.y = 250;
		//�߂�{�^��
		{
			auto button = ResultButton::Object::Create(true);
			button->pos.x = ge->screen2DWidth / 2.0f - button->imgSizeW + buttonDistanceClose;
			button->pos.y = 440;
			button->orderNum = 0;
			button->displayStr = "back";
		}
		//���g���C�{�^��
		{
			auto button = ResultButton::Object::Create(true);
			button->pos.x = ge->screen2DWidth / 2.0f + button->imgSizeW - buttonDistanceClose;
			button->pos.y = 440;
			button->orderNum = 1;
			button->displayStr = "again";
		}
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
		//���[�h����
		if (true == this->loading) {
			if (true == ge->finishedLoadAppear) {
				switch (this->selectingButton) {
					//�Q�[��������x
				case SelectMode::AGAIN:
					Play::Object::Create(true);
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Play;
					break;
					//�}�b�v�Z���N�g��
				case SelectMode::BACK:
					MapSelect::Object::Create(true);
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Select;
					break;
				}
					DisappearLoad(3, 20);
					ge->KillAll_G("���U���g");
			}
		}



		auto inp = this->controller->GetState();

		//���E�̈ړ�
		if (inp.LStick.BR.down) {
			if (this->selectingButton == SelectMode::BACK) {
				this->selectingButton = SelectMode::AGAIN;
			}
			else {
				this->selectingButton = SelectMode::BACK;
			}
		}
		if (inp.LStick.BL.down) {
			if (this->selectingButton == SelectMode::AGAIN) {
				this->selectingButton = SelectMode::BACK;
			}
			else {
				this->selectingButton = SelectMode::AGAIN;
			}
		}

		//����Ń��[�h�J�n
		if (inp.B1.down) {
			this->loading = true;
			switch (this->selectingButton) {
			case SelectMode::AGAIN:
				AppearLoad(10, 3, 20,ML::Color(1.0f, 1.0f, 0.8f, 0.5f));
				break;
				//�}�b�v�Z���N�g��
			case SelectMode::BACK:
				AppearLoad(10, 3, 20, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
				break;
			}
		}


		++this->timeCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
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