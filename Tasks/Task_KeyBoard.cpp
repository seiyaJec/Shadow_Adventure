//-------------------------------------------------------------------
//�L�[�{�[�h
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_KeyBoard.h"
#include  "Task_Key.h"
#include  "Task_Cursor.h"
#include  "Task_KeyBoardDisplay.h"
#include  "Task_KeyBoardEnter.h"
#include  "Task_KeyBoardChange.h"
#include  "Task_KeyBoardSpace.h"
#include  "Task_KeyBoardBackSpace.h"
#include  "Task_KeyBoardCancel.h"
#include  "Task_KeyBoardMessage.h"

namespace  KeyBoard
{

	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/KeyBoard.png");
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
		//�Q�Ɨp
		ge->qa_KeyBoard = ge->GetTask<KeyBoard::Object>("�L�[�{�[�h", "���C��");
		//�摜�D��x
		this->render2D_Priority[1] = 0.29f;
		//�摜�T�C�Y
		this->SetImgSize(960, 540);
		//�摜��`
		this->drawBase = setCenterPoint(this->imgSizeW,this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//�o���X�s�[�h
		this->moveSpeed = 20;
		//�ʒu���
		this->pos = ML::Vec2(this->imgSizeW / 2.0f, this->imgSizeH / 2.0f);
		//���͏��
		this->userInput = "";
		//���b�Z�[�W
		this->messageStr = "";
		//���͏��
		this->userInputLimit = 20;
		//�t���O
		this->finishInputFlag = false;
		this->changeLangFlag = false;
		this->englishKeyFlag = false;
		this->disappearFlag = false;
		//���^�X�N�̐���
		//�J�[�\��
		if (nullptr == ge->qa_Cursor) {
			Cursor::Object::Create(true);
			ge->qa_Cursor = ge->GetTask<Cursor::Object>("�J�[�\��");
		}
		ge->qa_Cursor->cursorMode = CursorMode::FREE;
		//�f�B�X�v���C
		KeyBoardDisplay::Object::Create(true);
		//�����L�[
		this->GenerateButtonJP();
		//�G���^�[�L�[
		KeyBoardEnter::Object::Create(true);
		//����ϊ��L�[
		KeyBoardChange::Object::Create(true);
		//�X�y�[�X�L�[
		KeyBoardSpace::Object::Create(true);
		//�o�b�N�X�y�[�X�L�[
		KeyBoardBackSpace::Object::Create(true);
		//�L�����Z���{�^��
		KeyBoardCancel::Object::Create(true);
		//���b�Z�[�W�E�B���h�E
		KeyBoardMessage::Object::Create(true);
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
		ge->qa_Cursor = ge->GetTask<Cursor::Object>("�J�[�\��");
		ge->qa_KeyBoard = ge->GetTask<KeyBoard::Object>("�L�[�{�[�h", "���C��");
		//����ϊ�����
		if(false == this->disappearFlag) {
			if (true == this->changeLangFlag) {
				ge->KillAll_GN("�L�[�{�[�h", "�L�[");
				if (false == this->englishKeyFlag) {
					this->GenerateButtonEN();
				}
				else {
					this->GenerateButtonJP();
				}
				this->englishKeyFlag = !this->englishKeyFlag;
				this->changeLangFlag = false;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		this->res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//���{��
	void Object::GenerateButtonJP()
	{
		float keyDistanceW = 16;	//�L�[�̊Ԋu��
		float keyDistanceH = 20;	//�L�[�̊Ԋu�c
		//�L�[���������
		for (int i = 0; i < 75; ++i) {
			auto key = Key::Object::Create(true);
			key->pos = ML::Vec2(90, 150);
			key->fontDrawBase = setCenterPoint(36, 36);
			key->mychar = this->res->HiraganaArr[i];
			int keyPosX = i / 5;
			int keyPosY = i % 5;
			key->pos.x += keyPosX * (key->imgSizeW + keyDistanceW);
			key->pos.y += keyPosY * (key->imgSizeH + keyDistanceH);
		}
	}
	//�p��
	void Object::GenerateButtonEN()
	{
		float keyDistanceW = 16;	//�L�[����
		float keyDistanceH = 20;	//�L�[�c��
		int keyImageSize = 40;		//�L�[�摜�T�C�Y
		ML::Vec2 keyPos = ML::Vec2(110, 150);
		//����
		for (char c = '0'; c <= '9'; ++c) {
			this->CreateKeyHalfSize(c, keyPos);
			keyPos.x += keyImageSize + keyDistanceW;
		}

		//�L��
		this->CreateKeyHalfSize(':', keyPos);
		keyPos.x += keyImageSize + keyDistanceW;
		this->CreateKeyHalfSize('_', keyPos);
		keyPos.x += keyImageSize + keyDistanceW;
		this->CreateKeyHalfSize('/', keyPos);
		keyPos.x += keyImageSize + keyDistanceW;
		this->CreateKeyHalfSize('.', keyPos);

		//�������A���t�@�x�b�g
		keyPos.x = 130;
		keyPos.y += keyImageSize + keyDistanceH;
		for (char c = 'a'; c <= 'z'; ++c) {
			this->CreateKeyHalfSize(c, keyPos);
			keyPos.x += keyImageSize + keyDistanceW;
			if (c == 'm') {
				keyPos.x = 130;
				keyPos.y += keyImageSize + keyDistanceH;
			}
		}

		//�啶���A���t�@�x�b�g
		keyPos.x = 130;
		keyPos.y += keyImageSize + keyDistanceH;
		for (char c = 'A'; c <= 'Z'; ++c) {
			this->CreateKeyHalfSize(c, keyPos);
			keyPos.x += keyImageSize + keyDistanceW;
			if (c == 'M') {
				keyPos.x = 130;
				keyPos.y += keyImageSize + keyDistanceH;
			}
		}


	}

	//���p�̃L�[�����Ƃ��̊֐�
	void Object::CreateKeyHalfSize(char c_, ML::Vec2 pos_)
	{
		auto key = Key::Object::Create(true);
		key->fontDrawBase = setCenterPoint(18, 36);
		char cStr[3] = { c_, '\0', '\0' };
		key->mychar = cStr;
		key->pos = pos_;
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