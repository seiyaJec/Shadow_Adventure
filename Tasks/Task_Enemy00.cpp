//-------------------------------------------------------------------
//�G00�F�X���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy00.h"

namespace  Enemy00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		imgLeft = DG::Image::Create("./data/image/Enemy/Enemy00L.png");
		imgRight = DG::Image::Create("./data/image/Enemy/Enemy00R.png");
		LoadAnim(this->animIdle, "./data/animation/enemy/enemy00/Idle.txt");
		LoadAnim(this->animRunning, "./data/animation/enemy/enemy00/Running.txt");
		LoadAnim(this->animAttack, "./data/animation/enemy/enemy00/Attack.txt");
		LoadAnim(this->animDamaged, "./data/animation/enemy/enemy00/Damaged.txt");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		imgLeft.reset();
		imgRight.reset();
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
		//�\���D��x
		this->render2D_Priority[1] = 0.4f;
		//��`
		this->hitBase = setCenterPoint(44, 36);
		this->MovePosToAngle(this->hitBase, 0, 6);
		this->drawBase = setCenterPoint(48, 48);
		this->srcBase = ML::Box2D(0, 0, 48, 48);
		this->playerSerchBox = setCenterPoint(200,50);
		//�A�j���[�V����
		this->animState = AnimState::IDLE;
		//�X�e�[�^�X
		this->statHPMax = 10;
		this->statHP = this->statHPMax;
		this->statAttack = 10;
		//�ړ�
		this->jumpPow = -3;
		this->fallSpeedMax = 10;
		//��_���[�W�����֌W
		this->damagedVecMax = ML::Vec2(7, -7);
		this->damagedCntMax = 20;
		//�U�����̌���
		this->attackAngle = Angle_LR::Right;
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
		ML::Vec2 est(0, 0);
		if (true == this->damaged) {
			if (this->moveCnt > damagedCntMax) {
				this->animState = AnimState::DAMAGED;
				//�ړ�
				this->moveVec.x -= damagedSlowdownX * Angle_LR_ChangeAngleToInt(this->damagedAngle_LR);
				//��������
				this->moveVec.y += ge->gravity;
				est = this->moveVec;

				this->CheckMove(est);

				//�ړ����Ȃ��Ȃ�����I������
				if (true == this->IsZeroDamagedMove()) {
					if (this->statHP <= 0) {
						this->Kill();
						return;
					}
					this->damaged = false;
					this->animState = AnimState::IDLE;
					return;
				}
			}
		}
		else {
			//�v���C���[�ɍU������
			this->CheckDamageToPlayer();
			if (this->animState != AnimState::ATTACK) {
				if (true == this->SerchPlayer()) {
					this->animState = AnimState::ATTACK;
					this->moveCnt = 0;
					this->animCnt = 0;
				}
			}

			//���[�V�������Ƃɏ�����ύX
			switch (this->animState) {
				//�ҋ@
			case AnimState::IDLE:
				if (this->moveCnt >= 200) {
					this->animState = AnimState::RUNNING;
					this->moveCnt = 0;
					this->animCnt = 0;
				}
				break;
				//�ړ�
			case AnimState::RUNNING: {
				//int checkCnt = animCnt / 10 % this->res->animRunning.Max;
				//if(checkCnt <)
				est.x += 0.1f * Angle_LR_ChangeAngleToInt(this->angle_LR);
				if (this->moveCnt >= 200) {
					this->animState = AnimState::IDLE;
					this->moveCnt = 0;
					this->animCnt = 0;
					}
				}
				break;
				//�U��
			case AnimState::ATTACK:

				if (this->moveCnt >= 20) {
					est.x += 1 * Angle_LR_ChangeAngleToInt(this->angle_LR);
				}
				if (this->moveCnt >= 100) {
					this->animState = AnimState::IDLE;
					this->moveCnt = 0;
					this->animCnt = 0;
				}
				break;
				
			}
			//�������x�̐���
			if (this->fallSpeed > this->fallSpeedMax) {
				this->fallSpeed = this->fallSpeedMax;
			}
			//��������
			est.y += this->fallSpeed;

			this->CheckMove(est);
			
			//���n����
			this->hitFlagFoot = this->CheckFoot();
			//���n����
			if (true == this->hitFlagFoot) {
				if (this->fallSpeed > 0) {
					this->fallSpeed = 0; //�������x0
				}
			}
			else {
				this->fallSpeed += ge->gravity;
			}
		}

		

		++this->moveCnt;
		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase;
		this->Anim(draw);
		ML::Box2D src = this->srcBase;
		//���ݒn�ֈړ�
		draw.Offset(this->pos);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//�`���[�g���A�����͕`���ύX
		DrawSetIfTuto(draw,src);
		//�F�ݒ�
		ML::Color drawColor(1, 1, 1, 1);
		if (true == this->damaged) {
			drawColor = ML::Color(0.5f, 1, 1, 1);
			if (moveCnt <= 20) {
				this->drawBase.x += (rand() % 10) - 5;
			}
		}
		if (this->angle_LR == Angle_LR::Right) {
			this->res->imgRight->Draw(draw, src, drawColor);
		}
		else {
			this->res->imgLeft->Draw(draw, src, drawColor);
		}

		//�f�o�b�O�p��`�\��
		if (this->moveCnt >= 10) {
			this->DebugDrawBox(this->hitBase);
		}
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	void Object::Anim(ML::Box2D& draw_)
	{
		int imgSizeW = 48;
		int imgSizeH = 48;
		draw_.x = imgSizeW / -2;
		draw_.y = imgSizeH / -2;
		int animNum = 0;
		switch (this->animState) {
			//�Î~
		case AnimState::IDLE:
			AnimSet(this->res->animIdle, this->animCnt / 10);
			break;

			//����
		case AnimState::RUNNING:
			AnimSet(this->res->animRunning, this->animCnt / 10);
			break;
			//�U��
		case AnimState::ATTACK:
			AnimSet(this->res->animAttack, this->animCnt / 10);
			break;
			//��_���[�W
		case AnimState::DAMAGED:
			AnimSet(this->res->animDamaged, this->animCnt / 10);
			break;

		}
			//�n�ʂɂ���
		MovePosToAngle(draw_, 0, 2);
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