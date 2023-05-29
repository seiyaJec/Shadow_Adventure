//-------------------------------------------------------------------
//�}�b�v����Ŏg�p����UI
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MakeMapUI.h"
#include  "Task_Grid.h"
#include  "Task_IconEnemy.h"
#include  "Task_IconMapchip.h"
#include  "Task_DataPlayer.h"
#include  "Task_SelectingIcon.h"
#include  "Task_HideMakeMapUIButton.h"
#include  "Task_MakerSetTimeDisplay.h"
#include  "Task_MakerSetTimeButton.h"
#include  "Task_MakerSetPlayerHPButton.h"
#include  "Task_MakerSetPlayerHPDisplay.h"

namespace MakeMapUI
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->UIbarImg = DG::Image::Create("./data/image/MapMaker/UIbar.png");
		this->cantPutColor = DG::Image::Create("./data/image/MapMaker/cantPut.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->UIbarImg.reset();
		this->GridImg.reset();
		this->cantPutColor.reset();

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
		ge->qa_MakeMapUI = ge->GetTask<MakeMapUI::Object>("�}�b�v����UI", "NoName");
		//�摜�D��x
		this->render2D_Priority[1] = 0.3f;
		//�}�E�X����
		this->mouse = ge->mouse;
		//�摜�T�C�Y
		this->SetImgSize(960, 80);
		//��`
		this->hitBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		//�ʒu���
		this->pos.x = (float)(this->imgSizeW / 2);
		this->pos.y = (float)(ge->screen2DHeight - this->imgSizeH / 2);
		//��\���֌W
		this->hideFlag = false;
		this->hideMovePosY = 0;
		this->hideMoveSpeed = 10;
		//���^�X�N�̐���
		//�G�A�C�R��
		IconEnemy::Object::Create(true);
		//�}�b�v�`�b�v�A�C�R��
		IconMapchip::Object::Create(true);
		//�v���C���[�X�^�[�g�n�_
		DataPlayer::Object::Create(true);
		//�O���b�h
		Grid::Object::Create(true);
		//�I��
		SelectingIcon::Object::Create(true);
		//UI�o�[��\���{�^��
		HideMakeMapUIButton::Object::Create(true);

		//�������Ԑݒ�^�X�N����
		int numberPlace = 1;//��������^�X�N�̈�
		for (int i = 0; i < 3; ++i) {
			//�����\���p�f�B�X�v���C
			auto display = MakerSetTimeDisplay::Object::Create(true);
			display->addPosX = 382 - (i * 29);
			display->numberPlace = numberPlace;
			display->myNumber = ge->qa_MapMaker->dataForPlay.gameTime / numberPlace % 10;
			//�����ݒ�{�^��
			auto buttonTop = MakerSetTimeButton::Object::Create(true);
			buttonTop->linkingDisplay = display;
			buttonTop->buttonMode = buttonTop->ButtonMode::TOP;
			auto buttonBottom = MakerSetTimeButton::Object::Create(true);
			buttonBottom->linkingDisplay = display;
			buttonBottom->buttonMode = buttonBottom->ButtonMode::BOTTOM;

			numberPlace *= 10;
		}

		//HP�ݒ�^�X�N����
		numberPlace = 1;
		//�����\���p�f�B�X�v���C
		auto display = MakerSetPlayerHPDisplay::Object::Create(true);
		display->addPosX = -275;
		display->numberPlace = numberPlace;
		display->myNumber = ge->qa_MapMaker->dataForPlay.playerHPMax / 10;
		//�����ݒ�{�^��
		auto buttonTop = MakerSetPlayerHPButton::Object::Create(true);
		buttonTop->linkingDisplay = display;
		buttonTop->buttonMode = buttonTop->ButtonMode::TOP;
		auto buttonBottom = MakerSetPlayerHPButton::Object::Create(true);
		buttonBottom->linkingDisplay = display;
		buttonBottom->buttonMode = buttonBottom->ButtonMode::BOTTOM;

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
		//�}�E�X�̏��擾
		auto ms = this->mouse->GetState();
		//�ʒu�ݒ�
		this->pos.x = (float)(this->imgSizeW / 2);
		this->pos.y = (float)(ge->screen2DHeight - this->imgSizeH / 2);

		//�\���E��\������
		//�{�^���������ꂽ��
		if (true == this->hideFlag) {
			//���S�ɏ�����܂ň�葬�x�ňړ�
			if (this->hideMovePosY < this->imgSizeH) {
				this->hideMovePosY += this->hideMoveSpeed;
				//���ɍs���߂��Ȃ��悤�ɂ��鏈��
				if (this->hideMovePosY >= this->imgSizeH) {
					this->hideMovePosY = (float)this->imgSizeH;
				}
			}
		}
		//��\�����Ƀ{�^���������ꂽ��
		else {
			//���ɖ߂�܂ň�葬�x�ňړ�
			if (this->hideMovePosY > 0) {
				this->hideMovePosY -= this->hideMoveSpeed;
				//�[���������Ȃ��悤�ɂ��鏈��
				if (this->hideMovePosY <= 0) {
					this->hideMovePosY = 0;
				}
			}
		}
		//��\���A�j���[�V����
		this->pos.y += this->hideMovePosY;

		//�J�����������_��
		this->pos.x += ge->camera2D.x;
		this->pos.y += ge->camera2D.y;

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
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

			ML::Box2D src(0, 0, imgSizeW, imgSizeH);
			this->res->UIbarImg->Draw(draw, src);
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