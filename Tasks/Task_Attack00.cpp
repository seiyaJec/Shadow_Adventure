//-------------------------------------------------------------------
//�v���C���[�̒ʏ�U��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Play.h"
#include  "Task_Player.h"
#include  "Task_Attack00.h"

namespace  Attack00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		LoadAnim(this->animAttack, "./data/animation/player/Attack.txt");
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
		this->hitBase = setCenterPoint(30, 48);
		this->MovePosToAngle(this->hitBase, 0, 14);
		this->attackBase = setCenterPoint(54, 64);
		//�F
		this->color = ML::Color(1, 1, 1, 1);
		//�X�e�[�^�X
		this->statAttack = 10;
		//�ړ�
		this->moveTime = 20;
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
			float vecRate = this->CheckDamageToEnemy(this->hitBase);
			if (vecRate != 0) {
				if (vecRate >= 0.5f) {
					if (nullptr != ge->qa_Player) {
						ge->qa_Player->StartCameraShake(6, 2, 2, 0);
					}
				}
				this->Kill();
			}
		}
		//�U�����Ԃ��߂��������
		if (this->moveCnt >= this->moveTime) {
			this->Kill();
		}
		++this->moveCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		AnimSet(this->res->animAttack, moveCnt / 6);

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
				ge->qa_TPlayer->res->imgShadowLeft->Draw(draw, src, color);
			}
			else {
				ge->qa_TPlayer->res->imgShadowRight->Draw(draw, src, color);
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