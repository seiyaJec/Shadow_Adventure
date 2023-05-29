//-------------------------------------------------------------------
//タイトル
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_TitlePlayer.h"
#include  "Task_TitleLogo.h"
#include  "Task_TitleMessage.h"
#include  "Task_LoadBG.h"
#include  "Task_Button.h"
#include  "Task_GameSelector.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_Enemy.h"
#include  "Task_MapSelect.h"
#include  "Task_Map2D.h"
#include  "Task_Ending.h"
#include  "Task_KeyConfig.h"

namespace  Title
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
		//キーコンフィグをなくす
		ge->qa_KeyConfig = nullptr;
		ge->KillAll_G("キーコンフィグ");

		//コントローラー
		this->controller = ge->in1;
		ge->qa_Title = ge->GetTask<Title::Object>("タイトル", "NoName");
		ge->imgDebug = DG::Image::Create("./data/image/DebugColor.png");
		ge->camera2D = ML::Box2D(
			0,
			0,
			ge->screen2DWidth,
			ge->screen2DHeight
		);//取りあえず初期値設定


		//ロゴデータ読み込み
		this->LoadLogoData();
		//ロゴの位置設定
		this->logoPosX = ge->screen2DWidth / 15;

		//ゲーム開始判定
		this->startingGame = false;

		//リソースを常駐させる
		this->residentResorce.push_back(Enemy00::Resource::Create());
		this->residentResorce.push_back(Effect00::Resource::Create());
		this->residentResorce.push_back(Effect01::Resource::Create());
		this->residentResorce.push_back(Effect02::Resource::Create());
		this->residentResorce.push_back(Attack00::Resource::Create());
		this->residentResorce.push_back(Attack01::Resource::Create());
		this->residentResorce.push_back(Attack02::Resource::Create());
		//★タスクの生成
		//マップの生成
		auto map = Map2D::Object::Create(true);
		map->Load("./data/map/Titlemap.txt");
		//プレイヤの生成
		auto pl = TitlePlayer::Object::Create(true);
		pl->pos.x = 30;
		pl->pos.y = 440;
		ge->qa_TPlayer = pl;
		//プレイヤ（影）の生成
		auto sdw = TitlePlayer::Object::Create(true);
		sdw->render2D_Priority[1] = 0.51f;
		sdw->imgSwap = true;
		sdw->pos.x = 30;
		sdw->pos.y = 130;
		this->qa_Shadow = sdw;
		//敵の生成
		auto enemy = Enemy00::Object::Create(true);
		enemy->Angle_LR_Reverse(enemy->angle_LR);
		enemy->pos.x = 800;
		enemy->pos.y = 440;


		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//リソースの常駐を解除する（書かなくても勝手に解除される）
		this->residentResorce.clear();
		//★データ＆タスク解放
		ge->KillAll_G("タイトルロゴ");
		ge->KillAll_GN("プレイヤ","タイトル");
		ge->KillAll_G("ゲームセレクター");
		ge->KillAll_G("ボタン");
		ge->KillAll_G("フィールド");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			//auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		//アクセス効率化のため（敵の検出を減らす）
		ge->qa_Enemys = ge->GetTasks<BChara>("敵");
		ge->qa_Map = ge->GetTask<Map2D::Object>("フィールド", "マップ");

		//タイトル表示・ゲームモード選択
		if (false == this->startingGame) {
			if (this->timeCnt <= 350) {
				if (this->timeCnt % 25 == 0) {
					int logoNum = timeCnt / 25;
					//タイトルロゴ出現
					auto logo = TitleLogo::Object::Create(true);
					logo->srcBase = logoData[logoNum];
					logo->drawBase.w = logoData[logoNum].w;
					logo->drawBase.h = logoData[logoNum].h;
					logo->pos.x = (float)(logoPosX);

					//エフェクト表示
					auto efct = Effect02::Object::Create(true);
					efct->pos.x = logo->pos.x + (logoData[logoNum].w / 2);
					efct->pos.y = logo->pos.y + (logoData[logoNum].h / 2);

					this->logoPosX += logoData[logoNum].w;
				}
				//表示シーン省略
				if (inp.B1.down) {
					++this->timeCnt;
					while (this->timeCnt <= 350) {
						if (this->timeCnt % 25 == 0) {
							int logoNum = timeCnt / 25;

							auto logo = TitleLogo::Object::Create(true);
							logo->srcBase = logoData[logoNum];
							logo->drawBase.w = logoData[logoNum].w;
							logo->drawBase.h = logoData[logoNum].h;
							logo->pos.x = (float)(logoPosX);
							logo->moveCnt = 50;

							this->logoPosX += logoData[logoNum].w;
						}
						++this->timeCnt;
					}
					--this->timeCnt;
				}

			}
			else{
				if (this->timeCnt == 351) {
					//メッセージ出現
					TitleMessage::Object::Create(true);
				}
				//決定ボタンを押したらゲームスタート処理
				if (inp.B1.down) {
					AppearLoad(10, 3, 20, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
					this->startingGame = true;
				}
			}
		}
		else {
			if (true == ge->finishedLoadAppear)
			{
				MapSelect::Object::Create(true);
				ge->qa_KeyConfig = KeyConfig::Object::Create(true);
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->GameMode::Select;
				
				//ロードを消滅させる
				DisappearLoad(3, 20);

				this->Kill();
				ge->KillAll_G("タイトルロゴ");
				ge->KillAll_GN("プレイヤ", "タイトル");
				ge->KillAll_G("ゲームセレクター");
				ge->KillAll_G("ボタン");
				ge->KillAll_G("フィールド");
				ge->KillAll_G("敵");
				ge->KillAll_G("タイトルメッセージ");
			}
		}


		++this->timeCnt;
		if (inp.ST.down) {
			//自身に消滅要請
			this->Kill();
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
	//タイトルのロゴを読み込む
	void Object::LoadLogoData()
	{
		ifstream fin("./data/title/logoData.txt");
		//読み込めなかったらすべて0で初期化
		if (!fin) {
			for (int i = 0; i < 15; ++i) {
				this->logoData[i] = ML::Box2D(0, 0, 0, 0);
			}
			return;
		}

		for (int i = 0; i < 15; ++i) {
			fin >> this->logoData[i].x;
			fin >> this->logoData[i].y;
			fin >> this->logoData[i].w;
			fin >> this->logoData[i].h;
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