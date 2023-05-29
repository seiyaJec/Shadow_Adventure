//-------------------------------------------------------------------
//�`���[�g���A��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorial.h"
#include  "Task_TutorialPlayer.h"
#include  "Task_TutorialCategory.h"
#include  "Task_Key.h"
#include  "Task_Effect.h"

namespace  Tutorial
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Tutorial/BackGround.png");
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
		ge->qa_Tutorial = ge->GetTask<Tutorial::Object>("�`���[�g���A��");
		ge->inTutorial = true;
		//���g���C�t���O�L���E���Z�b�g
		this->retryFlagMemory = ge->retryFlag;
		ge->retryFlag = false;
		//�Q�[���I�[�o�[�t���O�L���E���Z�b�g
		this->gameOverFlagMemory = ge->gameOverflag;
		ge->gameOverflag = false;


		//�摜�D��x
		this->render2D_Priority[1] = 0.99f;
		//���Ԍv��
		this->timeCnt = 0;
		//����
		this->categories = 8;
		this->selectingCategories = 0;
		this->categoryDistance = 10;
		//����
		this->inputDataSize = 0;

		//���͏��̓ǂݍ���
		this->LoadInputData();
		this->inputNow = this->inputData[0].input;
		this->waitNextInput = this->inputData[0].distanceOfNextInput;

		//���ږ��̓ǂݍ���
		this->LoadCategoryName();

		//�Q�[�����̓ǂݍ���
		this->dataForPlay.Load(this->FolderPath() + "tutorialMap.txt");

		//�J�����̐ݒ�
		ge->camera2D = ML::Box2D(
			0,
			0,
			ge->screen2DWidth,
			ge->screen2DHeight
		);

		this->screen = ML::Box2D(450, 100, 480, 270);
		this->screenScaleRateW = (float)this->screen.w / ge->camera2D.w;
		this->screenScaleRateH = (float)this->screen.h / ge->camera2D.h;
		
		//���^�X�N�̐���
		//�}�b�v�E�G�̋L���E����
		this->qaMapMemory = ge->qa_Map;
		this->qaEnemyMemory = ge->qa_Enemys;
		auto map = Map2D::Object::Create(true);
		ge->qa_Map = map;
		map->Load(this->FolderPath() + "tutorialMap.txt");
		//�v���C���̐���
		this->qaPlayerMemory = ge->qa_Player;
		this->player = Player::Object::Create(true);
		this->player->pos = this->dataForPlay.playerStartPos;
		//�`���[�g���A���p���{�̐���
		this->tutoPlayer = TutorialPlayer::Object::Create(true);
		this->tutoPlayer->pos = this->dataForPlay.playerStartPos;
		//�L�[�̐���
		this->CreateKey(this->keyB1, 0, 570,440);
		this->CreateKey(this->keyB3, 2, 545, 410);
		this->CreateKey(this->keyA, 5, 720, 420);
		this->CreateKey(this->keyD, 7, 790, 420);
		this->CreateKey(this->keyM, 8, 870, 440);
		this->CreateKey(this->keyL, 9, 905, 420);

		//�V�t�g�L�[
		this->keyShift = TutorialKeyDisplay::Object::Create(true);
		this->keyShift->useShift = true;
		this->keyShift->SetImgSize(64, 32);
		this->keyShift->pos = ML::Vec2(736, 454);
		this->keyShift->drawBase = setCenterPoint(this->keyShift->imgSizeW, this->keyShift->imgSizeH);
		this->keyShift->srcBase = ML::Box2D(0, 0, this->keyShift->imgSizeW, this->keyShift->imgSizeH);
		//RT�L�[
		this->keyRT = TutorialKeyDisplay::Object::Create(true);
		this->keyRT->useRT = true;
		this->keyRT->pos = ML::Vec2(610, 415);
		this->keyRT->SetImgSize(32, 64);
		this->keyRT->drawBase = setCenterPoint(this->keyRT->imgSizeW, this->keyRT->imgSizeH);
		this->keyRT->srcBase = ML::Box2D(0, 0, this->keyRT->imgSizeW, this->keyRT->imgSizeH);
		//�ꊇ�ݒ�p��ݒ�
		this->keys = ge->GetTasks<TutorialKeyDisplay::Object>("�`���[�g���A��", "���͕\��");
		
		
		//�X�e�B�b�N
		this->keyStick = TutorialKeyStickDisplay::Object::Create(true);
		this->keyStick->pos = ML::Vec2(480, 430);

		//���ڂ̕\��
		for (int i = 0; i < this->categories; ++i) {
			auto category = TutorialCategory::Object::Create(true);
			category->pos = ML::Vec2(225, 50);
			category->pos.y += (float)(this->categoryDistance + category->imgSizeH) * i;
			category->fontObj->pos = category->pos;
			category->fontObj->SetString(this->categoryName[i]);
			category->orderNum = i;
		}


		//�؂�ւ��G�t�F�N�g�\��
		this->AppearSwitchEffect();

		//�����Ń|�[�Y��ʂ̉摜�D��x���_�E���I���̃Q�[����ʂ͔�\���I



		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		delete[] this->inputData;
		delete[] this->categoryName;

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		ge->qa_Enemys = ge->GetTasks<BChara>("�G");

		//���g���C����
		if (true == ge->retryFlag) {
			this->player->Kill();
			auto shadow = ge->GetTask<BTask>("�Q�[���I�[�o�[�V���h�E");
			if (nullptr != shadow) {
				shadow->Kill();
			}
			ge->gameOverflag = false;
			this->player = Player::Object::Create(true);
			this->player->SetRetryPos(this->dataForPlay.playerStartPos);
			this->player->retryAppear = true;
			ge->retryFlag = false;
		}

		//B4�Ō��{���Z�b�g
		if (inp.B4.down) {
			this->ResetPlayer();
		}

		//L1,R1�ō��ڐ؂�ւ�
		if (inp.L1.down) {
			if (this->selectingCategories > 0) {
				--this->selectingCategories;
				this->ResetScreen();
			}
		}
		if (inp.R1.down) {
			if (this->selectingCategories < this->categories - 1) {
				++this->selectingCategories;
				this->ResetScreen();
			}
		}

		//B2�Ń`���[�g���A���I��
		if (inp.B2.down) {
			this->finishTutorial = true;
			AppearLoad(10, 3, 20,ML::Color(1.0f,0.5f,0.5f,0.5f));
			return;
		}


		//���͏����e�^�X�N�ɔ��f
		this->reflectInput(this->inputNow,inp);
		
		//�w�莞�ԂɒB�����玟�̓��͂�
		if (this->waitNextInput <= 0 && this->inputCnt < this->inputDataSize - 1) {
			++this->inputCnt;
			this->inputNow += inputData[inputCnt].input;
			this->waitNextInput = inputData[inputCnt].distanceOfNextInput;
		}

		--this->waitNextInput;
		++this->timeCnt;

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 960, 540);
		ML::Box2D src(0, 0, 960, 540);
		this->res->img->Draw(draw, src);
	}

	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�X�N���[���p�̍��W�E�T�C�Y�ɐݒ�
	void Object::SetToScreen(ML::Box2D& drawBox_, ML::Box2D& srcBox_)
	{
		//�X�N���[���̈ʒu�ֈړ�
		drawBox_.x = (int)(drawBox_.x * this->screenScaleRateW);
		drawBox_.y = (int)(drawBox_.y * this->screenScaleRateH);
		drawBox_.Offset(this->screen.x, this->screen.y);
		//�X�N���[���̃T�C�Y�ɍ��킹��
		drawBox_.w = (int)(drawBox_.w * this->screenScaleRateW);
		drawBox_.h = (int)(drawBox_.h * this->screenScaleRateH);
		//�X�N���[��
		ML::Rect s = {
			this->screen.x,
			this->screen.y,
			this->screen.x + this->screen.w,
			this->screen.y + this->screen.h
		};
		//�`��
		ML::Rect d = {
			drawBox_.x,
			drawBox_.y,
			drawBox_.x + drawBox_.w,
			drawBox_.y + drawBox_.h
		};

		if (this->screen.Hit(drawBox_)) {
			//�摜���X�N���[������͂ݏo�Ă���ꍇ�A
			//�o�Ă��镔���͕`�悳���Ȃ�����
			//��
			if (s.left > d.left) {
				drawBox_.x += s.left - d.left;
				drawBox_.w -= s.left - d.left;
				srcBox_.x += (int)((s.left - d.left) / this->screenScaleRateW);	//�͂ݏo���Ă��镔�������̃f�[�^�͈̔͂ɍ��킹�Ă���v�Z
				srcBox_.w -= (int)((s.left - d.left) / this->screenScaleRateW);	
			}
			//��
			if (s.top > d.top) {
				drawBox_.y += s.top - d.top;
				drawBox_.h -= s.top - d.top;
				srcBox_.y += (int)((s.top - d.top) / this->screenScaleRateH);
				srcBox_.h -= (int)((s.top - d.top) / this->screenScaleRateH);
			}
			//�E
			if (s.right < d.right) {
				drawBox_.w -= d.right - s.right;
				srcBox_.w -= (int)((d.right - s.right) / this->screenScaleRateW);
			}
			//��
			if (s.bottom < d.bottom) {
				drawBox_.h -= d.bottom - s.bottom;
				srcBox_.h -= (int)((d.bottom - s.bottom) / this->screenScaleRateH);
			}
		}
		//���S�ɃX�N���[���O�ɏo�Ă�����T�C�Y��0��
		else {
			drawBox_.w = 0;
			drawBox_.h = 0;
		}
	}

	//���͏��̓ǂݍ���
	void Object::LoadInputData()
	{
		ifstream fin(this->FolderPath() + "InputData.txt");
		if (!fin) { return; }

		//�z����Đݒ�
		delete[] this->inputData;
		fin >> this->inputDataSize;
		this->inputData = new InputData[this->inputDataSize];
		
		//���͏��ǂݍ���
		for (int i = 0; i < this->inputDataSize; ++i) {
			string inp;
			fin >> inp;
			//�uDIS�v��������܂œ��͓ǂ݂���
			while (inp != "DIS")
			{
				if (!fin) { 
					this->inputDataSize = i;
					return; 
				}
				this->CheckInput(i, inp, fin);
				fin >> inp;
			}

			fin >> this->inputData[i].distanceOfNextInput;
		}
		fin.close();
	}

	//���ږ��̓ǂݍ���
	void Object::LoadCategoryName()
	{
		this->categoryName = new string[this->categories];
		//���ݗp�ӂ��Ă��邷�ׂĂ̍��ږ���ǂݍ���
		for (int i = 0; i < this->categories; ++i) {
			string fPath = "./data/Tutorial/Category" + to_string(i) + "/";
			ifstream fin(fPath + "Name.txt");
			fin >> categoryName[i];
			fin.close();
		}
	}

	//�ǂ̃{�^���̓��͂�
	void Object::CheckInput(int cnt_, const string& inp_,ifstream& fin_)
	{
		if (inp_ == "LBR") {
			fin_ >> this->inputData[cnt_].input.LStickBR;
		}
		else if (inp_ == "LBL") {
			fin_ >> this->inputData[cnt_].input.LStickBL;
		}
		else if (inp_ == "B1") {
			fin_ >> this->inputData[cnt_].input.B1;
		}
		else if (inp_ == "B3") {
			fin_ >> this->inputData[cnt_].input.B3;
		}
		else if (inp_ == "RT") {
			fin_ >> this->inputData[cnt_].input.RT;
		}
	}

	//���͏��̔��f
	void Object::reflectInput(KeyInput& in_,XI::VGamePad& inp_)
	{
		//�t���O���Z�b�g
		//�v���C���[
		this->tutoPlayer->moveLRFlag = false;
		this->tutoPlayer->dashFlag = false;
		this->tutoPlayer->attackFlag = false;
		this->tutoPlayer->jumpFlag = false;
		//�L�[���͕\��
		for (auto& key : *this->keys) {
			key->pushing = false;
			key->playerInput = false;
		}
		//�X�e�B�b�N���͕\��
		this->keyStick->movingStick = this->keyStick->Angle::NON;
		this->keyStick->playerInput = this->keyStick->Angle::NON;


		//�f�[�^��ɂ�����͏��[�[�[�[�[�[�[�[�[
		//�E�ړ�
		if (in_.LStickBL >= 1) {
			this->tutoPlayer->ctrlAng = BChara::Angle_LR::Left;
			this->tutoPlayer->moveLRFlag = true;
			this->keyA->pushing = true;
			this->keyStick->movingStick = this->keyStick->Angle::LEFT;
		}
		//���ړ�
		if (in_.LStickBR >= 1) {
			this->tutoPlayer->ctrlAng = BChara::Angle_LR::Right;
			this->tutoPlayer->moveLRFlag = true;
			this->keyD->pushing = true;
			this->keyStick->movingStick = this->keyStick->Angle::RIGHT;
		}
		//�_�b�V��
		if (in_.B3 >= 1) {
			this->tutoPlayer->dashFlag = true;
			this->keyB3->pushing = true;
			this->keyShift->pushing = true;
		}
		//�W�����v
		if (in_.B1 >= 1) {
			this->tutoPlayer->jumpFlag = true;
			this->keyB1->pushing = true;
			this->keyM->pushing = true;
		}
		//�A�^�b�N
		if (in_.RT >= 1) {
			this->tutoPlayer->attackFlag = true;
			this->keyRT->pushing = true;
			this->keyL->pushing = true;
		}

		//�v���C���[�̓��͏��[�[�[�[�[�[�[�[�[
		//�E�ړ�
		if (inp_.LStick.BR.on) {
			this->keyD->playerInput = true;
			this->keyStick->playerInput = this->keyStick->Angle::RIGHT;
		}
		if (inp_.LStick.BL.on) {
			this->keyA->playerInput = true;
			this->keyStick->playerInput = this->keyStick->Angle::LEFT;
		}
		//�W�����v
		if (inp_.B1.on) {
			this->keyB1->playerInput = true;
			this->keyM->playerInput = true;
		}
		//�_�b�V��
		if (inp_.B3.on) {
			this->keyB3->playerInput = true;
			this->keyShift->playerInput = true;
		}
		//�A�^�b�N
		if (inp_.Trigger.volumeR != 0) {
			this->keyRT->playerInput = true;
			this->keyL->playerInput = true;
		}
		//���g�̌��Z����
		in_.DecrementAll();

	}

	//�L�[�̐���
	void Object::CreateKey(TutorialKeyDisplay::Object::SP& key_, int srcOfsY_, float posX_, float posY_)
	{
		//����
		key_ = TutorialKeyDisplay::Object::Create(true);
		//�`�悷��摜�̕ύX
		key_->srcBase.y = srcOfsY_ * key_->imgSizeH;
		//�ʒu�̐ݒ�
		key_->pos.x = posX_;
		key_->pos.y = posY_;
	}

	//���ڐ؂�ւ����̃G�t�F�N�g�\��
	void Object::AppearSwitchEffect()
	{
		auto effect = Effect05::Object::Create(true);
		effect->Set(480, 270, 100);
		effect->pos = ML::Vec2(690, 235);
	}

	//���{�v���C���[���Z�b�g
	void Object::ResetPlayer()
	{
		this->LoadInputData();
		this->inputNow = this->inputData[0].input;
		this->waitNextInput = this->inputData[0].distanceOfNextInput;
		this->inputCnt = 0;

		this->tutoPlayer->Kill();
		this->tutoPlayer = TutorialPlayer::Object::Create(true);
		this->tutoPlayer->pos = this->dataForPlay.playerStartPos;
	}

	//�X�N���[�����ƃ��Z�b�g
	void Object::ResetScreen()
	{
		//�G�t�F�N�g���c���Ă����炢������L��
		ge->KillAll_GN("�G�t�F�N�g", "05");
		//�G������ꍇ���L��
		ge->KillAll_G("�G");
		//���͏��
		this->dataForPlay.Load(this->FolderPath() + "tutorialMap.txt");
		//�}�b�v
		ge->qa_Map->Load(this->FolderPath() + "tutorialMap.txt");
		//���{
		this->ResetPlayer();
		//�v���C���[�ʒu
		this->player->Kill();
		this->player = Player::Object::Create(true);
		this->player->pos = this->dataForPlay.playerStartPos;
		//�G�t�F�N�g
		this->AppearSwitchEffect();
	}

	//���ݑI�𒆂̍��ڂւ̃t�H���_�p�X��Ԃ�
	string Object::FolderPath()
	{
		return "./data/Tutorial/Category" + to_string(this->selectingCategories) + "/";
	}

	//�`���[�g���A���I������
	void Object::FinishTask()
	{
		//�`���[�g���A���Ɏg�p�����^�X�N�̃L��
		//�v���C���[
		this->player->Kill();
		//�G		���ݖ�����Ԃł͂Ȃ����́i�`���[�g���A���Ŏg�p���Ă������́j�̂ݏ���											
		auto enemys = ge->GetTasks<BChara>("�G");				
		if (nullptr != enemys) {
			for (auto& it : *enemys) {
				if (it->TaskStateCnt_Suspend() == 0) {
					it->Kill();
				}
			}
		}
		//�}�b�v
		ge->qa_Map->Kill();
		//�G�t�F�N�g
		auto effects = ge->GetTasks<BTask>("�G�t�F�N�g");
		if (nullptr != effects) {
			for (auto& it : *effects) {
				if (it->TaskStateCnt_Suspend() == 0) {
					it->Kill();
				}
			}
		}
		//�U��
		auto attacks = ge->GetTasks<BChara>("�U��");
		if (nullptr != attacks) {
			for (auto& it : *attacks) {
				if (it->TaskStateCnt_Suspend() == 0) {
					it->Kill();
				}
			}
		}
		//�Q�[��UI
		auto uis = ge->GetTasks<BTask>("�Q�[��UI");
		if (nullptr != uis) {
			for (auto& it : *uis) {
				if (it->TaskStateCnt_Suspend() == 0) {
					it->Kill();
				}
			}
		}
		//�Q�[���I�[�o�[�V���h�E
		auto shadow = ge->GetTask<BTask>("�Q�[���I�[�o�[�V���h�E");
		if (nullptr != shadow) {
			if (shadow->TaskStateCnt_Suspend() == 0) {
				shadow->Kill();
			}
		}

		//�Q�Ƃ̏�Ԃ����ɖ߂�
		ge->qa_Player = this->qaPlayerMemory;
		ge->qa_Map = this->qaMapMemory;
		ge->qa_Enemys = this->qaEnemyMemory;
		ge->retryFlag = this->retryFlagMemory;
		ge->gameOverflag = this->gameOverFlagMemory;

		//�`���[�g���A���t���O���I�t��
		ge->inTutorial = false;

		//�`���[�g���A���̃L��
		ge->KillAll_G("�`���[�g���A��");
		ge->KillAll_GN("�v���C��", "�`���[�g���A��");

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