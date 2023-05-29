//-------------------------------------------------------------------
//�}�b�v�̖��O������
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapNameDisplay.h"

namespace  MapNameDisplay
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
		this->render2D_Priority[1] = 0.49f;
		//�}�b�v��
		this->fileNumString = "File";
		//�ʒu���
		this->pos = ML::Vec2(285, 100);		//���S215,100
		//����
		this->orderNum = 0;
		//�F
		this->color = ML::Color(1, 0, 0, 0);
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
		//�����̃t�@�C��
		int fileNum = (ge->qa_MapSelect->page * 3) + this->orderNum;
		//�t�@�C���̃}�b�v�����󂯎��
		nameString = ge->qa_MapSelect->mapData[fileNum].mapName;

		//�t�@�C�����𕶎���
		this->fileNumString = "File" + to_string(fileNum);

		//�������I������Ă�����F�ύX
		if (ge->qa_MapSelect->selectingPos == this->orderNum) {
			this->color = ML::Color(1, 1, 1, 1);
		}
		else {
			this->color = ML::Color(1, 0, 0, 0);
		}
		this->drawBase = setCenterPoint(((int)this->nameString.size() * 22), 44);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			this->res->font->Draw(draw, this->nameString, this->color);
		}
		{
			ML::Box2D draw = setCenterPoint(((int)this->fileNumString.size() * 22), 44);
			draw.Offset(100.0f, this->pos.y);
			this->res->font->Draw(draw, this->fileNumString, this->color);
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