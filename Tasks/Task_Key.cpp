//-------------------------------------------------------------------
//�Q�[�����L�[�{�[�h�̃L�[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Key.h"

namespace  Key
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/Key.png");
		this->font = DG::Font::Create("�l�r �S�V�b�N", 18, 36);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->font.reset();
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
		this->render2D_Priority[1] = 0.25f;
		//�ʒu���
		this->pos = ML::Vec2(100, 100);
		//����
		this->mychar = '\0';
		//�摜�T�C�Y
		this->SetImgSize(40,40);
		//��`
		this->hitBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, 40, 40);
		this->fontDrawBase = setCenterPoint(18, 36);
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
		//�J�[�\���Ə��A���N���b�N
		if (true == ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			if (true == ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				//�������L�[�{�[�h�ɓ����
				string tentativeStr = ge->qa_KeyBoard->userInput;
				tentativeStr += this->mychar;
				if (tentativeStr.size() <= ge->qa_KeyBoard->userInputLimit) {
					ge->qa_KeyBoard->userInput = tentativeStr;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			ML::Box2D src = this->srcBase;
			//�J�[�\���q�b�g�A�j���[�V����
			if (true == ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				src.x = this->imgSizeW;
				if (true == ge->qa_Cursor->CheckLeftState(BuState::ON)) {
					src.x = this->imgSizeW * 2;
				}
			}
			this->res->img->Draw(draw, src);
		}
		{
			ML::Box2D draw = this->fontDrawBase.OffsetCopy(this->pos);
			this->res->font->Draw(draw, mychar);
			//ge->qa_KeyBoard->res->fontObj.Draw(mychar,this->pos,48,48);
			
		}

	}
	//-------------------------------------------------------------------
	//�֐���`
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