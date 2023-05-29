//-------------------------------------------------------------------
//���Ԍv���\��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameTimeDisplay.h"
#include  "Task_MapMakerGame.h"

namespace  GameTimeDisplay
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->font = DG::Font::Create("�l�r �S�V�b�N", 22, 44);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		this->pos.x = ge->screen2DWidth / 2.0f;
		//����
		this->timeDisplay = "";
		//�t�H���g�T�C�Y
		this->fontSizeW = 22;
		this->fontSIzeH = 44;
		//��`
		this->drawBase = setCenterPoint(this->fontSizeW, this->fontSIzeH);
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
		//�Q�[���^�X�N
		if (nullptr != ge->qa_Play) {
			this->timeDisplay = "TIME " + to_string(ge->qa_Play->timeCntGame);
			if (ge->qa_Play->timeCntGame > 100) {
				this->color = ML::Color(1, 0, 0, 0);
			}
			else {
				this->color = ML::Color(1, 1, 0.5f, 0.5f);
			}
		}
		//�}�b�v������Q�[���^�X�N
		else {
			auto play = ge->GetTask<MapMakerGame::Object>("�Q�[�����[�h", "�}�b�v����");
			if (nullptr == play) {
				return;
			}
			this->timeDisplay = "TIME" + to_string(play->timeCntGame);
			if (play->timeCntGame > 100) {
				this->color = ML::Color(1, 0, 0, 0);
			}
			else {
				this->color = ML::Color(1, 1, 0.5f, 0.5f);
			}
		}

		//�T�C�Y�𕶎���ɍ��킹��
		this->drawBase.w = (int)this->timeDisplay.size() * this->fontSizeW;


		this->pos.x = ge->screen2DWidth / 2.0f;
		this->pos.y = this->drawBase.h / 2.0f;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		this->res->font->Draw(draw, this->timeDisplay,this->color);
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