//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_EffectNew.h"

namespace  EffectNew
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�f�t�H���g
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
		this->render2D_Priority[1] = 0.f;

		//�ړ���
		this->pos = ML::Vec2(0, 0);
		this->moveVec = ML::Vec2(0,0);
		this->graVec = ML::Vec2(0, 0);

		//�p�x
		this->angle = 0.f;//ML::ToRadian((float)(rand() % 360));
		this->addAngle = 0.f;

		//��
		this->alpha = 1.f;
		this->addAlpha = 0.f;

		//����������@
		this->scale = 1.f;
		this->addScale = 0.f;

		//�J�E���g
		this->count = 0;
		this->countMax = 30;

		//���\�[�X�T�C�Y
		this->resSizeX = 256;
		this->resSizeY = 256;
		this->drawSizeX = 64;
		this->drawSizeY = 64;

		//�A�j��
		this->animno = 0;
		this->addAnim = 0.f;
		this->animMax = 0;
		this->animResNum = 1;

		this->filename = "";

		this->AddRender = false;

		this->fade = false;
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
//		this->img.reset();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (fade == true) {
			alpha = sin(ML::ToRadian(angle));
			angle += addAngle;
			if (angle >= 180.f) {
				this->Kill();
			}
		}
		else {

			//�ړ�
			pos += moveVec;		//�ړ�
			moveVec += graVec;	//�d�͉��Z

			//�p�x
			angle += addAngle;
			//�����x
			alpha += addAlpha;
			//�g�嗦
			scale += addScale;

			//���Ńg���K�[�̓J�E���g�A�A�j���̂�
			//�J�E���g
			//�J�E���g�����ŏ���
			this->count++;
			//�J�E���g�̓A�j��������ꍇ�A���ŏ����ɂȂ�Ȃ�
			if (animMax == 0) {
				if (count == countMax) {
					this->Kill();
				}
			}
			//�A�j���[�V����
			//�A�j���I���ŏ���
			this->animno += this->addAnim;

			//�A�j����0�R�}�̏ꍇ�͏��ŏ����ɂȂ�Ȃ�
			if (animMax != 0) {
				if ((int)animno >= animMax) {
					this->Kill();
				}
			}
			//----------
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�肳�ꂽ�t�@�C���̃��[�h
//
		if (this->count == 0) {
			if (!this->filename.empty()) {
				this->img = DG::Image::Create(filename);
			}
			else {
				this->img = DG::Image::Create("./data/image/��-01.png");
			}
		}

		//Box2D��`��int x,y,w,h��float�ɂ��邱��
		ML::Box2D  draw((int)(-drawSizeX / 2 * scale) ,(int)( -drawSizeY / 2 * scale),
			(int)(drawSizeX * scale),(int)( drawSizeY * scale));
		draw.Offset(this->pos);
		//
		if (this->animResNum <= 0) this->animResNum = 1;
		ML::Box2D  src(((int)this->animno % this->animResNum) * resSizeX,
			((int)this->animno / this->animResNum) * resSizeY,
			resSizeX, resSizeY);

		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

		//�t�F�[�h�̍ۂɂ͗��p��
		if (fade != true) {
			this->img->Rotation(this->angle, ML::Vec2(draw.w / 2.0f, draw.h / 2.0f));
		}
		if (this->AddRender) {
			//���ݓo�^����Ă���o�b�t�@���������_�����O����B
			ge->dgi->Flush2D();
			ge->dgi->EffectState().BS_AlphaAdd();
		}
		this->img->Draw(draw, src, ML::Color(this->alpha, 1, 1, 1));

		this->img->Rotation(0, ML::Vec2(draw.w / 2.0f, draw.h / 2.0f));

		if (this->AddRender) {
			//������x�����_�����O
			ge->dgi->Flush2D();
			ge->dgi->EffectState().BS_Alpha();
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
				//�i���\�b�h�����ςȂ̂͋��o�[�W�����̃R�s�[�ɂ��o�O��������邽��
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