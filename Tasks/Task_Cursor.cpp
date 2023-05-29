//-------------------------------------------------------------------
//�}�E�X�J�[�\��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Cursor.h"
#include  "Task_IconEnemy.h"
#include  "Task_HideMakeMapUIButton.h"
#include  "Task_Effect.h"

namespace  Cursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/MapMaker/cursor.png");
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
		//�摜�D��x
		this->render2D_Priority[1] = 0.2f;
		//�R���g���[���[
		this->controller = ge->in1;
		//�摜�T�C�Y
		this->SetImgSize(32, 32);
		//��`
		this->drawBase = ML::Box2D(0, 0, imgSizeW, imgSizeH);
		//�ʒu���
		this->pos = ML::Vec2(0, 0);
		this->screenPos = ML::Vec2(0, 0);
		this->preMouseCursorPos = ML::Vec2(0, 0);
		//�F
		this->color = ML::Color(1, 1, 1, 1);
		//�{�^�����
		this->leftButton = BuState::OFF;
		this->rightButton = BuState::OFF;
		this->cursorMode = CursorMode::ENEMY;
		this->inGameMap = false;
		//�ړ����
		this->moveCount = 0;
		this->movingObject = false;
		//�J�[�\����\��
		ShowCursor(false);
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ShowCursor(true);
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto inp = this->controller->GetState();
		this->color = ML::Color(1, 1, 1, 1);

		//�}�E�X�{�^���ƃJ�[�\���̏�Ԃ������N����
		this->LinkToInput(ms, inp);
		//�}�b�v���ɃJ�[�\�������邩�m�F
		this->IsInGameMap();

		


		//���{�^���������ꂽ�Ƃ��̏���
		if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
			auto effect = Effect03::Object::Create(true);
			effect->Circle(this->pos, 20, 5);
			if (true == this->inGameMap) {
				if (false == this->movingObject) {
					//�}�b�v��ŃN���b�N�����Ƃ��̏���
					switch (this->cursorMode) {
					case CursorMode::ENEMY:
						ge->qa_IconEnemy->Generate();
						break;
					}
				}
			}
		}

		//LR�Ń��[�h�ؑ�
		if (inp.R1.down) {
			if (false == this->movingObject) {
				if (this->cursorMode == CursorMode::ENEMY) {
					this->cursorMode = CursorMode::MAP;
				}
			}
		}
		else if(inp.L1.down) {
			if (false == this->movingObject) {
				if (this->cursorMode == CursorMode::MAP) {
					this->cursorMode = CursorMode::ENEMY;
				}
			}
		}

		//�ړ����[�h��on�Eoff
		if (this->moveCount > 0) {
			this->movingObject = true;
		}
		else {
			this->movingObject = false;
		}

		//�{�^���N���b�N���ɐF��Z������
		if (this->CheckLeftState(BuState::ON)) {
			this->color = ML::Color(1.0f, 1.0f, 1.0f, 0.3f);
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		//�Q�[�����Ȃ�J�����̈ʒu������
		if (this->cursorMode != CursorMode::FREE) {
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		}
		this->ApplySize(draw);
		ML::Box2D src(0, 0, imgSizeW, imgSizeH);
		this->res->img->Draw(draw, src,this->color);
	}

	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//��`�ƃJ�[�\���̓����蔻��
	bool Object::Hit(ML::Box2D hitBox_)
	{
		if (this->pos.x < hitBox_.x) {
			return false;
		}
		if (this->pos.x >= hitBox_.x + hitBox_.w) {
			return false;
		}
		if (this->pos.y < hitBox_.y) {
			return false;
		}
		if (this->pos.y >= hitBox_.y + hitBox_.h) {
			return false;
		}
		return true;
	}

	//�}�E�X�J�[�\�����Q�[���}�b�v���ɂ��邩
	void Object::IsInGameMap()
	{
		//UI�o�[�Ɠ������Ă�����Q�[���}�b�v���ɂ��Ȃ�
		auto hideButton = ge->GetTask<HideMakeMapUIButton::Object>("�}�b�v����UI", "UI��\���{�^��");
		if(this->Hit(ge->qa_MakeMapUI->hitBase.OffsetCopy(ge->qa_MakeMapUI->pos))){
			this->inGameMap = false;
		}
		else if (this->Hit(hideButton->hitBase.OffsetCopy(hideButton->pos))) {
			this->inGameMap = false;
		}
		else {
			this->inGameMap = true;
		}
	}


	//�}�E�X�J�[�\���̈ړ�
	void Object::MoveMouseCursor(XI::VMouse& ms_)
	{
		//�l�����Z
		this->screenPos.x += ms_.pos_Screen.x - preMouseCursorPos.x;
		this->screenPos.y += ms_.pos_Screen.y - preMouseCursorPos.y;
		SetCursorPos(100, 100);
		//���̒l���L��
		this->preMouseCursorPos.x = 100;
		this->preMouseCursorPos.y = 100;


		//�ړ��̐���
		if (this->screenPos.x < 0) {
			this->screenPos.x = 0;
		}
		if (this->screenPos.x >= ge->screen2DWidth) {
			this->screenPos.x = ge->screen2DWidth - 1.0f;
		}
		if (this->screenPos.y < 0) {
			this->screenPos.y = 0;
		}
		if (this->screenPos.y >= ge->screen2DHeight) {
			this->screenPos.y = ge->screen2DHeight - 1.0f;
		}

	}

	//�}�E�X�̃{�^�������󂯎��
	void Object::LinkToInput(XI::VMouse& ms_, XI::VGamePad& inp_)
	{
		//L�X�e�B�b�N�ňړ�
		float angle = inp_.LStick.angleDYP;
		float volume = inp_.LStick.volume;
		if (volume > 0) {
			ML::Vec2 movePos;
			this->screenPos.x += cos(angle)*(volume * 14);
			this->screenPos.y += sin(angle)*(volume * 14);
		}
		//�}�E�X�J�[�\���ړ��������W�ɕϊ�
		this->MoveMouseCursor(ms_);
		//�J����������Ȃ�J�����̈ʒu�����_�ɂ���
		if (this->cursorMode != CursorMode::FREE) {
			this->pos.x = this->screenPos.x + ge->camera2D.x;
			this->pos.y = this->screenPos.y + ge->camera2D.y;
		}
		//�J������������΃X�N���[���Ōv�Z
		else {
			this->pos.x = this->screenPos.x;
			this->pos.y = this->screenPos.y;
		}


		//���{�^��
		if (leftButton != BuState::ON && (ms_.LB.down || inp_.B1.down)) {
			leftButton = BuState::DOWN;
		}
		else if (ms_.LB.on || inp_.B1.on) {
			leftButton = BuState::ON;
		}
		else if (ms_.LB.up || inp_.B1.up) {
			leftButton = BuState::UP;
		}
		else{
			leftButton = BuState::OFF;
		}

		//���{�^��
		if (rightButton != BuState::ON && (ms_.RB.down || inp_.B2.down)) {
			rightButton = BuState::DOWN;
		}
		else if (ms_.RB.on || inp_.B2.on) {
			rightButton = BuState::ON;
		}
		else if (ms_.RB.up || inp_.B2.up) {
			rightButton = BuState::UP;
		}
		else {
			rightButton = BuState::OFF;
		}

	}
	//���{�^���������ꂽ�Ƃ��̔���
	bool Object::CheckLeftState(BuState state_)
	{
		if (this->leftButton == state_) {
			return true;
		}
		else if (state_ == BuState::ON){
			if (this->leftButton == BuState::DOWN) {
				return true;
			}
		}
		else if (state_ == BuState::OFF) {
			if (this->leftButton == BuState::UP) {
				return true;
			}
		}
		return false;
	}

	//�E�{�^���������ꂽ�Ƃ��̔���
	bool Object::CheckRightState(BuState state_)
	{
		if (this->rightButton == state_) {
			return true;
		}
		else if (state_ == BuState::ON) {
			if (this->rightButton == BuState::DOWN) {
				return true;
			}
		}
		else if (state_ == BuState::OFF) {
			if (this->rightButton == BuState::UP) {
				return true;
			}
		}
	return false;
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