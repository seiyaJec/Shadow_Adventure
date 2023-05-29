//-------------------------------------------------------------------
//�}�b�v�I�����̊m�F���
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSelectCheck.h"
#include  "Task_MapSelectCheckSelector.h"

namespace  MapSelectCheck
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/MapSelectCheck.png");
		this->font = DG::Font::Create("�l�r �S�V�b�N", 22, 44);
		this->fontMin = DG::Font::Create("�l�r �S�V�b�N", 14, 28);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->font.reset();
		this->fontMin.reset();
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
		this->render2D_Priority[1] = 0.3f;
		//�R���g���[���[
		this->controller = ge->in1;
		//�摜�T�C�Y
		this->SetImgSize(610, 260);
		//�ʒu���
		this->pos.x = (float)ge->screen2DWidth / 2;
		this->pos.y = (float)ge->screen2DHeight / 2;
		//��`
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		this->srcBase.x = ge->qa_MapSelect->selectMode;
		//�t���O
		this->selectedFlag = false;
		this->selectOK = false;
		//�}�b�v��
		this->mapName = "�u" + ge->qa_MapSelect->mapData[ge->qa_MapSelect->displayPos].mapName + "�v";
		//OK�܂ł̋���
		this->OKNum = 1;
		if (ge->qa_MapSelect->selectMode == ge->qa_MapSelect->REMOVE) {
			this->OKNum = 2;
		}
		//�I��
		this->selectingNum = 0;

		//���^�X�N�̐���
		auto selector = MapSelectCheckSelector::Object::Create(true);
		selector->pos.y = this->pos.y + 70;
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
					//�Z���N�^�[�ǉ�������L������������悤��

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		if (false == this->selectedFlag) {
			//���E�{�^���őI��
			if (inp.LStick.BR.down) {
				if (this->selectingNum < this->OKNum) {
					++this->selectingNum;
				}
			}
			else if (inp.LStick.BL.down) {
				if (this->selectingNum > 0) {
					--this->selectingNum;
				}
			}
			//����
			if (inp.B1.down) {
				this->selectedFlag = true;
				if (this->selectingNum == this->OKNum) {
					this->selectOK = true;
				}
				ge->KillAll_GN("�}�b�v�I���m�F���", "�Z���N�^�[");
			}
		}

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�摜
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			ML::Box2D src = this->srcBase;
			src.x = ge->qa_MapSelect->selectMode * this->imgSizeW;
			this->res->img->Draw(draw, src);
		}
		//����
		{
			ML::Box2D draw = setCenterPoint((int)this->mapName.size() * 22, 44);
			draw.Offset(this->pos.x,this->pos.y - 44);
			this->res->font->Draw(draw, this->mapName, ML::Color(1, 0, 0, 0));
		}
		//���b�Z�[�W
		{
			ML::Box2D draw = setCenterPoint((int)this->checkStr[ge->qa_MapSelect->selectMode].size() * 14, 28);
			draw.Offset(this->pos);
			this->res->fontMin->Draw(draw, this->checkStr[ge->qa_MapSelect->selectMode], ML::Color(1, 0, 0, 0));
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