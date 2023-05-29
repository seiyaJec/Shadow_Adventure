//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_Player.h"
#include  "Task_GameUIGoalDisplay.h"
#include  "Task_GameOverShadow.h"

namespace  Player
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
		LoadAnim(this->animDeath, "./data/animation/player/Death.txt");
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
		//�Q�Ɨp
		ge->qa_Player = ge->GetTask<Player::Object>("�v���C��", "��");
		//�R���g���[���[
		this->controller = ge->in1;
		//�摜�D��x
		this->render2D_Priority[1] = 0.5f;
		//��`
		this->hitBase = ML::Box2D(-15, -21, 30, 58);
		this->drawBase = ML::Box2D(-50, -37, 100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		this->ChkWSpikeDecrease = 22;
		//�A�j���[�V����
		this->animState = AnimState::IDLE;
		//�X�e�[�^�X
		this->statHPMax = 50;
		this->statHP = this->statHPMax;
		//�U���֌W
		this->attackTime = 0;
		this->attackCnt = 0;
		this->attacked = false;
		//�J�����֌W
		this->cameraShakeTime = 0;
		this->cameraShakePosMax.x = 0;
		this->cameraShakePosMax.y = 0;
		//�ړ�
		this->walkSpeed = 3;
		this->dashRate = 1.8f;
		this->firstJump = false;
		this->firstJumpTime = 10;
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
		//���S�֌W
		this->deathCount = 0;
		this->deathCountMax = 60;
		this->deathDisappearSpeed = 30;
		//���g���C
		this->retryAppear = false;
		this->retryPos = ML::Vec2(0, 0);
		this->retryAppearSpeed = 10;
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
		//�R���g���[���[���̎擾
		auto inp = this->controller->GetState();

		//�ړ��f�[�^������
		ML::Vec2  est(0, 0);
		this->animState = AnimState::IDLE;

		//�S�[�������Ȃ珈�����Ȃ�
		if (true == ge->goalFlag) {
			++this->animCnt;
			return;
		}

		//�Q�[���I�[�o�[�̎����ړ�����������
		if (true == ge->gameOverflag) {
			this->DeathUpDate();
			return;
		}

		//���g���C�����s���ꂽ��
		if (true == this->retryAppear) {
			this->animState = AnimState::JUMPING;
			this->fallSpeed = 6;
			this->pos.y += this->retryAppearSpeed;
			if (this->pos.y >= this->retryPos.y) {
				this->pos.y = this->retryPos.y;
				this->retryAppear = false;
			}
			return;
		}

		//�_���[�W���󂯂Ă��Ȃ���
		if (false == this->damagedStopMove) {
			//�ړ�����
			this->MoveLR(inp, est);
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

			//�S�[������
			ge->qa_Map->CheckHit(this->hitBase.OffsetCopy(this->pos));
			if (true == ge->qa_Map->foundGoal) {
				GameUIGoalDisplay::Object::Create(true);
				ge->goalFlag = true;
				return;
			}
			//�������S����
			ML::Box2D pHit = this->hitBase.OffsetCopy(this->pos);
			if (pHit.y > ge->qa_Map->hitBase.y + ge->qa_Map->hitBase.h) {
				this->statHP = 0;
				ge->gameOverflag = true;
			}

			//���n����(�߂荞��łȂ����m�F������ɍs���j
			this->hitFlagFoot = this->CheckFoot();
			CheckSpike();

			//�W�����v�{�^���������ꂽ��\�����͂Ƃ��Ď󂯕t����
			if (inp.B1.down) {
				this->inputJump = 0;
			}
			else if (inp.B1.on) {
				++this->inputJump;

			}
			else {
				this->inputJump = 99999;
			}

			//�W�����v
			this->Jump();
			//��i�W�����v
			if (true == this->IsCanDoubleJump(inp)) {
				this->DoubleJump();
			}
			//�Ǌ֌W
			this->hitFlagFront = this->CheckFront();	//�����͕Ǐ������ɐj���肷��
			WallAction(inp);
			//���n����
			this->IsOnGround();
			//�V�䏈��
			this->hitFlagHead = this->CheckHead();
			CheckSpike();
			this->IsOnCeiling();
			//�U���J�n
			this->Attack(inp);
		}
		//�_���[�W���󂯂Ă���ꍇ
		else {
			this->DamagedMove(est);
		}

		//�J�����̈ʒu���Ē���
		{
			//�v���C������ʂ̂ǂ��ɒu�����i����͉�ʒ����j
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h / 2;
			//�v���C������ʒ����ɒu�������̃J�����̍�����W�����߂�
			int cpx = int(this->pos.x) - px;
			int cpy = int(this->pos.y) - py;
			//�J�����̍��W���X�V
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
			//�}�b�v�̊O�����f��Ȃ��悤�ɃJ�����𒲐�����
			ge->qa_Map->AdjustCameraPos();

		}
		//�ړ��̎w��������Ύ��s����
		if (this->cameraShakeTime > 0) {
			if (this->cameraShakeTime % this->cameraShakeInterval == 0) {
				ge->camera2DMovePos.x = -ge->camera2DMovePos.x;
				ge->camera2DMovePos.y = -ge->camera2DMovePos.y;
			}
			ge->camera2D.x += ge->camera2DMovePos.x;
			ge->camera2D.y += ge->camera2DMovePos.y;

			--this->cameraShakeTime;
		}

		//��_���[�W���̏���
		if (true == this->damaged) {
			if (this->damagedCount > damagedCntMax) {
				this->damaged = false;
			}
			++this->damagedCount;
		}



		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		this->Anim(draw);
		ML::Box2D src = this->srcBase;
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//�`���[�g���A�����͕`���ύX
		DrawSetIfTuto(draw,src);
		//�F�ݒ�
		ML::Color color(1, 1, 1, 1);
		//�_���[�W���ɓ_��
		if (true == this->damaged) {
			if (this->damagedCount % 10 == 0) {
				color = ML::Color(0.2f, 1, 1, 1);
			}
		}
		//�����ɂ���Ďg���摜��ύX
		if (this->angle_LR == Angle_LR::Left) {
			this->res->imgLeft->Draw(draw, src, color);
		}
		else {
			this->res->imgRight->Draw(draw, src, color);
		}
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�A�j���[�V����
	void Object::Anim(ML::Box2D& draw_)
	{
		int animNum = 0;
		switch (this->animState) {

			//�Î~
		case AnimState::IDLE:
			AnimSet(this->res->animIdle, this->animCnt / 10);
			draw_.y += 3;
			break;

			//����
		case AnimState::RUNNING:
			AnimSet(this->res->animRunning, this->animCnt / 10);
			MovePosToAngle(draw_, -7, 3);
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
				MovePosToAngle(draw_, 0, 6);
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
			break;
		case AnimState::DEATH:
			AnimSet(this->res->animDeath, this->animCnt / 10);
			break;
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
	void Object::MoveLR(XI::VGamePad inp_, ML::Vec2& est_)
	{
		if (inp_.LStick.BL.on) {
			if (inp_.B3.on) {
				est_.x -= this->walkSpeed * this->dashRate;
			}
			else {
				est_.x -= this->walkSpeed;
			}
			this->angle_LR = Angle_LR::Left;
			this->animState = AnimState::RUNNING;
		}
		//���ړ��i�E�j
		if (inp_.LStick.BR.on) {
			if (inp_.B3.on) {
				est_.x += this->walkSpeed * this->dashRate;
			}
			else {
				est_.x += this->walkSpeed;
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
		//�W�����v�{�^�����������̏���
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

	bool Object::IsCanDoubleJump(XI::VGamePad inp_)
	{
		if (this->inputJump >= inputJumpTimeMax) {
			return false;
		}
		if (true == this->hitFlagFront) {
			if (true == inp_.LStick.BR.on) {
				return false;
			}
			if (true == inp_.LStick.BL.on) {
				return false;
			}
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
	void Object::WallAction(XI::VGamePad inp_)
	{
		if (true == this->hitFlagFront) {//�Ǎۂɂ��邩
			if (false == this->hitFlagFoot) {//�󒆂ɂ��邩
				if (false == this->attackFlag) {//�U��������Ȃ���
					if (false == firstJump) {//���ڂ̃W�����v����������Ȃ���
						if (false == this->isWallJump) {//�ǃW�����v������Ȃ���
							//������
							if (inp_.LStick.BL.on) {
								//�j���������Ȃ珈�����Ȃ�
								if (true == this->CheckWallSpike()) {
									this->CheckSpike();
									return;
								}
								//��������
								this->WallFaling();
								//�W�����v�{�^���ŕǃW�����v
								if (this->inputJump <= inputJumpTimeMax) {
									this->WallJumpStart(Angle_LR::Right);		//��ԕ������w��
								}
								return;
							}
							//�E����
							if (inp_.LStick.BR.on) {
								//�j���������珈�����Ȃ�
								if (true == this->CheckWallSpike()) {
									this->CheckSpike();
									return;
								}
								//��������
								this->WallFaling();
								//�W�����v�{�^���ŕǃW�����v
								if (this->inputJump <= inputJumpTimeMax) {
									this->WallJumpStart(Angle_LR::Left);	//��ԕ������w��
								}
								return;
							}
						}
						else {
							if (this->inputJump <= inputJumpTimeMax) {
								auto ang = this->angle_LR;
								this->Angle_LR_Reverse(ang);
								this->WallJumpStart(ang);
								return;
							}
						}
					}
				}
			}
		}
		//�ǂɌ������ĕ����Ă�����j���m
		if (inp_.LStick.BL.on || inp_.LStick.BR.on) {
			this->CheckSpike();
		}
	}

	//�ǂ̂Ƃ�����
	bool Object::CheckWallSpike()
	{
		//�����蔻������Ƃɂ��Đ��ʋ�`�𐶐�
		ML::Box2D front(
			this->hitBase.x,
			this->hitBase.y + this->ChkWSpikeDecrease,
			1,
			this->hitBase.h - this->ChkWSpikeDecrease * 2
		);
		//�L�����̌����ɂ���Ĕ��肷��ʒu��ύX
		if (this->angle_LR == Angle_LR::Right) {
			front.x += this->hitBase.w;
		}
		else {
			front.x += -1;
		}
		front.Offset(this->pos);

		if (nullptr == ge->qa_Map) { return false; }//�}�b�v���Ȃ���Δ��肵�Ȃ��i�ł��Ȃ��j
		//�O�ʂɐj������Ƃ��͔��肷��
		if (true == ge->qa_Map->foundSpike) {
			//����ʒu�ɕǂ����邩
			bool iswall = ge->qa_Map->CheckHit(front);
			return !iswall || ge->qa_Map->foundSpike;
		}
		return false;
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
	void Object::Attack(XI::VGamePad inp_)
	{
		if (false == this->attacked) {
			if (inp_.Trigger.volumeR != 0) {
				//�A�j���[�V�����̕ύX
				switch (this->animState) {
					//�n�ʂɂ���Ƃ�
				case AnimState::IDLE:
				{
					auto atk = Attack00::Object::Create(true);
					this->AttackInitialize(atk);
					atk->MovePosToAngle(atk->attackBase, 22, -5);
					break;
				}
				//�����Ă���Ƃ�
				case AnimState::RUNNING:
				{
					auto atk = Attack01::Object::Create(true);
					this->AttackInitialize(atk);
					atk->MovePosToAngle(atk->attackBase, 2, 10);
					break;
				}
				//�󒆂ɂ���Ƃ�
				case AnimState::JUMPING:
				case AnimState::ONWALL:
				{
					auto atk = Attack02::Object::Create(true);
					this->AttackInitialize(atk);
					atk->fallSpeedMax = this->fallSpeedMax;
					if (inp_.RStick.BR.on) {
						atk->angle_LR = Angle_LR::Right;
					}
					else  if (inp_.LStick.BL.on) {
						atk->angle_LR = Angle_LR::Left;
					}
					atk->moveVec.x = 4.0f * Angle_LR_ChangeAngleToInt(this->angle_LR);
					break;
				}
				}
				this->attacked = true;
			}
		}
		else if (inp_.Trigger.volumeR == 0) {
			this->attacked = false;
		}
	}
	void Object::AttackInitialize(BChara::SP atk_) 
	{
		atk_->angle_LR = this->angle_LR;
		atk_->pos = this->pos;
	}
	//----------------------------------------------------------------------------------
	//�v���C���[�Ƀ_���[�W
	void Object::DamageToMe(Angle_LR ang_, int damage_)
	{
		//�v���C���[
		this->statHP -= damage_;
		//HP��0�Ȃ�Q�[���I�[�o�[
		if (this->statHP <= 0) {
			ge->gameOverflag = true;
		}
		else {
			this->damaged = true;
			this->damagedStopMove = true;
			this->damagedCount = 0;
			//�v���C���[���G�̉E���ɂ���ꍇ
			this->damagedAngle_LR = ang_;
			//�v���C���[������΂�����
			this->moveVec.x = ge->qa_Player->damagedVecMax.x * Angle_LR_ChangeAngleToInt(ge->qa_Player->damagedAngle_LR);
			this->moveVec.y = ge->qa_Player->damagedVecMax.y;
			this->StartCameraShake(6, 2, 6, 6);
		}
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
	//�j�_���[�W����i�}�b�v�����蔻��̒���łȂ��ƌ��ʂȂ��j
	bool Object::CheckSpike()
	{
		if (true == this->damaged) {
			return false;
		}
		if (true == ge->qa_Map->foundSpike) {
			Angle_LR reverseAngle = this->angle_LR;
			Angle_LR_Reverse(reverseAngle);
			this->DamageToMe(reverseAngle, 20);
			return true;
		}
		return false;
	}

	//���S���̏���
	void Object::DeathUpDate()
	{
		this->animState = AnimState::DEATH;
		if (this->deathCount == 0) {
			this->animCnt = 0;
			GameOverShadow::Object::Create(true);
		}
		if (this->animCnt >= this->deathCountMax - 1) {
			//�A�j���[�V�����Ō�̉摜���p��������
			this->animCnt = this->deathCountMax -1;	

			//���S�����ł̏���
			this->pos.y -= deathDisappearSpeed;

			//�v���C���[�����S�ɏ�������L��
			if (this->pos.y <= ge->camera2D.y - this->drawBase.h) {
				ge->retryFlag = true;
			}
		}
		else {
			++this->animCnt;
		}
		++this->deathCount;
	}

	void Object::SetRetryPos(const ML::Vec2& pos_)
	{
		this->retryPos = pos_;
		this->pos = pos_;
		this->pos.y -= ge->screen2DHeight;
		//�J�����̈ʒu���Ē���
		{
			//�v���C������ʂ̂ǂ��ɒu�����i����͉�ʒ����j
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h / 2;
			//�v���C������ʒ����ɒu�������̃J�����̍�����W�����߂�
			int cpx = int(pos_.x) - px;
			int cpy = int(pos_.y) - py;
			//�J�����̍��W���X�V
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
			//�}�b�v�̊O�����f��Ȃ��悤�ɃJ�����𒲐�����
			ge->qa_Map->AdjustCameraPos();

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