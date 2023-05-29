//-------------------------------------------------------------------
//�t�H���g�����ݒ�^�X�N
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Font.h"

namespace  Font
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
		this->render2D_Priority[1] = 0.11f;
		this->finishSet = false;
		this->font = nullptr;
		this->displayStr = "";
		this->drawBase = ML::Box2D(0, 0, 0, 0);
		this->color = ML::Color(1, 1, 1, 1);
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		this->font.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		this->font->Draw(draw, this->displayStr);
	}

	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�ʒu�̐ݒ�
	void Object::SetPos(float x_, float y_)
	{
		this->pos.x = x_;
		this->pos.y = y_;
	}
	void Object::SetPos(const ML::Vec2& pos_)
	{
		this->pos = pos_;
	}

	//�T�C�Y�̐ݒ�
	void Object::SetSize(int w_, int h_)
	{
		//�ݒ�ς݂Ȃ珈�����Ȃ�
		if (true == this->finishSet) {
			return;
		}

		//�T�C�Y�̐ݒ�
		this->fontSizeW = w_;
		this->fontSizeH = h_;
		//�t�H���g����
		this->font = DG::Font::Create("�l�r �S�V�b�N", this->fontSizeW, this->fontSizeH);
		//��`�̐ݒ�
		this->drawBase = setCenterPoint(this->fontSizeW, this->fontSizeH);
	}

	//������̐ݒ�+�`���`�̍Đݒ�
	void Object::SetString(const string& str_)
	{
		this->displayStr = str_;
		this->drawBase.w = this->fontSizeW * (int)this->displayStr.size();
		this->drawBase.x = this->drawBase.w / -2;
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