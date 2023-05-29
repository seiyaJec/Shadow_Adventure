//-------------------------------------------------------------------
//���U���g��ʗp�X�R�A�{�[�h
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_ResultScoreBoard.h"

namespace  ResultScoreBoard
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Result/Result_ScoreBoard.png");
		this->font = DG::Font::Create("�l�r �S�V�b�N", 33, 66);
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
		this->render2D_Priority[1] = 0.8f;
		//�摜�T�C�Y
		this->SetImgSize(592, 220);
		//�t�H���g�T�C�Y
		this->fontSizeW = 33;
		this->fontSizeH = 66;
		//��`
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//�\���p
		this->clearTime = "�N���A�^�C���@";
		this->retryConut = "���g���C�񐔁@";
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
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			this->res->img->Draw(draw, this->srcBase);
		}
		//�N���A�^�C��
		{
			string clearTimeDisplay = this->clearTime + to_string(ge->qa_Result->clearTime);
			ML::Box2D draw = setCenterPoint(
				this->fontSizeW * (int)clearTimeDisplay.size(),
				this->fontSizeH
			);
			ML::Vec2 clearTimePos;
			clearTimePos.x = this->pos.x;
			clearTimePos.y = this->pos.y - 50;
			draw.Offset(clearTimePos);
			this->res->font->Draw(draw, clearTimeDisplay, ML::Color(1, 0, 0, 0));
		}
		//���g���C��
		{
			string retryCountDisplay = this->retryConut + to_string(ge->qa_Result->retryCount);
			ML::Box2D draw = setCenterPoint(
				this->fontSizeW * (int)retryCountDisplay.size(),
				this->fontSizeH
			);
			ML::Vec2 retryCountPos;
			retryCountPos.x = this->pos.x;
			retryCountPos.y = this->pos.y + this->fontSizeH - 10;
			draw.Offset(retryCountPos);
			this->res->font->Draw(draw, retryCountDisplay, ML::Color(1, 0, 0, 0));
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