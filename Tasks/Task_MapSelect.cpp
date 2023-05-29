//-------------------------------------------------------------------
//�}�b�v�I��
//-------------------------------------------------------------------
#include <direct.h>
#include  "MyPG.h"
#include  "Task_MapSelect.h"
#include  "Task_MapSelectBG.h"
#include  "Task_MapSelector.h"
#include  "Task_MapNameDisplay.h"
#include  "Task_MapStartDisplay.h"
#include  "Task_MapMakersDisplay.h"
#include  "Task_MapSelectMessage.h"
#include  "Task_MapSelectCheck.h"
#include  "Task_MapSelectPage.h"
#include  "Task_KeyBoard.h"
#include  "Task_LoadBG.h"
#include  "Task_BeforePlay.h"

namespace  MapSelect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
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
		//���g�̃|�C���^��ݒ�
		ge->qa_MapSelect = ge->GetTask<MapSelect::Object>("�}�b�v�I��");

		//���[�h
		this->selectMode = SELECTMODE::PLAY;
		//�ʒu
		this->selectingPos = UP;
		//�y�[�W
		this->page = 0;
		//�y�[�W�ő�
		this->pageMax = 5;
		//�R���g���[���[
		this->controller = ge->in1;
		//�t���O
		this->inCheck = false;
		this->saving = false;
		this->appearLoad = false;
		this->backToTitle = false;
		//���͏��
		this->stageNameInput = "";
		this->makerNameInput = "";

		//�}�b�v�t�@�C���ǂݍ���
		this->LoadMapFile();

		//���^�X�N�̐���
		MapSelectBG::Object::Create(true);
		MapSelector::Object::Create(true);
		//�y�[�WUI
		for (int i = 0; i < this->pageMax; ++i) {
			auto page = MapSelectPage::Object::Create(true);
			page->pos.x = 170.0f + (i * 20);
			page->orderNum = i;
		}
		//���O������\��
		for (int i = 0; i < 3; ++i) {
			auto map = MapNameDisplay::Object::Create(true);
			map->pos.y = 104.0f + (i * 154);
			map->orderNum = i;
		}
		//�}�b�v�X�^�[�g�n�_�摜�\��
		MapStartDisplay::Object::Create(true);
		//�}�b�v����ҕ\��
		MapMakersDisplay::Object::Create(true);
		//���b�Z�[�W
		MapSelectMessage::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�}�b�v�I��w�i");
		ge->KillAll_G("�}�b�v�I���m�F���");
		ge->KillAll_G("�}�b�v����Җ��\��");
		ge->KillAll_G("�}�b�v�I�𐧍�Җ��\��");
		ge->KillAll_G("�}�b�v�I���Z���N�^�[");
		ge->KillAll_G("�}�b�v�I���X�^�[�g�n�_�\���p");
		ge->KillAll_G("�}�b�v�I�����b�Z�[�W");
		ge->KillAll_G("�}�b�v�I���y�[�WUI");



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


		if (false == this->inCheck) {
			//�J�[�\���E�y�[�W�̈ړ�
			if (inp.LStick.BD.down) {
				if (this->selectingPos != DOWN) {
					++this->selectingPos;
				}
			}
			if (inp.LStick.BU.down) {
				if (this->selectingPos != UP) {
					--this->selectingPos;
				}
			}
			if (inp.LStick.BR.down) {
				if (this->page < this->pageMax - 1) {
					++this->page;
				}
			}
			if (inp.LStick.BL.down) {
				if (this->page > 0) {
					--this->page;
				}
			}
			//�I�����[�h�̕ύX
			if (this->selectMode != SAVE) {
				if (inp.B3.down) {
					++this->selectMode;
					if (this->selectMode >= SAVE) {
						this->selectMode = LOAD;	//0�ł���uLOAD�v�ɖ߂���
					}
				}
				if (inp.B2.down) {
					AppearLoad(10, 3, 20);
					this->appearLoad = true;
					this->backToTitle = true;
					return;
				}
			}
			//�Z�[�u���[�h�̏ꍇ�AB2�������ꂽ��Z�[�u�����ɖ߂�
			else {
				if (inp.B2.down) {
					AppearLoad(10, 3, 20, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
					this->appearLoad = true;
					return;
				}
			}

			//����
			if (inp.B1.down) {
				this->inCheck = true;
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->ResultCheck;
				MapSelectCheck::Object::Create(true);
			}
			this->displayPos = (this->page * 3) + this->selectingPos;
		}
		else {
			auto chk = ge->GetTask<MapSelectCheck::Object>("�}�b�v�I���m�F���", "�f�B�X�v���C");
			//chk���L��������I���i�������̏����j
			if (nullptr != chk) {
				if (true == chk->selectedFlag) {
					//���݂̃��[�h���Ƃɕʂ̏���������
					if (true == chk->selectOK) {
						switch (this->selectMode) {
						case LOAD: {
							if (false == this->appearLoad) {
								this->folderPath = "./data/MapMaker/MapData/Data" + to_string(this->displayPos) + "/";
								AppearLoad(10, 3, 20,ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
								this->appearLoad = true;
								chk->Kill();
							}
							break;
						}
						case PLAY: {
							//�Q�[���^�X�N
							this->folderPath = "./data/MapMaker/MapData/Data" + to_string(this->displayPos) + "/";
							//�J����΃Q�[���J�n����
							ifstream fin(folderPath + "OriginalMap.txt");
							if (fin) {
								AppearLoad(10, 3, 20, ML::Color(1.0f, 1.0f, 0.8f, 0.5f));
								this->appearLoad = true;
							}
							else {
								this->inCheck = false;
							}
							chk->Kill();
							break;
						}
						case REMOVE: {
							//�t�@�C���̍폜
							string fpath = "./data/MapMaker/MapData/Data" + to_string(displayPos) + "/";
							std::remove((fpath + "OriginalMapFileData.txt").c_str());
							std::remove((fpath + "OriginalMap.txt").c_str());
							std::remove((fpath + "OriginalMap_DataForPlay.txt").c_str());
							std::remove((fpath + "OriginalMap_MapFileData.txt").c_str());
							std::remove((fpath + "OriginalMap_MakerData.txt").c_str());
							for (int i = 0; i < 5; ++i) {
								char num[3];
								sprintf_s(num, "%02d", i);
								string efpath = fpath + "OriginalMap_Enemy" + num + "Data.txt";
								std::remove(efpath.c_str());
							}
							//�Q�[�����\���p�̈ꎞ�L���������Ă���
							this->mapData[displayPos].imageFileName = "";
							this->mapData[displayPos].mapName = "NoData";
							this->mapData[displayPos].makerName = "";
							for (int y = 0; y < 11; ++y) {
								for (int x = 0; x < 14; ++x) {
									this->mapData[displayPos].startPointMapData[y][x] = 0;
								}
							}
							this->inCheck = false;
							chk->Kill();
							break;
						}
						case SAVE: {
							//�Z�[�u
							this->folderPath = "./data/MapMaker/MapData/Data" + to_string(this->displayPos) + "/";
							KeyBoard::Object::Create(true);
							ge->qa_KeyBoard->userInput = mapData[this->displayPos].mapName;
							this->saving = true;
							ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->KeyBoard;
							chk->Kill();
							break;
						}
						}
					}
					else {
						this->inCheck = false;
						if (this->selectMode == SAVE) {
							ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->SelectSave;
						}
						else {
							ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Select;
						}
						chk->Kill();
					}
				}
			}

			if (true == saving) {
				//�L�����Z������
				if (ge->qa_KeyBoard->cancelFlag) {
					this->stageNameInput = "";
					this->saving = false;
					this->inCheck = false;
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->SelectSave;
					ge->qa_Cursor->Kill();
					ge->KillAll_G("�L�[�{�[�h");
				}
				else {
					//�X�e�[�W���ݒ�
					if (this->stageNameInput == "") {
						ge->qa_KeyBoard->messageStr = "�X�e�[�W�������";
						ge->qa_KeyBoard->userInputLimit = 10;
						if (true == ge->qa_KeyBoard->finishInputFlag) {
							this->stageNameInput = ge->qa_KeyBoard->userInput;
							ge->qa_KeyBoard->finishInputFlag = false;
							ge->qa_KeyBoard->userInput = mapData[this->displayPos].makerName;
						}
					}
					//��Җ��ݒ�E�Z�[�u�E�I������
					else if (this->makerNameInput == "") {
						ge->qa_KeyBoard->messageStr = "��Җ������";
						ge->qa_KeyBoard->userInputLimit = 20;
						if (true == ge->qa_KeyBoard->finishInputFlag) {
							this->makerNameInput = ge->qa_KeyBoard->userInput;
							//�Z�[�u
							ge->qa_MapMaker->SaveToFile(this->stageNameInput, this->makerNameInput);
							ge->KillAll_G("�L�[�{�[�h");
							//���[�h�^�X�N�o��
							AppearLoad(10, 3, 20);
							this->appearLoad = true;
							this->saving = false;
						}
					}
				}
			}


		}

		//���[�h����
		if (true == this->appearLoad) {
			if (true == ge->finishedLoadAppear) {
				if (true == this->backToTitle) {
					Title::Object::Create(true);
				}
				//���[�h���Ƃɏ�����ς���
				else {
					switch (this->selectMode) {
					case SELECTMODE::LOAD:
						MapMaker::Object::Create(true);
						ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Maker;
						break;
					case SELECTMODE::SAVE:
						ge->qa_MapMaker->gamemode = GameMode::Maker;
						ge->qa_MapMaker->startSave = false;
						ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Maker;
						if (nullptr == ge->qa_Cursor) {
							Cursor::Object::Create(true);
						}
						break;
					case SELECTMODE::PLAY: {
						auto before = BeforePlay::Object::Create(true);
						before->filePath = this->folderPath + "OriginalMap.txt";
						ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Play;
					}
					}
				}
				//���[�h�����ł�����
				DisappearLoad(3, 20);

				this->Kill();
				ge->KillAll_G("�}�b�v�I��w�i");
				ge->KillAll_G("�}�b�v�I���m�F���");
				ge->KillAll_G("�}�b�v����Җ��\��");
				ge->KillAll_G("�}�b�v�I���}�b�v���\��");
				ge->KillAll_G("�}�b�v�I���Z���N�^�[");
				ge->KillAll_G("�}�b�v�I���X�^�[�g�n�_�\���p");
				ge->KillAll_G("�}�b�v�I�����b�Z�[�W");
				ge->KillAll_G("�}�b�v�I���y�[�WUI");

			}
		}


	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�}�b�v���̓ǂݍ���
	void Object::LoadMapFile()
	{
		for (int i = 0; i < 15; ++i) {
			string folderPath = "./data/MapMaker/MapData/Data" + to_string(i) + '/';
			//�t�H���_�[���̂��Ȃ��ꍇ�͍��
			int makeResult = _mkdir(folderPath.c_str());

			//�ǂݍ���
			ifstream fin(folderPath + "OriginalMap_MapFileData.txt");
			if (!fin) {		//���s��
				this->mapData[i].mapName = "NoData";
			}
			else {			//������
				//�}�b�v��
				getline(fin, this->mapData[i].mapName);
				//��Җ�
				getline(fin, this->mapData[i].makerName);
				//�}�b�v�摜��
				fin >> this->mapData[i].imageFileName;
				//�X�^�[�g�n�_�}�b�v�f�[�^
				for (int y = 0; y < 11; ++y) {
					for (int x = 0; x < 14; ++x) {
						fin >> this->mapData[i].startPointMapData[y][x];
					}
				}
				fin.close();
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