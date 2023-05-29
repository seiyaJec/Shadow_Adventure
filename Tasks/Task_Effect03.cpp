//-------------------------------------------------------------------
//�G�t�F�N�g3�@��юU����̗�
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect03.h"

namespace  Effect03
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Effect/Light.png");
		LoadAnim(this->anim, "./data/animation/Effect/Effect03.txt");
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
		//�摜�T�C�Y
		this->SetImgSize(24, 24);
		//��`
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//�p�x
		this->angle = 0;
		//�A�j���[�V����
		this->animCnt = 0;
		this->animDistance = 4;
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
		this->pos.x += cos(this->angle);
		this->pos.y += sin(this->angle);
		//�A�j���[�V�������Ō�܂ōs������L��
		if (this->animCnt >= this->res->anim.Max * this->animDistance) {
			this->Kill();
		}
		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		int animNum = this->animCnt / this->animDistance;

		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D src = this->srcBase.OffsetCopy(this->res->anim.Pos[animNum]);
		//��]
		float centerPosX = this->imgSizeW / 2.0f;
		float centerPosY = this->imgSizeH / 2.0f;
		float imgAng = this->angle + ML::ToRadian(90);
		this->res->img->Rotation(imgAng, ML::Vec2(centerPosX, centerPosY));
		//�`��
		this->res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�܂Ƃ߂Đ��l��ς���Ƃ��Ɏg�p
	void Object::Set(ML::Vec2 pos_, float angRad_, int animDistance_)
	{
		this->pos = pos_;
		this->angle = angRad_;
		this->animDistance = animDistance_;
	}
	//�~�`�ɏo��������
	void Object::Circle(ML::Vec2 pos_, float distance_, int efctSize_)
	{
		this->pos.x = pos_.x + cos(this->angle) * distance_;
		this->pos.y = pos_.y + sin(this->angle) * distance_;

		for (int i = 0; i < efctSize_; ++i) {
			float efctAngle = this->angle + ML::ToRadian(360.0f / efctSize_ * i);

			auto efct = Effect03::Object::Create(true);
			efct->pos.x = pos_.x + cos(efctAngle) * distance_;
			efct->pos.y = pos_.y + sin(efctAngle) * distance_;
			efct->angle = efctAngle;
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