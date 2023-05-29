//-------------------------------------------------------------------
//���f�̃{�^��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_PauseButton.h"

namespace  PauseButton
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/GamePause/GamePauseButton.png");
		this->font = DG::Font::Create("�l�r �S�V�b�N", 12, 24);
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
		this->render2D_Priority[1] = 0.15f;
		//�摜�T�C�Y
		this->SetImgSize(400, 60);
		//�t�H���g�T�C�Y
		this->fontSizeW = 12;
		this->fontSizeH = 24;
		//��`
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, 8, 8);
		//�o��
		this->appearWait = 0;
		this->appearTimeMax = 20;
		this->appearTime = this->appearTimeMax;
		this->appearSpeed = 8;
		//�I�𒆈ړ�
		this->selectingMove = 0;
		this->selectingMoveSpeed = 10;
		this->selectingMoveMax = 40;
		//�I�𒆑ҋ@
		this->selectingAlphaDec = 0;
		this->selectingAlphaDecSpeed = 1;
		this->selectingAlphaDecMax = 45;
		//�����x���Z����
		this->isAlphaDec = false;
		//�o���I���t���O
		this->finishedAppear = false;
		//������
		this->displayStr = "";
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
		//�o���҂��i���Ԃɂ��ς��j
		if (this->appearWait > 0) {
			--appearWait;
			return;
		}
		//�o������
		if (this->appearTime > 0) {
			--this->appearTime;
			//0�ɂȂ�����I����Ԃ�
			if (this->appearTime == 0) {
				this->finishedAppear = true;
			}
		}
		//�������I������Ă�����
		else if (ge->pauseSelectingButton == this->orderNum) {
			//�A�j���[�V�����p�̃J�E���g�𑝂₷
			if (this->selectingMove < this->selectingMoveMax) {
				this->selectingMove += this->selectingMoveSpeed;
				//�ő�Ȃ�J�E���g���~�߂�
				if (this->selectingMove >= this->selectingMoveMax) {
					this->selectingMove = this->selectingMoveMax;
				}
			}
			//�����x�ύX����
			if (true == this->isAlphaDec) {
				//�����x���グ��
				this->selectingAlphaDec += this->selectingAlphaDecSpeed;
				if (this->selectingAlphaDec >= this->selectingAlphaDecMax) {
					this->isAlphaDec = false;
					this->selectingAlphaDec = this->selectingAlphaDecMax;
				}
			}
			else {
				//�����x��������
				this->selectingAlphaDec -= this->selectingAlphaDecSpeed;
				if (this->selectingAlphaDec <= 0) {
					this->isAlphaDec = true;
					this->selectingAlphaDec = 0;
				}
			}
		}
		else {
			//�A�j���[�V�����p�̃J�E���g�𑝂₷
			if (this->selectingMove > 0) {
				this->selectingMove -= this->selectingMoveSpeed;
				//�ő�Ȃ�J�E���g���~�߂�
				if (this->selectingMove <= 0) {
					this->selectingMove = 0;
				}
			}

		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�����x
		float alpha = 1.0f;
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			//�o��A�j���[�V��������
			if (this->appearTime > 0) {
				alpha = (this->appearTimeMax - this->appearTime) / (float)this->appearTimeMax;
				draw.x += this->appearTime * this->appearSpeed;
			}
			//�I�����A�j���[�V��������
			else if(ge->pauseSelectingButton == this->orderNum) {
				draw.x += this->selectingMove;
				alpha = 1.0f - (this->selectingAlphaDec * 0.01f);
			}
			ML::Color color(alpha, 0.9f, 0.9f, 0.9f);
			this->res->img->Draw(draw, this->srcBase, color);
		}
		alpha = 1.0f;
		{
			ML::Box2D draw = setCenterPoint(this->fontSizeW * (int)this->displayStr.size(), this->fontSizeH);
			draw.Offset(this->pos);
			//�o��A�j���[�V��������
			if (this->appearTime > 0) {
				draw.x += this->appearTime * this->appearSpeed;
				alpha = (this->appearTimeMax - this->appearTime) / (float)this->appearTimeMax;
			}
			//�I�����A�j���[�V��������
			else if (ge->pauseSelectingButton == this->orderNum) {
				draw.x += this->selectingMove;
				alpha = 1.0f - (this->selectingAlphaDec * 0.01f);
			}
			ML::Color color(alpha, 0, 0, 0);
			this->res->font->Draw(draw, this->displayStr, color);
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