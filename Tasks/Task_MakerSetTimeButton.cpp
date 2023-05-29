//-------------------------------------------------------------------
//�}�b�v���쎞�Ԑݒ�p�{�^��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MakerSetTimeButton.h"

namespace  MakerSetTimeButton
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/SetTimeButton.png");
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
		this->render2D_Priority[1] = 0.24f;
		//�摜�T�C�Y
		this->SetImgSize(21, 13);
		//��`
		this->hitBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//�{�^�����[�h
		this->buttonMode = TOP;
		//�ʒu���
		this->addPosY = 1;
		//�I�u�W�F�N�g
		this->linkingDisplay = nullptr;
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
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

		//�ʒu����
		this->pos = this->linkingDisplay->pos;
		this->pos.y += (this->linkingDisplay->fontSizeH / 2 + this->imgSizeH / 2 + this->addPosY) * (this->buttonMode * -1);
		//�J�[�\���������Ȃ珈�����Ȃ�
		if (nullptr != ge->qa_Cursor) {
			//�J�[�\�����Ə����Ă��āA
			if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				//���N���b�N���ꂽ��
				if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
					//������ύX
					this->linkingDisplay->myNumber += this->buttonMode;

					//����E�����𒴂����烋�[�v
					if (this->linkingDisplay->myNumber > 9) {
						this->linkingDisplay->myNumber = 0;
					}
					else if (this->linkingDisplay->myNumber < 0) {
						this->linkingDisplay->myNumber = 9;
					}

				}
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

		//�A�j���[�V����
		ML::Box2D src = this->srcBase;
		if (this->buttonMode == ButtonMode::BOTTOM) {
			src.y += this->imgSizeH;
		}
		if (nullptr != ge->qa_Cursor) {
			if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				src.x += this->imgSizeW;
			}
		}
		this->res->img->Draw(draw, src);
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