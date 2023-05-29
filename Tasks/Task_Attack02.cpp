//-------------------------------------------------------------------
//�v���C���[�̒ʏ�U��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Play.h"
#include  "Task_Player.h"
#include  "Task_Attack02.h"

namespace  Attack02
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		LoadAnim(this->animAttackAir, "./data/animation/player/AttackAir.txt");
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
		//�摜�D��x
		this->render2D_Priority[1] = 0.4f;
		//��`
		this->drawBase = setCenterPoint(100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		this->hitBase = setCenterPoint(38, 48);//ML::Box2D(-19, -24, 38, 48);
		this->MovePosToAngle(this->hitBase, 0, 10);
		this->attackBase = setCenterPoint(90, 58);//ML::Box2D(-45, -29, 90, 58);
		this->MovePosToAngle(this->attackBase, 0, 6);
		//�X�e�[�^�X
		this->statAttack = 10;
		//�ړ�
		this->moveTime = 22;
		this->fallSpeed = 0;
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

		//�������x�̐���
		if (this->fallSpeed > this->fallSpeedMax) {
			this->fallSpeed = this->fallSpeedMax;
		}

		est += this->moveVec;

		//���������i���݉��ŋx�~���j
		//est.y += this->fallSpeed;

		this->CheckMove(est);

		//���n����(�߂荞��łȂ����m�F������ɍs���j
		this->hitFlagFoot = this->CheckFoot();

		//���n����
		if (true == this->hitFlagFoot) {
			if (this->fallSpeed > 0) {
				this->fallSpeed = 0; //�������x0
			}
		}
		else {
			this->fallSpeed += ge->gravity/2;
		}

		//�U�������̓����蔻��
		if (this->moveCnt >= 6) {
			float vecRate = this->CheckDamageToEnemy(this->attackBase);
			if (vecRate >= 0.5f) {
				if (nullptr != ge->qa_Player) {
					ge->qa_Player->StartCameraShake(6, 2, 2, 0);
				}
			}
		}

		//�̕����̓����蔻��
		{
			int atackMemory = this->statAttack;
			this->statAttack = (int)(statAttack * 0.1f);
			float vecRate = this->CheckDamageToEnemy(this->hitBase);
			if (vecRate != 0) {
				if (vecRate >= 0.5f) {
					if (nullptr != ge->qa_Player) {
						ge->qa_Player->StartCameraShake(6, 2, 2, 0);
					}
				}
			}
			this->statAttack = atackMemory;
		}
		if (this->moveCnt >= this->moveTime) {
			this->Kill();
		}
		++this->moveCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		AnimSet(this->res->animAttackAir, moveCnt / 6);

		auto draw = this->drawBase;
		auto src = this->srcBase;
		auto color = this->color;
		draw.Offset(this->pos);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//�`���[�g���A�����͕`���ύX
		DrawSetIfTuto(draw,src);


		if (nullptr != ge->qa_Player) {
			if (this->angle_LR == Angle_LR::Left) {
				ge->qa_Player->res->imgShadowLeft->Draw(draw, src, color);
			}
			else {
				ge->qa_Player->res->imgShadowRight->Draw(draw, src, color);
			}
		}
		else if (nullptr != ge->qa_TPlayer) {
			if (this->angle_LR == Angle_LR::Left) {
				ge->qa_TPlayer->res->imgShadowLeft->Draw(draw, src);
			}
			else {
				ge->qa_TPlayer->res->imgShadowRight->Draw(draw, src);
			}
		}


		//�f�o�b�O�p��`�\��
		this->DebugDrawBox(this->hitBase);
		if (this->moveCnt >= 8) {
			this->DebugDrawBox(this->attackBase);
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
















#if(false)






//-------------------------------------------------------------------
//�v���C���[�̒ʏ�U��
//-------------------------------------------------------------------

namespace  Attack02
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
		this->pos = ML::Vec2(0, 0);
		this->hitBase = ML::Box2D(-45, -29, 90, 58);
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
		this->pos = ge->qa_Player->pos;
		this->pos += ML::Vec2(1, 6);
		this->pos.x *= this->Angle_LR_ChangeAngleToInt(ge->qa_Player->angle_LR);

		auto enemy = this->CheckHitToEnemy();
		if (nullptr != enemy) {
			enemy->Kill();
		}
		++this->aliveTimeCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (true == this->res->debugFlag) {
			ML::Box2D drawD = this->hitBase.OffsetCopy(this->pos);
			ML::Box2D srcD(0, 0, 1, 1);
			drawD.Offset(-ge->camera2D.x, -ge->camera2D.y);
			ge->qa_Player->res->imgDebug->Draw(drawD, srcD);
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
#endif