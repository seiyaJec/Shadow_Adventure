//-------------------------------------------------------------------
//�G�I���A�C�R��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_IconEnemy.h"
#include  "Task_DataEnemy.h"
#include  "Task_MakeMapUI.h"
#include  "Task_MapMaker.h"

namespace  IconEnemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img00 = DG::Image::Create("./data/image/Enemy/Enemy00R.png");
		this->img01 = DG::Image::Create("./data/image/Enemy/Enemy01R.png");
		this->imgNow = nullptr;	//���ݑI�����Ă���摜
		this->cantPutColor = ge->qa_MakeMapUI->res->cantPutColor;

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img00.reset();
		this->img01.reset();
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
		//�ǂݍ��݂��\�ɂ�����
		ge->qa_IconEnemy = ge->GetTask<IconEnemy::Object>("�A�C�R��", "�G");
		//�摜�D��x
		this->render2D_Priority[1] = 0.25f;
		//�摜
		this->SetImgSize(48, 48);
		//��`
		this->hitBase = setCenterPoint(44, 36);
		this->hitBase.Offset(0, 6);
		this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
		this->srcBase = ML::Box2D(0, 0, imgSizeW, imgSizeH);
		//���݂̓G�̎��
		this->enemyType = 0;
		//���^�X�N�̐���
		this->Load();

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
		auto inp = ge->qa_MapMaker->controller->GetState();
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		this->pos = ge->qa_MakeMapUI->pos;
		this->pos += ML::Vec2(-50, 0);

		//B3�ł��ꂼ��^�C�v�؂�ւ�
		if (true == inp.B3.down) {
			switch (ge->qa_Cursor->cursorMode) {
			case CursorMode::ENEMY:
				++ge->qa_IconEnemy->enemyType;
				if (ge->qa_IconEnemy->enemyType == ge->qa_IconEnemy->enemyTypeSize) {
					ge->qa_IconEnemy->enemyType = 0;
				}
				break;
			}
		}

		//�N���b�N���ꂽ�烂�[�h�ύX
		if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				ge->qa_Cursor->cursorMode = CursorMode::ENEMY;
			}
		}

		//�^�C�v���ƂɈ�����摜�E�T�C�Y��p��
		this->SetImageByType();

		//�T�C�Y�̕ύX
		this->InCursorSizeUp();

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�Q�[�����[�h���͏������Ȃ�
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

		if (nullptr != this->res->imgNow) {
			this->Draw(this->res->imgNow);
		}

	}

	//-------------------------------------------------------------------
	// �֐���`
	//-------------------------------------------------------------------
	//�G�ǂݍ���
	void Object::Load()
	{
		ge->KillAll_G("�G");
		for (int i = 0; i < this->enemyTypeSize; ++i) {
			char num[3];
			sprintf_s(num, "%02d", i);
			string enemyTypeNum = num;
			//�t�@�C�����Ȃ��ꍇ�͓ǂݍ��܂Ȃ�
			ifstream fin(ge->qa_MapMaker->folderPath + "OriginalMap_Enemy" + enemyTypeNum + "Data.txt");
			if (!fin) { continue; }
			int enemyCnt;
			fin >> this->enemyType;
			this->SetImageByType();
			fin >> enemyCnt;
			for (int e = 0; e < enemyCnt; ++e) {
				ML::Vec2 enemyPos;
				auto enemy = Data_Enemy::Object::Create(true);
				fin >> enemy->pos.x;
				fin >> enemy->pos.y;
				enemy->puted = true;
			}
			fin.close();
		}
	}
	//����
	void Object::Generate()
	{
		auto enemy = Data_Enemy::Object::Create(true);
		enemy->pos = ge->qa_Cursor->pos;
	}

	//�^�C�v���Ƃɉ摜��ύX
	void Object::SetImageByType() {
		switch (enemyType) {
		case 0:
			this->ChangeImage(this->res->img00, 48, 48);
			this->hitBase = setCenterPoint(44, 36);
			this->hitBase.Offset(0, 6);
			break;
		case 1:
			this->ChangeImage(this->res->img01, 80, 80);
			this->hitBase = setCenterPoint(44, 36);
			this->hitBase.Offset(0, 6);
			break;
		default:
			break;
		}
	}

	//�摜�ύX
	void Object::ChangeImage(DG::Image::SP img_,int imgW_, int imgH_)
	{
		this->res->imgNow = img_;
		this->SetImgSize(imgW_, imgH_);
		this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
		this->srcBase = ML::Box2D(0, 0, imgSizeW, imgSizeH);
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