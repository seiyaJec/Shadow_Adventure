//-------------------------------------------------------------------
//�^�X�N�ǂݍ��ݎ����[�h�̔w�i
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_LoadBG.h"

namespace  LoadBG
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/LoadBG.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->cuts = 0;
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
		this->render2D_Priority[1] = 0.1f;
		//y��h�͌Ăяo�����Ɍ��肳����
		//��`
		this->drawBase.w = ge->screen2DWidth;
		this->drawBase.x = -this->drawBase.w / 2;
		this->srcBase = ML::Box2D(8, 8, 8, 8);
		//�ʒu
		this->pos.x = this->drawBase.w + (this->drawBase.w / 2.0f);
		//����
		this->orderNum = 0;
		//�ړ�
		this->moveSpeed = 0;
		//����
		this->appearFlag = false;
		this->disappearFlag = false;
		//�v��
		this->appearCnt = 0;
		this->appearCntMax = 0;
		this->disappearCnt = 0;
		this->disappearCntMax = 0;
		//�F
		this->color = ML::Color(1, 1, 1, 1);
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
		float centerOfScreenX = ge->screen2DWidth / 2.0f;

		//�o������
		if (this->appearFlag == true) {
			//��ʒ[�ɂȂ�܂ňړ�������
			if (this->pos.x > centerOfScreenX) {		
				//�֐��Ō��߂��J�E���g���l�ɂȂ�����ړ��J�n
				if (this->appearCnt >= appearCntMax) {	
					this->pos.x -= this->moveSpeed;
					//�[�𒴂�����߂�
					if (this->pos.x <= centerOfScreenX) {	
						this->pos.x = centerOfScreenX;
						//�Ō�̃��[�h�^�X�N�Ȃ�I����`����
						if (true == this->returnFinish) {	
							ge->finishedLoadAppear = true;
						}
					}
				}
				++this->appearCnt;
			}

			//���ŏ���
			else if (this->disappearFlag == true) {
				//�w�肵���J�E���g�ɂȂ�����ړ��J�n
				if (this->disappearCnt >= disappearCntMax) {
					//�ړ�
					if (this->pos.x > -centerOfScreenX) {
						this->pos.x -= this->moveSpeed;
					}
					//�Ō�̃��[�h�^�X�N�Ȃ�I����`����
					else {
						if (true == this->returnFinish) {
							ge->finishedLoadDisappear = true;
						}
						this->Kill();
					}
				}
				++this->disappearCnt;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		this->res->img->Draw(draw, src, this->color);
	}
	
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�o������
	void Object::Appear(int cuts_,int orderNum_,int distance_, float speed_)
	{
		//��������ݒ�
		this->cuts = cuts_;
		//������ݒ�
		this->orderNum = orderNum_;
		//��`�̐ݒ�
		this->drawBase.h = ge->screen2DHeight / this->cuts;
		this->drawBase.y = -this->drawBase.h / 2;
		//�w�肵�����Ԃ̒��S�̍��������߂�
		this->pos.y = (this->drawBase.h * this->orderNum) + (this->drawBase.h / 2.0f);
		//�o���̊Ԋu��ݒ�
		this->appearCntMax = distance_ * this->orderNum;
		//�ړ��̑����̐ݒ�
		this->moveSpeed = speed_;
		//�o�������J�n
		this->appearFlag = true;
		//�Ō�̏��ԂȂ烍�[�h�I����Ԃ���ڂ���������
		if (this->orderNum == this->cuts - 1) {
			this->returnFinish = true;
		}
	}

	//���ŏ���
	void Object::Disappear(int distance_, float speed_)
	{
		//�ړ��̑����̐ݒ�
		this->moveSpeed = speed_;
		//���ł̊Ԋu��ݒ�i���Ԃ�1�̏ꍇ�͐ݒ肵�Ȃ�
		this->disappearCntMax = distance_ * this->orderNum;
		//���ŏ����J�n
		this->disappearFlag = true;
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