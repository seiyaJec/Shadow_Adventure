//-------------------------------------------------------------------
//�}�b�v����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  <direct.h>
#include  "Task_Player.h"
#include  "Task_Map2D.h"
#include  "Task_LoadBG.h"
#include  "Task_MapMaker.h"
#include  "Task_MapMakerGame.h"
#include  "Task_MakeMapUI.h"
#include  "Task_DataMapChip.h"
#include  "Task_DataEnemy.h"
#include  "Task_Map2D.h"
#include  "Task_Effect.h"
#include  "Task_Cursor.h"
#include  "Task_MakerPause.h"
#include  "Task_MakerSetTimeDisplay.h"
#include  "Task_MakerSetPlayerHPDisplay.h"
#include  "Task_DataPlayer.h"

namespace  MapMaker
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->fontDebug = DG::Font::Create("�l�r �S�V�b�N", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->fontDebug.reset();
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
		if (nullptr != ge->qa_MapSelect) {
			this->folderPath = ge->qa_MapSelect->folderPath;
		}
		else {
			this->folderPath = "./data/MapMaker/MapData/Data0/";
		}
		//�摜�̗D��x
		this->render2D_Priority[1] = 0.01f;
		//�R���g���[���[
		this->controller = ge->in1;
		//���[�h�ؑ�
		this->gamemode = GameMode::Maker;
		//�Z�[�u�֌W
		this->startSave = false;
		//�J����
		ge->camera2D = ML::Box2D(
			0,
			0,
			ge->screen2DWidth,
			ge->screen2DHeight
		);
		//�v���C���}�b�v�T�C�Y
		this->playMapSize = { 0,0 };
		//�Q�Ɖ\�ɂ��鏀��
		ge->qa_MapMaker = ge->GetTask<MapMaker::Object>("�}�b�v����", "NoName");
		//�������ԓ��̓ǂݍ���
		this->LoadDataForPlay();
		//���^�X�N�̐���
		//�}�b�v
		auto map = Map2D::Object::Create(true);
		if (false == map->Load(this->folderPath + "OriginalMap.txt")) {
			map->Load("./data/MapMaker/DefaultMap.txt");
		}
		map->ChangeMapSize(100, 100);
		ge->qa_Map = ge->GetTask<Map2D::Object>("�t�B�[���h", "�}�b�v");
		//�J�[�\��
		Cursor::Object::Create(true);
		//UI
		MakeMapUI::Object::Create(true);
		//�w�i�̉_
		for (int i = 0; i < 20; ++i) {
			auto effect = Effect04::Object::Create(true);
			int basePosX = i * 150;
			effect->pos = ML::Vec2((float)(basePosX + rand() % 100), (float)(rand() % 300));
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�}�b�v����UI");
		ge->KillAll_G("�t�B�[���h");
		ge->KillAll_G("�A�C�R��");
		ge->KillAll_G("�I�u�W�F�N�g�f�[�^");
		ge->KillAll_G("�J�[�\��");
		ge->KillAll_G("�O���b�h");

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
		ge->qa_Map = ge->GetTask<Map2D::Object>("�t�B�[���h", "�}�b�v");
		ge->qa_DataObjects = ge->GetTasks<BObject>("�I�u�W�F�N�g�f�[�^");
		ge->qa_MakeMapUI = ge->GetTask<MakeMapUI::Object>("�}�b�v����UI", "NoName");
		ge->qa_Cursor = ge->GetTask<Cursor::Object>("�J�[�\��", "NoName");
		ge->qa_MapMaker = ge->GetTask<MapMaker::Object>("�}�b�v����", "NoName");
		ge->qa_IconEnemy = ge->GetTask<IconEnemy::Object>("�A�C�R��", "�G");
		ge->qa_IconMapchip = ge->GetTask<IconMapchip::Object>("�A�C�R��", "�}�b�v");

		if (false == this->startSave) {
			if (true == ge->finishedLoadDisappear) {
				//�Z���N�g�{�^���ŃZ�[�u�J�n
				if (this->gamemode == GameMode::Maker) {
					if (true == inp.SE.down) {
						AppearLoad(10, 3, 20, ML::Color(1.0f, 0.6f, 0.9f, 1.0f));
						this->startSave = true;
						return;
					}
				}
				if (true == inp.ST.down) {
					this->Stop();
					ge->StopAll_G("�}�b�v����UI");
					ge->StopAll_G("�t�B�[���h");
					ge->StopAll_G("�A�C�R��");
					ge->StopAll_G("�I�u�W�F�N�g�f�[�^");
					ge->StopAll_G("�}�b�v�f�[�^");
					ge->StopAll_G("�J�[�\��");
					ge->StopAll_G("�O���b�h");
					ge->StopAll_G("�Q�[�����[�h");
					ge->StopAll_G("�v���C��");
					ge->StopAll_G("�G");
					ge->StopAll_G("�G�t�F�N�g");
					ge->StopAll_G("�U��");
					ge->StopAll_G("�Q�[��UI");
					MakerPause::Object::Create(true);
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Pause;
				}
			}
			//B4�ŃQ�[�����[�h�ɐ؂�ւ�
			if (true == inp.B4.down) {
				if (this->gamemode == GameMode::Maker && timeCnt >= 10) {
					this->gamemode = GameMode::Game;
					//�}�b�v�̃T�C�Y�𔽉f
					this->SetPlayMapSize();
					ge->qa_Map->ChangeMapSize(this->playMapSize.x, this->playMapSize.y);

					//this->MovePlayerToGameMap();
					ge->KillAll_G("�J�[�\��");
					MapMakerGame::Object::Create(true);
				}
			}


			//�E�X�e�B�b�N�ŃJ�����ړ�
			{
				float angle = inp.RStick.angleDYP;
				float volume = inp.RStick.volume;
				if (volume > 0) {
					ge->camera2D.x += (int)(cos(angle) * (volume * 10));
					ge->camera2D.y += (int)(sin(angle) * (volume * 10));
				}
				//�}�b�v�̊O�����f��Ȃ��悤�ɃJ�����𒲐�����
				ge->qa_Map->AdjustCameraPos();
			}
			++this->timeCnt;
		}
		else {
			if (this->gamemode != GameMode::SAVE) {
				//���[�h�^�X�N���o�������������_�ŏ���������
				if (true == ge->finishedLoadAppear) {
					//�}�b�v�I���^�X�N�ǂݍ���
					auto select = MapSelect::Object::Create(true);
					select->selectMode = select->SAVE;
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->SelectSave;
					//�J�[�\������
					ge->qa_Cursor->Kill();
					//���[�h�^�X�N���ŏ���
					DisappearLoad(3, 20);
					//���[�h�ύX
					this->gamemode = GameMode::SAVE;
				}
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
	//�t�@�C���ɃZ�[�u
	bool Object::SaveToFile(const string& stageName_, const string& makerName_)
	{
		string fpath = ge->qa_MapSelect->folderPath;
		//�}�b�v�f�[�^�i�v���C�p�j
		{
			//�t�@�C�����J���i�������݁j
			ofstream fout(fpath + "OriginalMap.txt");
			if (!fout) { return false; }//�������ݎ��s

			//�ύX�\����Ƃ��ɏ���������
			//�w�i
			fout << 0 << endl;
			//fout << 2 << endl;
			//fout << "BG1.png\n";
			//fout << "992 544\n";
			//fout << "BG2.png\n";
			//fout << "992 544\n";
			//�}�b�v�`�b�v
			fout << "mygame2.txt\n";


			//�G��ǉ������Ƃ��͂����̊֐��𑝂₷�i���₷�K�v�����Ȃ��������H�j
			{
				int enemysCnt = 0;
				string mFileName = "";
				int files = SaveEnemyToFile(fpath, mFileName);
				fout << files << endl;
				fout << mFileName;
			}

			//�}�b�v�T�C�Y�ݒ�
			this->SetPlayMapSize();

			//�}�b�v�f�[�^
			fout << to_string(this->playMapSize.x) << ' ' << to_string(this->playMapSize.y) << '\n';
			for (int y = 0; y < this->playMapSize.y; ++y) {
				for (int x = 0; x < this->playMapSize.x; ++x) {
					fout << ge->qa_Map->arr[y][x] << ' ';
				}
				fout << endl;
			}

			fout.close();
		}
		{
			//�}�b�v�f�[�^�i���[�J�[�p�j
			ofstream fout(fpath + "OriginalMap_MakerData.txt");
			fout << "100 100\n";
			auto mapData = ge->GetTask<DataMapChip::Object>("�}�b�v�f�[�^");
			for (int y = 0; y < 100; ++y) {
				for (int x = 0; x < 100; ++x) {
					fout << mapData->arr[y][x] << ' ';
				}
				fout << endl;
			}
			fout.close();
		}

		//�v���C�^�X�N�p�f�[�^�ۑ�
		{
			ofstream fout(fpath + "OriginalMap_DataForPlay.txt");
			int gameTime = 0;
			//��������
			{
				auto displays = ge->GetTasks<MakerSetTimeDisplay::Object>("�}�b�v����UI", "���Ԑݒ�f�B�X�v���C");
				for (auto it = displays->begin();
					it != displays->end();
					++it)
				{
					gameTime += (*it)->myNumber * (*it)->numberPlace;
				}
				fout << gameTime << endl;
			}
			//HP
			{
				auto display = ge->GetTask<MakerSetPlayerHPDisplay::Object>("�}�b�v����UI", "�v���C���[HP�ݒ�f�B�X�v���C");
				fout << display->myNumber * 10 << endl;
			}
			//�X�^�[�g���W
			{
				//�v���C���[�f�[�^�I�u�W�F�N�g���}�b�v�͈͓̔��ɂ���΂��̂܂܂̍��W������
				auto player = ge->GetTask<DataPlayer::Object>("�I�u�W�F�N�g�f�[�^", "�v���C��");
				if (ge->qa_Map->hitBase.Hit(player->hitBase.OffsetCopy(player->pos))) {
					fout << player->pos.x << ' ' << player->pos.y << endl;
				}
				//���Ȃ���Ώ����l
				else {
					fout << 100 << ' ' << 100 << endl;
				}
			}
			fout.close();
		}
		{
			//�Z���N�g��ʗp�f�[�^
			ofstream fout(fpath + "OriginalMap_MapFileData.txt");
			fout << ge->qa_MapSelect->stageNameInput << endl;
			fout << ge->qa_MapSelect->makerNameInput << endl;
			fout << "NewAssets.png" << endl;
			for (int y = 0; y < 11; ++y) {
				for (int x = 0; x < 14; ++x) {
					fout << ge->qa_Map->arr[y][x] << ' ';
				}
				fout << endl;
			}
			fout.close();
		}



		return true;

	}

	int Object::SaveEnemyToFile(const string& fpath_, string& mFileName_)
	{
		auto enemys = ge->GetTasks<Data_Enemy::Object>("�I�u�W�F�N�g�f�[�^", "�G");
		//������Ή������Ȃ�
		if (enemys->size() == 0) {
			return 0;
		}


		//�^�C�v���Ƃɐݒ�
		for (int ty = 0; ty < ge->qa_IconEnemy->enemyTypeSize; ++ty) {
			string eFileName;
			//�����𕶎���ɕϊ���A�t�@�C�����w��
			{
				char eNumC[3];
				sprintf_s(eNumC, "%02d", ty);
				string eNumStr = eNumC;
				eFileName = "Enemy" + eNumStr + "Data.txt";
				mFileName_ += eFileName + "\n";
			}
			//�G�t�@�C���쐬
			ofstream efout(fpath_ + "OriginalMap_" + eFileName);
			efout << ty << endl;
			string enemyPosData;	//�G�ʒu�f�[�^
			int enemysCount = 0;	//�G���J�E���g
			for (auto it = enemys->begin();
				it != enemys->end();
				++it)
			{
				if ((*it)->type == ty)
				{
					//���łɁu�^�X�N��Ԃ����S�v�ɂȂ��Ă���G�͏��O����
					if ((*it)->CheckState() == BTask::State::Kill) { continue; }
					char enemyPos[100];
					sprintf_s(enemyPos, "%d %d\n", (int)(*it)->pos.x, (int)(*it)->pos.y);
					enemyPosData += enemyPos;
					++enemysCount;
				}
			}
			efout << enemysCount << endl;
			efout << enemyPosData;
			efout.close();
		}
		return ge->qa_IconEnemy->enemyTypeSize;
	}

	//�Q�[���p�f�[�^�ǂݍ���
	void Object::LoadDataForPlay()
	{
		ifstream fin(this->folderPath + "OriginalMap_DataForPlay.txt");
		//�t�@�C�����ǂݍ��߂Ȃ��ꍇ
		if (!fin) {
			this->dataForPlay.gameTime = 0;
			this->dataForPlay.playerHPMax = 10;
			this->dataForPlay.playerStartPos = ML::Vec2(100, 100);
			return;
		}
		fin >> this->dataForPlay.gameTime;
		fin >> this->dataForPlay.playerHPMax;
		fin >> this->dataForPlay.playerStartPos.x;
		fin >> this->dataForPlay.playerStartPos.y;

		fin.close();
	}

	//�v���C���}�b�v�T�C�Y�̐ݒ�
	void Object::SetPlayMapSize()
	{
		//��Ԓ[�ɂ���u���b�N���}�b�v�̒[�Ƃ���
		for (int y = 0; y < 100; ++y) {
			for (int x = 0; x < 100; ++x) {
				if (ge->qa_Map->arr[y][x] != 0) {
					//x
					if (x + 1 > this->playMapSize.x) {
						this->playMapSize.x = x + 1;
					}
					//y
					if (y + 1 > this->playMapSize.y) {
						this->playMapSize.y = y + 1;
					}
				}
			}
		}
		
		//�}�b�v�ŏ������߂�
		int mapSizeMinX = ge->camera2D.w / ge->qa_Map->length;
		if (ge->camera2D.w % ge->qa_Map->length != 0) {
			++mapSizeMinX;
		}
		int mapSizeMinY = ge->camera2D.h / ge->qa_Map->length;
		if (ge->camera2D.h % ge->qa_Map->length != 0) {
			++mapSizeMinY;
		}
		//�}�b�v�ŏ������������Ȃ�Ȃ��悤�ɒ���
		//x
		if (mapSizeMinX > this->playMapSize.x) {
			this->playMapSize.x = mapSizeMinX;
		}
		//y
		if (mapSizeMinY > this->playMapSize.y) {
			this->playMapSize.y = mapSizeMinY;
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