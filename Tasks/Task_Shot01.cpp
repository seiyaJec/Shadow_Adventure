//-------------------------------------------------------------------
//�v���C���̏o���e
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect00.h"
#include  "Task_EnemyTest.h"
#include  "Task_Map2D.h"
#include  "Task_Shot01.h"

namespace  Shot01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Shot00.png");
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
		this->render2D_Priority[1] = 0.4f;
		this->hitBase = ML::Box2D(-8, -8, 16, 16);
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
		this->moveCnt++;
		//���E�̎��Ԃ��}���������
		if (this->moveCnt >= 100 ) {
			//���Ő\��
			this->Kill();
			return;
		}

		//�ړ�
		this->pos += this->moveVec;
		//�d�͉���
		this->moveVec.y += ML::Gravity(32) * 5;

		//�ړ���ŏ�Q���ɐڐG���������
		//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
		if (ge->qa_Map->CheckState() != BTask::State::Kill) {
			ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
			if (true == ge->qa_Map->CheckHit(hit)) {
				//���Ő\��
				this->Kill();
				//��肠�������͂΂�܂���
				for (int c = 0; c < 4; ++c) {
					auto eff = Effect00::Object::Create(true);
					eff->pos = this->pos;
				}
				return;
			}
		}

		//�G�ΏۂƏՓ˔���&�_���[�W��^���鏈��
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		//�G�����ׂĒ��o����
		for (auto it = ge->qa_Enemys->begin();
			it != ge->qa_Enemys->end();
			++it)
		{
			//���łɁu�^�X�N��Ԃ����S�v�ɂȂ��Ă���G�͓����蔻�肩�珜�O����
			if ((*it)->CheckState() == BTask::State::Kill) { continue; }
			//�G�L�����N�^�̓����蔻���`��p��
			ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
			//�d�Ȃ�𔻒�
			if (true == you.Hit(me)) {
				(*it)->Kill();
				this->Kill();
				//��肠�������͂΂�܂���
				for (int c = 0; c < 4; ++c) {
					auto eff = Effect00::Object::Create(true);
					eff->pos = this->pos;
				}
				return;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-8, -8, 16, 16);
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 32, 32);

		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
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