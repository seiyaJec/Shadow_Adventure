//-------------------------------------------------------------------
//�Q�[���I�[�o�[���ɏo������V���h�E
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameOverShadow.h"

namespace  GameOverShadow
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgR = DG::Image::Create("./data/image/Player/PlayerShadowR.png");
		this->imgL = DG::Image::Create("./data/image/Player/PlayerShadowL.png");
		LoadAnim(this->anim, "./data/animation/gameOverShadow.txt");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgR.reset();
		this->imgL.reset();
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
		this->render2D_Priority[1] = 0.51f;
		//��`
		this->drawBase = setCenterPoint(100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		//�ʒu���
		this->pos = ge->qa_Player->pos;
		//���E
		this->angle_LR = ge->qa_Player->angle_LR;
		Angle_LR_Reverse(this->angle_LR);

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
		//�v���C���[�ƈꏏ�ɏ㏸����
		if (ge->qa_Player->deathCount >= ge->qa_Player->deathCountMax) {
			this->pos.y -= ge->qa_Player->deathDisappearSpeed;
			this->moveCnt = 1;
		}


	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase;
		this->AnimSet(this->res->anim, this->moveCnt);
		ML::Box2D src = this->srcBase;
		//���W�ʒu�֕ϊ�
		draw.Offset(this->pos);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//�`���[�g���A�����͕`���ύX
		DrawSetIfTuto(draw,src);
		//�����ɂ���Ďg���摜��ύX
		if (this->angle_LR == Angle_LR::Left) {
			this->res->imgL->Draw(draw, src);
		}
		else {
			this->res->imgR->Draw(draw, src);
		}

		//�f�o�b�O�p��`�\��
		this->DebugDrawBox(this->hitBase);

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