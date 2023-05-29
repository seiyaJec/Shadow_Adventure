//-------------------------------------------------------------------
//マップ生成
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->fontDebug = DG::Font::Create("ＭＳ ゴシック", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->fontDebug.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		if (nullptr != ge->qa_MapSelect) {
			this->folderPath = ge->qa_MapSelect->folderPath;
		}
		else {
			this->folderPath = "./data/MapMaker/MapData/Data0/";
		}
		//画像の優先度
		this->render2D_Priority[1] = 0.01f;
		//コントローラー
		this->controller = ge->in1;
		//モード切替
		this->gamemode = GameMode::Maker;
		//セーブ関係
		this->startSave = false;
		//カメラ
		ge->camera2D = ML::Box2D(
			0,
			0,
			ge->screen2DWidth,
			ge->screen2DHeight
		);
		//プレイ時マップサイズ
		this->playMapSize = { 0,0 };
		//参照可能にする準備
		ge->qa_MapMaker = ge->GetTask<MapMaker::Object>("マップ生成", "NoName");
		//制限時間等の読み込み
		this->LoadDataForPlay();
		//★タスクの生成
		//マップ
		auto map = Map2D::Object::Create(true);
		if (false == map->Load(this->folderPath + "OriginalMap.txt")) {
			map->Load("./data/MapMaker/DefaultMap.txt");
		}
		map->ChangeMapSize(100, 100);
		ge->qa_Map = ge->GetTask<Map2D::Object>("フィールド", "マップ");
		//カーソル
		Cursor::Object::Create(true);
		//UI
		MakeMapUI::Object::Create(true);
		//背景の雲
		for (int i = 0; i < 20; ++i) {
			auto effect = Effect04::Object::Create(true);
			int basePosX = i * 150;
			effect->pos = ML::Vec2((float)(basePosX + rand() % 100), (float)(rand() % 300));
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("マップ制作UI");
		ge->KillAll_G("フィールド");
		ge->KillAll_G("アイコン");
		ge->KillAll_G("オブジェクトデータ");
		ge->KillAll_G("カーソル");
		ge->KillAll_G("グリッド");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();
		ge->qa_Map = ge->GetTask<Map2D::Object>("フィールド", "マップ");
		ge->qa_DataObjects = ge->GetTasks<BObject>("オブジェクトデータ");
		ge->qa_MakeMapUI = ge->GetTask<MakeMapUI::Object>("マップ制作UI", "NoName");
		ge->qa_Cursor = ge->GetTask<Cursor::Object>("カーソル", "NoName");
		ge->qa_MapMaker = ge->GetTask<MapMaker::Object>("マップ生成", "NoName");
		ge->qa_IconEnemy = ge->GetTask<IconEnemy::Object>("アイコン", "敵");
		ge->qa_IconMapchip = ge->GetTask<IconMapchip::Object>("アイコン", "マップ");

		if (false == this->startSave) {
			if (true == ge->finishedLoadDisappear) {
				//セレクトボタンでセーブ開始
				if (this->gamemode == GameMode::Maker) {
					if (true == inp.SE.down) {
						AppearLoad(10, 3, 20, ML::Color(1.0f, 0.6f, 0.9f, 1.0f));
						this->startSave = true;
						return;
					}
				}
				if (true == inp.ST.down) {
					this->Stop();
					ge->StopAll_G("マップ制作UI");
					ge->StopAll_G("フィールド");
					ge->StopAll_G("アイコン");
					ge->StopAll_G("オブジェクトデータ");
					ge->StopAll_G("マップデータ");
					ge->StopAll_G("カーソル");
					ge->StopAll_G("グリッド");
					ge->StopAll_G("ゲームモード");
					ge->StopAll_G("プレイヤ");
					ge->StopAll_G("敵");
					ge->StopAll_G("エフェクト");
					ge->StopAll_G("攻撃");
					ge->StopAll_G("ゲームUI");
					MakerPause::Object::Create(true);
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Pause;
				}
			}
			//B4でゲームモードに切り替え
			if (true == inp.B4.down) {
				if (this->gamemode == GameMode::Maker && timeCnt >= 10) {
					this->gamemode = GameMode::Game;
					//マップのサイズを反映
					this->SetPlayMapSize();
					ge->qa_Map->ChangeMapSize(this->playMapSize.x, this->playMapSize.y);

					//this->MovePlayerToGameMap();
					ge->KillAll_G("カーソル");
					MapMakerGame::Object::Create(true);
				}
			}


			//右スティックでカメラ移動
			{
				float angle = inp.RStick.angleDYP;
				float volume = inp.RStick.volume;
				if (volume > 0) {
					ge->camera2D.x += (int)(cos(angle) * (volume * 10));
					ge->camera2D.y += (int)(sin(angle) * (volume * 10));
				}
				//マップの外側が映らないようにカメラを調整する
				ge->qa_Map->AdjustCameraPos();
			}
			++this->timeCnt;
		}
		else {
			if (this->gamemode != GameMode::SAVE) {
				//ロードタスクが出現完了した時点で処理をする
				if (true == ge->finishedLoadAppear) {
					//マップ選択タスク読み込み
					auto select = MapSelect::Object::Create(true);
					select->selectMode = select->SAVE;
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->SelectSave;
					//カーソル消滅
					ge->qa_Cursor->Kill();
					//ロードタスク消滅処理
					DisappearLoad(3, 20);
					//モード変更
					this->gamemode = GameMode::SAVE;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//ファイルにセーブ
	bool Object::SaveToFile(const string& stageName_, const string& makerName_)
	{
		string fpath = ge->qa_MapSelect->folderPath;
		//マップデータ（プレイ用）
		{
			//ファイルを開く（書き込み）
			ofstream fout(fpath + "OriginalMap.txt");
			if (!fout) { return false; }//書き込み失敗

			//変更可能するときに書き換える
			//背景
			fout << 0 << endl;
			//fout << 2 << endl;
			//fout << "BG1.png\n";
			//fout << "992 544\n";
			//fout << "BG2.png\n";
			//fout << "992 544\n";
			//マップチップ
			fout << "mygame2.txt\n";


			//敵を追加したときはここの関数を増やす（増やす必要無くなったかも？）
			{
				int enemysCnt = 0;
				string mFileName = "";
				int files = SaveEnemyToFile(fpath, mFileName);
				fout << files << endl;
				fout << mFileName;
			}

			//マップサイズ設定
			this->SetPlayMapSize();

			//マップデータ
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
			//マップデータ（メーカー用）
			ofstream fout(fpath + "OriginalMap_MakerData.txt");
			fout << "100 100\n";
			auto mapData = ge->GetTask<DataMapChip::Object>("マップデータ");
			for (int y = 0; y < 100; ++y) {
				for (int x = 0; x < 100; ++x) {
					fout << mapData->arr[y][x] << ' ';
				}
				fout << endl;
			}
			fout.close();
		}

		//プレイタスク用データ保存
		{
			ofstream fout(fpath + "OriginalMap_DataForPlay.txt");
			int gameTime = 0;
			//制限時間
			{
				auto displays = ge->GetTasks<MakerSetTimeDisplay::Object>("マップ制作UI", "時間設定ディスプレイ");
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
				auto display = ge->GetTask<MakerSetPlayerHPDisplay::Object>("マップ制作UI", "プレイヤーHP設定ディスプレイ");
				fout << display->myNumber * 10 << endl;
			}
			//スタート座標
			{
				//プレイヤーデータオブジェクトがマップの範囲内にあればそのままの座標を入れる
				auto player = ge->GetTask<DataPlayer::Object>("オブジェクトデータ", "プレイヤ");
				if (ge->qa_Map->hitBase.Hit(player->hitBase.OffsetCopy(player->pos))) {
					fout << player->pos.x << ' ' << player->pos.y << endl;
				}
				//いなければ初期値
				else {
					fout << 100 << ' ' << 100 << endl;
				}
			}
			fout.close();
		}
		{
			//セレクト画面用データ
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
		auto enemys = ge->GetTasks<Data_Enemy::Object>("オブジェクトデータ", "敵");
		//無ければ何もしない
		if (enemys->size() == 0) {
			return 0;
		}


		//タイプごとに設定
		for (int ty = 0; ty < ge->qa_IconEnemy->enemyTypeSize; ++ty) {
			string eFileName;
			//数字を文字列に変換後、ファイル名指定
			{
				char eNumC[3];
				sprintf_s(eNumC, "%02d", ty);
				string eNumStr = eNumC;
				eFileName = "Enemy" + eNumStr + "Data.txt";
				mFileName_ += eFileName + "\n";
			}
			//敵ファイル作成
			ofstream efout(fpath_ + "OriginalMap_" + eFileName);
			efout << ty << endl;
			string enemyPosData;	//敵位置データ
			int enemysCount = 0;	//敵数カウント
			for (auto it = enemys->begin();
				it != enemys->end();
				++it)
			{
				if ((*it)->type == ty)
				{
					//すでに「タスク状態が死亡」になっている敵は除外する
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

	//ゲーム用データ読み込み
	void Object::LoadDataForPlay()
	{
		ifstream fin(this->folderPath + "OriginalMap_DataForPlay.txt");
		//ファイルが読み込めない場合
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

	//プレイ時マップサイズの設定
	void Object::SetPlayMapSize()
	{
		//一番端にあるブロックをマップの端とする
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
		
		//マップ最小を求める
		int mapSizeMinX = ge->camera2D.w / ge->qa_Map->length;
		if (ge->camera2D.w % ge->qa_Map->length != 0) {
			++mapSizeMinX;
		}
		int mapSizeMinY = ge->camera2D.h / ge->qa_Map->length;
		if (ge->camera2D.h % ge->qa_Map->length != 0) {
			++mapSizeMinY;
		}
		//マップ最小よりも小さくならないように調整
		//x
		if (mapSizeMinX > this->playMapSize.x) {
			this->playMapSize.x = mapSizeMinX;
		}
		//y
		if (mapSizeMinY > this->playMapSize.y) {
			this->playMapSize.y = mapSizeMinY;
		}
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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