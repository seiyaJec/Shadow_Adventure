//-------------------------------------------------------------------
//�Q�[�����f
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GamePause.h"
#include  "Task_PauseBG.h"
#include  "Task_PauseButton.h"
#include  "Task_PauseDisplay.h"
#include  "Task_Tutorial.h"

namespace  GamePause
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
		ge->pauseSelectingButton = 0;
		//�R���g���[���[
		this->controller = ge->in1;
		//�{�^���̌�
		this->buttonMax = 5;
		//����
		this->timeCnt = 0;
		//�{�^���ݒ�
		this->buttonDistanceX = 0;
		this->buttonDistanceY = 76;
		this->buttonGenerateDistance = 10;
		//�t���O
		this->loadingSelect = false;
		this->loadingTitle = false;
		this->loadingTutorial = false;
		this->finishedAppearTutorial = false;
		this->appeardLastButton = false;
		//���^�X�N�̐���
		//�|�[�Y�w�i
		PauseBG::Object::Create(true);
		//�|�[�Y�f�B�X�v���C
		PauseDisplay::Object::Create(true);

		//�{�^������
		ML::Vec2 firstButtonPos;
		firstButtonPos.x = (ge->screen2DWidth / 2.0f) - this->buttonDistanceX;
		firstButtonPos.y = (ge->screen2DHeight / 2.0f) - this->buttonDistanceY;
		this->GenerateButton(firstButtonPos, (float)this->buttonDistanceX, (float)this->buttonDistanceY, this->buttonGenerateDistance);

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
		//�`���[�g���A���ǂݍ��ݏ���
		if (true == this->loadingTutorial) {
			//�`���[�g���A���o�������i���[�h�I����1�x���������j
			if (false == this->finishedAppearTutorial) {
				if (true == ge->finishedLoadAppear) {
					this->SuspendGameTask();
					ge->SuspendAll_G("�|�[�Y");
					this->Suspend(false);
					Tutorial::Object::Create(true);
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Tutorial;
					DisappearLoad(3, 20);
					this->finishedAppearTutorial = true;
				}
			}
			//�`���[�g���A�����̏���
			else{
				auto tutorial = ge->GetTask<Tutorial::Object>("�`���[�g���A��", "NoName");
				if (true == tutorial->finishTutorial) {
					if (true == ge->finishedLoadAppear) {

						tutorial->FinishTask();
						ge->SuspendAll_G("�|�[�Y", false);
						this->StopGameTask();
						this->finishedAppearTutorial = false;
						this->loadingTutorial = false;
						ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Pause;
						DisappearLoad(3, 20);
					}
				}
			}
			return;
		}

		//�{�^�������ׂďo��܂ŏ��������Ȃ�
		if (false == this->appeardLastButton) {
			if (true == lastButton->finishedAppear) {
				this->appeardLastButton = true;
			}
			else {
				return;
			}
		}

		//�}�b�v�Z���N�g�ǂݍ���
		if (true == this->loadingSelect) {
			if (true == ge->finishedLoadAppear) {
				this->KillGameTask();
				ge->KillAll_G("�|�[�Y");
				MapSelect::Object::Create(true);
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Select;
				DisappearLoad(3, 20);
			}
		}

		//�^�C�g���ǂݍ���
		if (true == this->loadingTitle) {
			if (true == ge->finishedLoadAppear) {
				this->KillGameTask();
				ge->KillAll_G("�|�[�Y");
				Title::Object::Create(true);
				DisappearLoad(3, 20);
			}
		}


		
		auto inp = this->controller->GetState();
		//�{�^���̏㉺�ړ�
		if (inp.LStick.BU.down) {
			if (ge->pauseSelectingButton > 0) {
				--ge->pauseSelectingButton;
			}
		}
		if (inp.LStick.BD.down) {
			if (ge->pauseSelectingButton < this->buttonMax - 1) {
				++ge->pauseSelectingButton;
			}
		}

		//����ł��ꂼ��̏���
		if (inp.B1.down) {
			switch (ge->pauseSelectingButton) {
			//�ĊJ
			case ButtonType::CONTINUE:
				this->RestartGameTask();
				ge->KillAll_G("�|�[�Y", false);
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Play;
				break;
			//���g���C
			case ButtonType::RETRY:
				this->RestartGameTask();
				ge->qa_Player->statHP = 0;
				ge->gameOverflag = true;
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Play;
				ge->KillAll_G("�|�[�Y", false);
				break;
			//�`���[�g���A��
			case ButtonType::TUTORIAL:
				this->loadingTutorial = true;
				AppearLoad(10, 3, 20, ML::Color(1.0f, 0.3f, 0.6f, 0.3f));
				break;
			//�}�b�v�Z���N�g
			case ButtonType::SELECT:
				this->loadingSelect = true;
				AppearLoad(10, 3, 20, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
				break;
			//�^�C�g��
			case ButtonType::TITLE:
				this->loadingTitle = true;
				AppearLoad(10, 3, 20);
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

	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�{�^���̐���
	void Object::GenerateButton(ML::Vec2 startPos_, float addPosX_,float addPosY_, int generateDistance_)
	{
		ML::Vec2 buttonPos = startPos_;			//��ڂ̃{�^���̈ʒu
		ML::Vec2 addPos(addPosX_, addPosY_);	//�ʒu�̉��Z
		int orderNumSet = 0;			//����
		int appearWaitSet = 0;			//�o���ҋ@����
		//�Q�[���ĊJ�{�^������
		{
			auto button = PauseButton::Object::Create(true);
			button->pos = buttonPos;
			button->displayStr = "�Q�[���ĊJ";
			button->orderNum = orderNumSet;
			button->appearWait = appearWaitSet;
		}
		buttonPos += addPos;
		++orderNumSet;
		appearWaitSet += generateDistance_;
		//���g���C�{�^������
		{
			auto button = PauseButton::Object::Create(true);
			button->pos = buttonPos;
			button->displayStr = "���g���C";
			button->orderNum = orderNumSet;
			button->appearWait = appearWaitSet;
		}
		buttonPos += addPos;
		++orderNumSet;
		appearWaitSet += generateDistance_;
		//�`���[�g���A���{�^������
		{
			auto button = PauseButton::Object::Create(true);
			button->pos = buttonPos;
			button->displayStr = "�`���[�g���A��";
			button->orderNum = orderNumSet;
			button->appearWait = appearWaitSet;
		}
		buttonPos += addPos;
		++orderNumSet;
		appearWaitSet += generateDistance_;
		//�}�b�v�Z���N�g����
		{
			auto button = PauseButton::Object::Create(true);
			button->pos = buttonPos;
			button->displayStr = "�}�b�v�Z���N�g�ɖ߂�";
			button->orderNum = orderNumSet;
			button->appearWait = appearWaitSet;
		}
		buttonPos += addPos;
		++orderNumSet;
		appearWaitSet += generateDistance_;
		//�^�C�g���߂�{�^������
		{
			this->lastButton = PauseButton::Object::Create(true);
			this->lastButton->pos = buttonPos;
			this->lastButton->displayStr = "�^�C�g���ɖ߂�";
			this->lastButton->orderNum = orderNumSet;
			this->lastButton->appearWait = appearWaitSet;
		}

	}
	//�Q�[���p�^�X�N���ĊJ������
	void Object::RestartGameTask()
	{
		ge->StopAll_G("�{��", false);
		ge->StopAll_G("�t�B�[���h", false);
		ge->StopAll_G("�v���C��", false);
		ge->StopAll_G("�G", false);
		ge->StopAll_G("�G�t�F�N�g", false);
		ge->StopAll_G("�U��", false);
		ge->StopAll_G("�Q�[��UI", false);
		ge->StopAll_G("�Q�[���I�[�o�[�V���h�E", false);
		ge->StopAll_G("���g���C",false);

	}
	
	//�Q�[���^�X�N�𖳌���Ԃɂ���
	void Object::SuspendGameTask()
	{
		ge->StopAll_G("�{��", false);
		ge->StopAll_G("�t�B�[���h", false);
		ge->StopAll_G("�v���C��", false);
		ge->StopAll_G("�G", false);
		ge->StopAll_G("�G�t�F�N�g", false);
		ge->StopAll_G("�U��", false);
		ge->StopAll_G("�Q�[��UI", false);
		ge->StopAll_G("�Q�[���I�[�o�[�V���h�E", false);
		ge->StopAll_G("���g���C", false);
		ge->SuspendAll_G("�{��");
		ge->SuspendAll_G("�t�B�[���h");
		ge->SuspendAll_G("�v���C��");
		ge->SuspendAll_G("�G");
		ge->SuspendAll_G("�G�t�F�N�g");
		ge->SuspendAll_G("�U��");
		ge->SuspendAll_G("�Q�[��UI");
		ge->SuspendAll_G("�Q�[���I�[�o�[�V���h�E");
		ge->SuspendAll_G("���g���C");
	}

	//�Q�[���^�X�N�𖳌������~��Ԃɂ���
	void Object::StopGameTask()
	{
		ge->SuspendAll_G("�{��", false);
		ge->SuspendAll_G("�t�B�[���h", false);
		ge->SuspendAll_G("�v���C��", false);
		ge->SuspendAll_G("�G", false);
		ge->SuspendAll_G("�G�t�F�N�g", false);
		ge->SuspendAll_G("�U��", false);
		ge->SuspendAll_G("�Q�[��UI", false);
		ge->SuspendAll_G("�Q�[���I�[�o�[�V���h�E", false);
		ge->SuspendAll_G("���g���C", false);
		ge->StopAll_G("�{��");
		ge->StopAll_G("�t�B�[���h");
		ge->StopAll_G("�v���C��");
		ge->StopAll_G("�G");
		ge->StopAll_G("�G�t�F�N�g");
		ge->StopAll_G("�U��");
		ge->StopAll_G("�Q�[��UI");
		ge->StopAll_G("�Q�[���I�[�o�[�V���h�E");
		ge->StopAll_G("���g���C");
	}
	//�Q�[���p�^�X�N���L������
	void Object::KillGameTask()
	{
		ge->KillAll_G("�{��");
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�G");
		ge->KillAll_G("�G�t�F�N�g");
		ge->KillAll_G("�U��");
		ge->KillAll_G("�Q�[��UI");
		ge->KillAll_G("�Q�[���I�[�o�[�V���h�E");
		ge->KillAll_G("���g���C");
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