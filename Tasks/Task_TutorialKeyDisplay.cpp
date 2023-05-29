//-------------------------------------------------------------------
//�`���[�g���A���p���͕\��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_TutorialKeyDisplay.h"

namespace  TutorialKeyDisplay
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgStd = DG::Image::Create("./data/image/Tutorial/KeyStandard.png");
		this->imgShift = DG::Image::Create("./data/image/Tutorial/KeyShift.png");
		this->imgRT = DG::Image::Create("./data/image/Tutorial/KeyRT.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgStd.reset();
		this->imgShift.reset();
		this->imgRT.reset();
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
		this->render2D_Priority[1] = 0.5f;
		//�摜�T�C�Y�ݒ�
		this->SetImgSize(32, 32);
		//��`
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//�t���O�֌W
		this->useRT = false;
		this->useShift = false;
		this->pushing = false;
		this->playerInput = false;
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
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		//�{�^����������Ă�����摜�ω�
		if (true == this->pushing) {
			src.x += this->imgSizeW;
		}
		//R�g���K�[�̎��̕`��
		if (true == this->useRT) {
			this->res->imgRT->Draw(draw, src);
			//�v���C���[�̓��͂�����ꍇ�͉摜�̒ǉ�
			if (true == this->playerInput) {
				src.x = this->imgSizeW * 2;
				this->res->imgRT->Draw(draw, src);
			}
		}
		//Shift�̎��̕`��
		else if (true == this->useShift) {
			this->res->imgShift->Draw(draw, src);
			//�v���C���[�̓��͂�����ꍇ�͉摜�̒ǉ�
			if (true == this->playerInput) {
				src.x = this->imgSizeW * 2;
				this->res->imgShift->Draw(draw, src);
			}
		}
		//�W���̕`��
		else {
			this->res->imgStd->Draw(draw, src);
			//�v���C���[�̓��͂�����ꍇ�͉摜�̒ǉ�
			if (true == this->playerInput) {
				src.x = this->imgSizeW * 2;
				this->res->imgStd->Draw(draw, src);
			}
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