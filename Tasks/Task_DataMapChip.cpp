//-------------------------------------------------------------------
//�}�b�v�f�[�^
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_DataMapChip.h"
#include  "Task_Map2D.h"

namespace  DataMapChip
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = ge->qa_Map->img;
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
		for (int y = 0; y < 100; ++y) {
			for (int x = 0; x < 100; ++x) {
				this->arr[y][x] = 0;
			}
		}

		this->Load();
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
		//�J�[�\�����܂����������������Ȃ�
		if (nullptr == ge->qa_Cursor) {
			return;
		}

		//�}�b�v�`�b�v�̐ݒu����
		if (ge->qa_Cursor->cursorMode == CursorMode::MAP) {
			//���N���b�N���������͐ݒu
			if (ge->qa_Cursor->CheckLeftState(BuState::ON)) {
				if (false == ge->qa_Cursor->movingObject) {
					if (true == ge->qa_Cursor->inGameMap) {
						int gridPosX = (int)ge->qa_Cursor->pos.x / 32;
						int gridPosY = (int)ge->qa_Cursor->pos.y / 32;
						this->arr[gridPosY][gridPosX] = ge->qa_IconMapchip->mapchipMode;
						this->ApplyToMap();
					}
				}
			}

			//�E�N���b�N���������͍폜
			else if (ge->qa_Cursor->CheckRightState(BuState::ON)) {
				if (true == ge->qa_Cursor->inGameMap) {
					int gridPosX = (int)ge->qa_Cursor->pos.x / 32;
					int gridPosY = (int)ge->qa_Cursor->pos.y / 32;
					this->arr[gridPosY][gridPosX] = 0;
					this->ApplyToMap();
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

	}


	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------

	//�ݒu�����`�b�v���}�b�v�ɔ��f
	void Object::ApplyToMap()
	{
		//�ǂݍ��ݔ͈͂��J�����̉f��͈͓������ɂ���
		ML::Rect loadArea;
		loadArea.left = ge->camera2D.x / 32;
		if (loadArea.left != 0) {
			--loadArea.left;		//�J������1�}�X�O���ǂݍ���
		}
		loadArea.top = ge->camera2D.y / 32;
		if (loadArea.top != 0) {
			--loadArea.top;
		}
		loadArea.right = (ge->camera2D.x + ge->camera2D.w) / 32;
		if (loadArea.right > 100 - 1) {
			loadArea.right = 100 - 1;
		}
		loadArea.bottom = (ge->camera2D.y + ge->camera2D.h) / 32;
		if (loadArea.bottom > 100 - 1) {
			loadArea.bottom = 100 - 1;
		}
		
		//�ݒu�E��������
		for (int y = loadArea.top; y <= loadArea.bottom; ++y) {
			for (int x = loadArea.left; x <= loadArea.right; ++x) {
				int find = this->arr[y][x];
				int arrNum = 0;

				//�������Ă��镔����T���i�㉺���E�j
				if (y - 1 >= 0 && this->arr[y - 1][x] == find) {
					arrNum += 1;
				}
				if (x + 1 <= 100 - 1 && this->arr[y][x + 1] == find) {
					arrNum += 2;
				}
				if (y + 1 <= 100 - 1 && this->arr[y + 1][x] == find) {
					arrNum += 4;
				}
				if (x - 1 >= 0 && this->arr[y][x - 1] == find) {
					arrNum += 8;
				}

				//�}�b�v�ɔ��f
				ge->qa_Map->arr[y][x] = ge->qa_Map->chipArr[find][arrNum];
			}
		}

	}

	//���[�J�[�p�}�b�v�f�[�^��ǂݍ���
	void Object::Load()
	{
		ifstream fin(ge->qa_MapMaker->folderPath + "OriginalMap_MakerData.txt");
		if (!fin) { return; }
		//�}�b�v�̃T�C�Y��ǂݍ���
		int mapSizeW;
		fin >> mapSizeW;
		int mapSizeH;
		fin >> mapSizeH;
		//1�}�X���f�[�^��ǂݍ���
		for (int y = 0; y < mapSizeH; ++y) {
			for (int x = 0; x < mapSizeW; ++x) {
				fin >> this->arr[y][x];
			}
		}
		fin.close();
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