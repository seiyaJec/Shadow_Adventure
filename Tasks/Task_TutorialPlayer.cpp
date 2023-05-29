//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_TutorialPlayer.h"

namespace  TutorialPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgLeft = DG::Image::Create("./data/image/Player/PlayerL.png");
		this->imgRight = DG::Image::Create("./data/image/Player/PlayerR.png");
		this->imgShadowLeft = DG::Image::Create("./data/image/Player/PlayerShadowL.png");
		this->imgShadowRight = DG::Image::Create("./data/image/Player/PlayerShadowR.png");
		LoadAnim(this->animIdle, "./data/animation/player/Idle.txt");
		LoadAnim(this->animRunning, "./data/animation/player/Running.txt");
		LoadAnim(this->animJumping, "./data/animation/player/Jumping.txt");
		LoadAnim(this->animDoubleJump, "./data/animation/player/DoubleJump.txt");
		LoadAnim(this->animOnWall, "./data/animation/player/OnWall.txt");
		LoadAnim(this->animDamaged, "./data/animation/player/Damaged.txt");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgRight.reset();
		this->imgLeft.reset();
		this->imgShadowLeft.reset();
		this->imgShadowRight.reset();
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
		this->render2D_Priority[1] = 0.5f;
		//��`
		this->hitBase = ML::Box2D(-15, -21, 30, 58);
		this->drawBase = ML::Box2D(-50, -37, 100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		//�F
		this->color = ML::Color(0.5f, 1, 1, 1);
		//�A�j���[�V����
		this->animState = AnimState::IDLE;
		//�X�e�[�^�X
		this->statHPMax = 50;
		this->statHP = this->statHPMax;
		//�U���֌W
		this->attackTime = 0;
		this->attackCnt = 0;
		//�J�����֌W
		this->cameraShakeTime = 0;
		this->cameraShakePosMax.x = 0;
		this->cameraShakePosMax.y = 0;
		//�ړ�
		this->walkSpeed = 3;
		this->dashRate = 1.8f;
		this->firstJump = false;
		this->firstJumpTime = 10;
		this->inputJump = 99999;
		this->jumpPow = -8;
		this->wallJumpPow = -6;
		this->doubleJumpPow = -7;
		this->fallSpeedMax = 10;
		//����
		this->hitFlagFront = false;
		this->hitFlagHead = false;
		//��_���[�W�֌W
		this->damagedStopMove = false;
		this->damagedCount = 0;
		this->damagedCntMax = 100;	//���G����
		this->damagedVecMax = ML::Vec2(2, -3);

		//����֌W
		this->ctrlAng = Angle_LR::Right;
		this->moveLRFlag = false;
		this->dashFlag = false;
		this->attackFlag_T = false;
		this->pushJumpCnt = 0;
		this->attackNum = 0;
		this->tutorialMoveCnt = 0;
		this->imgSwap = false;
		this->tutorialAnimation = 0;
		this->jumpFlag = false;
		this->preAttack = false;

		//�f�o�b�O
		this->debugFlag = false;
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

		//�ړ��f�[�^������
		ML::Vec2  est(0, 0);
		this->animState = AnimState::IDLE;


		//�_���[�W���󂯂Ă��Ȃ���
		if (false == this->damagedStopMove) {


			//���ړ�
			if (this->moveLRFlag == true) {
				MoveLR(this->ctrlAng, est);
			}




			//�ǃL�b�N���̏���
			this->MoveWallJump(est);

			//�������x�̐���
			if (this->fallSpeed > this->fallSpeedMax) {
				this->fallSpeed = this->fallSpeedMax;
			}
			//��������
			est.y += this->fallSpeed;

			//�߂荞�܂Ȃ��ړ�����
			this->CheckMove(est);

			//���n����(�߂荞��łȂ����m�F������ɍs���j
			this->hitFlagFoot = this->CheckFoot();
			//�ǐڐG����
			this->hitFlagFront = this->CheckFront();
			//���ڐG����
			this->hitFlagHead = this->CheckHead();


			//�W�����v�{�^���������ꂽ��\�����͂Ƃ��Ď󂯕t����
			if (true == this->jumpFlag && this->inputJump == 99999) {
				this->inputJump = 0;
			}
			else if (true == this->jumpFlag) {
				++this->inputJump;

			}
			else {
				this->inputJump = 99999;
			}

			//�W�����v
			this->Jump();
			//��i�W�����v
			if (true == this->IsCanDoubleJump()) {
				this->DoubleJump();
			}
			//�Ǌ֌W
			this->WallAction(this->ctrlAng);
			//���n����
			this->IsOnGround();
			//�V�䏈��
			this->IsOnCeiling();

			//�U��
			if (true == this->attackFlag) {
				if (false == this->preAttack) {
					this->Attack();
					this->preAttack = true;
				}
				this->attackFlag = false;
			}
			else {
				this->preAttack = false;
			}



		}
		//�_���[�W���󂯂Ă���ꍇ
		else {
			this->DamagedMove(est);
		}

		//��_���[�W���̏���
		if (true == this->damaged) {
			if (this->damagedCount > damagedCntMax) {
				this->damaged = false;
			}
			++this->damagedCount;
		}


		++this->tutorialMoveCnt;
		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		//�A�j���[�V�����ɍ��킹�ĕ`���`��ݒ�
		this->Anim(draw);
		ML::Box2D src = this->srcBase;
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//�F
		ML::Color color = this->color;
		//�`���[�g���A�����͕`���ύX
		DrawSetIfTuto(draw, src);
		//�����ɂ���Ďg���摜��ύX
		if (false == this->imgSwap) {
			if (this->angle_LR == Angle_LR::Left) {
				this->res->imgLeft->Draw(draw, src, color);
			}
			else {
				this->res->imgRight->Draw(draw, src, color);
			}
		}
		else {
			if (this->angle_LR == Angle_LR::Left) {
				this->res->imgShadowLeft->Draw(draw, src, color);
			}
			else {
				this->res->imgShadowRight->Draw(draw, src, color);
			}
		}

		//�f�o�b�O�p��`�\��
		this->DebugDrawBox(this->hitBase);
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------

	//�A�j���[�V����
	void Object::Anim(ML::Box2D& drawBox_)
	{
		int animNum = 0;
		switch (this->animState) {

			//�Î~
		case AnimState::IDLE:
			AnimSet(this->res->animIdle, this->animCnt / 10);
			break;

			//����
		case AnimState::RUNNING:
			AnimSet(this->res->animRunning, this->animCnt / 10);
			MovePosToAngle(drawBox_, -7, 0);
			break;

			//�W�����v
		case AnimState::JUMPING:
			if (this->fallSpeed <= -4) {
				animNum = 0;
			}
			else if (this->fallSpeed <= 0) {
				animNum = 1;
			}
			else if (this->fallSpeed <= 5) {
				animNum = 2;
			}
			else {
				animNum = 3;
			}
			AnimSet(this->res->animJumping, animNum);
			if (animNum == 3) {
				MovePosToAngle(drawBox_, 0, 6);
			}
			break;
		case AnimState::DOUBLEJUMP:
			AnimSet(this->res->animDoubleJump, this->animCnt / 10);
			break;
			//�ǂɂ�
		case AnimState::ONWALL:
			AnimSet(this->res->animOnWall, this->animCnt / 10);
			break;
			//�_���[�W���󂯂�
		case AnimState::DAMAGED:
			AnimSet(this->res->animDamaged, this->animCnt / 10);
		default:
			break;
		}
	}
	//��ʂ�h�炷
	void Object::StartCameraShake(int time_, int interval_, int xMax_, int yMax_)
	{
		this->cameraShakeTime = time_;
		this->cameraShakeInterval = interval_;
		ge->camera2DMovePos.x = xMax_;
		ge->camera2DMovePos.y = yMax_;
	}
	//���ړ�
	void Object::MoveLR(Angle_LR ang_, ML::Vec2& est_)
	{
		if (ang_ == Angle_LR::Left) {
			if (false == this->dashFlag) {
				est_.x -= this->walkSpeed;
			}
			else {
				est_.x -= this->walkSpeed * this->dashRate;
				this->dashFlag = false;
			}
			this->angle_LR = Angle_LR::Left;
			this->animState = AnimState::RUNNING;
		}
		//���ړ��i�E�j
		if (ang_ == Angle_LR::Right) {
			if (false == this->dashFlag) {
				est_.x += this->walkSpeed;
			}
			else {
				est_.x += this->walkSpeed * this->dashRate;
				this->dashFlag = false;
			}
			this->angle_LR = Angle_LR::Right;
			this->animState = AnimState::RUNNING;
		}
	}
	//�ǃL�b�N���̈ړ�
	void Object::MoveWallJump(ML::Vec2& est_)
	{
		if (true == this->isWallJump) {
			if (this->moveCnt > 0) {
				est_.x = this->moveVec.x;
				this->angle_LR = wallJumpAngle;
			}
			else {
				this->isWallJump = false;
			}
			--this->moveCnt;
		}
	}
	//�W�����v���菈��
	void Object::Jump()
	{
		if (this->inputJump < inputJumpTimeMax) {
			if (true == this->hitFlagFoot) {
				this->fallSpeed = this->jumpPow;
				this->firstJump = true;
				this->inputJump = 0;
				this->attackFlag = false;
			}
		}
		else if (firstJump == true) {
			if (this->inputJump < firstJumpTime) {
				this->fallSpeed = this->jumpPow;
			}
			else {
				this->firstJump = false;
				this->fallSpeed = -4;
			}
		}


	}

	//��i�W�����v�\��
	bool Object::IsCanDoubleJump()
	{
		if (this->inputJump >= inputJumpTimeMax) {
			return false;
		}
		if (true == this->firstJump) {
			return false;
		}
		if (true == this->doubleJump) {
			return false;
		}
		if (true == this->hitFlagFoot) {
			return false;
		}
		if (true == this->isWallJump) {
			return false;
		}
		return true;
	}
	//��i�W�����v���菈��
	void Object::DoubleJump()
	{
		this->fallSpeed = this->doubleJumpPow;
		this->doubleJump = true;
		this->attackFlag = false;
	}
	//�Ǎۂ̏���
	void Object::WallAction(Angle_LR ang_)
	{
		if (true == this->hitFlagFront) {//�Ǎۂɂ��邩
			if (false == this->hitFlagFoot) {//�󒆂ɂ��邩
				if (false == this->attackFlag) {//�U��������Ȃ���
					if (false == firstJump) {
						//������
						if (ang_ == Angle_LR::Left) {
							//��������
							this->WallFaling();
							//�W�����v�{�^���ŕǃW�����v
							if (this->inputJump <= inputJumpTimeMax) {
								this->WallJumpStart(Angle_LR::Right);		//��ԕ������w��
							}
						}
						//�E����
						if (ang_ == Angle_LR::Right) {
							//��������
							this->WallFaling();
							//�W�����v�{�^���ŕǃW�����v
							if (this->inputJump <= inputJumpTimeMax) {
								this->WallJumpStart(Angle_LR::Left);	//��ԕ������w��
							}
						}
					}
				}
			}
		}
	}
	//�ǂł̗�������
	void Object::WallFaling()
	{
		//�������̏���
		if (this->fallSpeed > this->fallSpeedMaxOnWall) {
			//�������x�𐧌�����
			this->fallSpeed = this->fallSpeedMaxOnWall;
			//�A�j���[�V������ύX
			this->animState = AnimState::ONWALL;
		}
	}
	//�ǃW�����v
	void Object::WallJumpStart(Angle_LR ang_)
	{
		//������ݒ�
		this->wallJumpAngle = ang_;

		//�W�����v���̉��ړ��̐ݒ�
		if (wallJumpAngle == Angle_LR::Right) {
			this->moveVec.x = this->walkSpeed * this->dashRate;
		}
		else {
			this->moveVec.x = -this->walkSpeed * this->dashRate;
		}
		//�ǃW�����v�����J�n�̍��}
		this->isWallJump = true;

		//��莞�ԑ�����Œ�
		this->moveCnt = this->wallJumpTime;

		//�W�����v�̍�����ݒ�
		this->fallSpeed = this->wallJumpPow;

	}
	//�n�ʂɂ��邩
	void Object::IsOnGround()
	{
		if (true == this->hitFlagFoot) {
			if (this->fallSpeed > 0) {
				if (this->fallSpeed > fallSpeedMax / 2) {
					auto smoke = Effect01::Object::Create(true);
					smoke->pos += this->pos;
				}
				this->fallSpeed = 0; //�������x0
				this->doubleJump = false;
			}
		}
		else {
			this->fallSpeed += ML::Gravity(32) * 3;
			if (this->animState != AnimState::ONWALL) {
				this->animState = AnimState::JUMPING;
			}
		}
	}
	//�V��ɐG��Ă��邩
	void Object::IsOnCeiling()
	{
		if (true == this->hitFlagHead) {
			if (this->fallSpeed <= -1) {
				this->fallSpeed *= -0.5f;
			}
			this->firstJump = false;
		}
	}
	//�U��
	void Object::Attack()
	{
		//�A�j���[�V�����̕ύX
		switch (this->animState) {
			//�n�ʂɂ���Ƃ�
		case AnimState::IDLE:
		{
			auto atk = Attack00::Object::Create(true);
			atk->color = this->color;
			this->AttackInitialize(atk);
			break;
		}
		//�����Ă���Ƃ�
		case AnimState::RUNNING:
		{
			auto atk = Attack01::Object::Create(true);
			atk->color = this->color;
			this->AttackInitialize(atk);
			break;
		}
		//�󒆂ɂ���Ƃ�
		case AnimState::JUMPING:
		case AnimState::ONWALL:
		{
			auto atk = Attack02::Object::Create(true);
			atk->color = this->color;
			this->AttackInitialize(atk);
			atk->fallSpeedMax = this->fallSpeedMax;
			atk->moveVec.x = 4.0f * Angle_LR_ChangeAngleToInt(this->angle_LR);
			break;
		}
		}
	}

	//�U��
	void Object::AttackInitialize(BChara::SP atk_)
	{
		atk_->angle_LR = this->angle_LR;
		atk_->pos = this->pos;
		//�����蔻����Ȃ���
		atk_->hitBase = ML::Box2D(0, 0, 0, 0);
		atk_->attackBase = ML::Box2D(0, 0, 0, 0);
	}

	//��_���[�W����
	void Object::DamagedMove(ML::Vec2& est_)
	{
		this->animState = AnimState::DAMAGED;
		//�������x�̐���
		if (this->moveVec.y > this->fallSpeedMax) {
			this->moveVec.y = this->fallSpeedMax;
		}
		//�ړ�
		this->moveVec.x -= 0.1f * Angle_LR_ChangeAngleToInt(this->damagedAngle_LR);
		//��������
		this->moveVec.y += ge->gravity;
		est_ = this->moveVec;

		this->CheckMove(est_);

		//�ړ����Ȃ��Ȃ�����I������
		if (true == this->IsZeroDamagedMove()) {
			this->damagedStopMove = false;
			this->fallSpeed = this->moveVec.y;
			return;
		}
	}

	//-------------------------------------------------------------------
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