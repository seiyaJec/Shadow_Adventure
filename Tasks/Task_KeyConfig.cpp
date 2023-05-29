//-------------------------------------------------------------------
//�L�[�R���t�B�O
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_KeyConfig.h"

namespace  KeyConfig
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/KeyConfig.png");
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
		this->render2D_Priority[1] = 0.11f;
		//�摜�T�C�Y�ݒ�
		SetImgSize(440, 480);
		//�Q�[�����[�h
		this->gamemode = GameMode::Select;
		//��`
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//���W
		this->pos.x = ge->screen2DWidth + this->imgSizeW / 2;
		this->pos.y = this->imgSizeH / 2 + 20;
		//�ړ��֌W
		this->defaultSpeed = 16;
		this->speedRate = 1;
		this->speedAttenuationRate = 0.964f;
		this->speedAcceleration = 1.1f;
		this->attenuations = 14;
		//�t���O
		this->appearFlag = false;
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
		auto inp = ge->in1->GetState();

		if (true == this->appearFlag) {
			//�X�N���[�����ɕ\������Ƃ��̈ʒu
			float onScreenPos = ge->screen2DWidth - this->imgSizeW / 2;
			if (this->pos.x > onScreenPos) {
				//�X�s�[�h���������Ȃ���\��
				float speed = this->defaultSpeed * this->speedRate;
				this->speedRate *= this->speedAttenuationRate;
				pos.x -= speed;
				if (this->pos.x <= onScreenPos) {
					this->pos.x = onScreenPos;
				}
			}
		}
		else {
			//�X�N���[�����犮�S�ɏ�����ʒu
			float offScreenPos = ge->screen2DWidth + this->imgSizeW / 2;
			if (this->pos.x < offScreenPos) {
				//�X�s�[�h���������Ȃ������
				float speed = this->defaultSpeed * this->speedRate;
				this->speedRate *= this->speedAcceleration;
				pos.x += speed;
				if (this->pos.x >= offScreenPos) {
					this->pos.x = offScreenPos;
				}
			}
		}

		//�o���E����
		if (inp.Trigger.volumeL != 0) {
			if (false == this->triggerDowned) {
				this->appearFlag = !this->appearFlag;
				this->speedRate = 1;
				this->triggerDowned = true;
			}
		}
		else {
			this->triggerDowned = false;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		src.x += this->imgSizeW * this->gamemode;
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