//-------------------------------------------------------------------
//マップ選択
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		//自身のポインタを設定
		ge->qa_MapSelect = ge->GetTask<MapSelect::Object>("マップ選択");

		//モード
		this->selectMode = SELECTMODE::PLAY;
		//位置
		this->selectingPos = UP;
		//ページ
		this->page = 0;
		//ページ最大
		this->pageMax = 5;
		//コントローラー
		this->controller = ge->in1;
		//フラグ
		this->inCheck = false;
		this->saving = false;
		this->appearLoad = false;
		this->backToTitle = false;
		//入力情報
		this->stageNameInput = "";
		this->makerNameInput = "";

		//マップファイル読み込み
		this->LoadMapFile();

		//★タスクの生成
		MapSelectBG::Object::Create(true);
		MapSelector::Object::Create(true);
		//ページUI
		for (int i = 0; i < this->pageMax; ++i) {
			auto page = MapSelectPage::Object::Create(true);
			page->pos.x = 170.0f + (i * 20);
			page->orderNum = i;
		}
		//名前文字列表示
		for (int i = 0; i < 3; ++i) {
			auto map = MapNameDisplay::Object::Create(true);
			map->pos.y = 104.0f + (i * 154);
			map->orderNum = i;
		}
		//マップスタート地点画像表示
		MapStartDisplay::Object::Create(true);
		//マップ制作者表示
		MapMakersDisplay::Object::Create(true);
		//メッセージ
		MapSelectMessage::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("マップ選択背景");
		ge->KillAll_G("マップ選択確認画面");
		ge->KillAll_G("マップ制作者名表示");
		ge->KillAll_G("マップ選択制作者名表示");
		ge->KillAll_G("マップ選択セレクター");
		ge->KillAll_G("マップ選択スタート地点表示用");
		ge->KillAll_G("マップ選択メッセージ");
		ge->KillAll_G("マップ選択ページUI");



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


		if (false == this->inCheck) {
			//カーソル・ページの移動
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
			//選択モードの変更
			if (this->selectMode != SAVE) {
				if (inp.B3.down) {
					++this->selectMode;
					if (this->selectMode >= SAVE) {
						this->selectMode = LOAD;	//0である「LOAD」に戻した
					}
				}
				if (inp.B2.down) {
					AppearLoad(10, 3, 20);
					this->appearLoad = true;
					this->backToTitle = true;
					return;
				}
			}
			//セーブモードの場合、B2が押されたらセーブせずに戻る
			else {
				if (inp.B2.down) {
					AppearLoad(10, 3, 20, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
					this->appearLoad = true;
					return;
				}
			}

			//決定
			if (inp.B1.down) {
				this->inCheck = true;
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->ResultCheck;
				MapSelectCheck::Object::Create(true);
			}
			this->displayPos = (this->page * 3) + this->selectingPos;
		}
		else {
			auto chk = ge->GetTask<MapSelectCheck::Object>("マップ選択確認画面", "ディスプレイ");
			//chkをキルしたら終了（実質一回の処理）
			if (nullptr != chk) {
				if (true == chk->selectedFlag) {
					//現在のモードごとに別の処理をする
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
							//ゲームタスク
							this->folderPath = "./data/MapMaker/MapData/Data" + to_string(this->displayPos) + "/";
							//開ければゲーム開始処理
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
							//ファイルの削除
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
							//ゲーム内表示用の一時記憶も消しておく
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
							//セーブ
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
				//キャンセル処理
				if (ge->qa_KeyBoard->cancelFlag) {
					this->stageNameInput = "";
					this->saving = false;
					this->inCheck = false;
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->SelectSave;
					ge->qa_Cursor->Kill();
					ge->KillAll_G("キーボード");
				}
				else {
					//ステージ名設定
					if (this->stageNameInput == "") {
						ge->qa_KeyBoard->messageStr = "ステージ名を入力";
						ge->qa_KeyBoard->userInputLimit = 10;
						if (true == ge->qa_KeyBoard->finishInputFlag) {
							this->stageNameInput = ge->qa_KeyBoard->userInput;
							ge->qa_KeyBoard->finishInputFlag = false;
							ge->qa_KeyBoard->userInput = mapData[this->displayPos].makerName;
						}
					}
					//作者名設定・セーブ・終了処理
					else if (this->makerNameInput == "") {
						ge->qa_KeyBoard->messageStr = "作者名を入力";
						ge->qa_KeyBoard->userInputLimit = 20;
						if (true == ge->qa_KeyBoard->finishInputFlag) {
							this->makerNameInput = ge->qa_KeyBoard->userInput;
							//セーブ
							ge->qa_MapMaker->SaveToFile(this->stageNameInput, this->makerNameInput);
							ge->KillAll_G("キーボード");
							//ロードタスク出現
							AppearLoad(10, 3, 20);
							this->appearLoad = true;
							this->saving = false;
						}
					}
				}
			}


		}

		//ロード処理
		if (true == this->appearLoad) {
			if (true == ge->finishedLoadAppear) {
				if (true == this->backToTitle) {
					Title::Object::Create(true);
				}
				//モードごとに処理を変える
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
				//ロードを消滅させる
				DisappearLoad(3, 20);

				this->Kill();
				ge->KillAll_G("マップ選択背景");
				ge->KillAll_G("マップ選択確認画面");
				ge->KillAll_G("マップ制作者名表示");
				ge->KillAll_G("マップ選択マップ名表示");
				ge->KillAll_G("マップ選択セレクター");
				ge->KillAll_G("マップ選択スタート地点表示用");
				ge->KillAll_G("マップ選択メッセージ");
				ge->KillAll_G("マップ選択ページUI");

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
	//マップ情報の読み込み
	void Object::LoadMapFile()
	{
		for (int i = 0; i < 15; ++i) {
			string folderPath = "./data/MapMaker/MapData/Data" + to_string(i) + '/';
			//フォルダー自体がない場合は作る
			int makeResult = _mkdir(folderPath.c_str());

			//読み込み
			ifstream fin(folderPath + "OriginalMap_MapFileData.txt");
			if (!fin) {		//失敗時
				this->mapData[i].mapName = "NoData";
			}
			else {			//成功時
				//マップ名
				getline(fin, this->mapData[i].mapName);
				//作者名
				getline(fin, this->mapData[i].makerName);
				//マップ画像名
				fin >> this->mapData[i].imageFileName;
				//スタート地点マップデータ
				for (int y = 0; y < 11; ++y) {
					for (int x = 0; x < 14; ++x) {
						fin >> this->mapData[i].startPointMapData[y][x];
					}
				}
				fin.close();
			}

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