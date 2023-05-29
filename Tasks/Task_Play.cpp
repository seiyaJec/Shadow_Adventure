//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Play.h"
#include  "Task_Player.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_Enemy.h"
#include  "Task_Map2D.h"
#include  "Task_Ending.h"
#include  "Task_GameDebug.h"
#include  "Task_Result.h"
#include  "Task_BeforePlay.h"
#include  "Task_GameUIPlayerHP.h"
#include  "Task_GameUIGoalDisplay.h"
#include  "Task_GameRetry.h"
#include  "Task_GameTimeDisplay.h"
#include  "Task_GamePause.h"

namespace  Play
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->fontDebug = DG::Font::Create("ＭＳ ゴシック",16,32);
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
		//フラグ
		ge->goalFlag = false;
		ge->gameOverflag = false;
		ge->imgDebug = DG::Image::Create("./data/image/DebugColor.png");
		ge->camera2D = ML::Box2D(
			ge->screen2DWidth/3,
			ge->screen2DHeight/3,
			ge->screen2DWidth,
			ge->screen2DHeight
		);//取りあえず初期値設定
		//ゲーム用ファイル読み込み
		this->dataForPlay.Load(ge->stageFilePath);
		//時間設定
		this->timeReseted = false;
		this->timeCntGameMax = this->dataForPlay.gameTime;
		this->timeCntGame = timeCntGameMax;
		this->timeCntGameDistance = 60;
		//フラグ
		this->stoppedTask = false;

		//リソースを常駐させる
		this->residentResorce.push_back(Enemy00::Resource::Create());
		this->residentResorce.push_back(Effect00::Resource::Create());
		this->residentResorce.push_back(Effect01::Resource::Create());
		this->residentResorce.push_back(Attack00::Resource::Create());
		this->residentResorce.push_back(Attack01::Resource::Create());
		this->residentResorce.push_back(Attack02::Resource::Create());
		//★タスクの生成

		//最初の処理（リトライじゃない処理）
		if (false == ge->retryFlag) {
			//マップの生成
			auto map = Map2D::Object::Create(true);
			map->Load(ge->stageFilePath);
			//プレイヤの生成
			auto pl = Player::Object::Create(true);
			pl->pos = this->dataForPlay.playerStartPos;
			pl->statHPMax = this->dataForPlay.playerHPMax;
			pl->statHP = this->dataForPlay.playerHPMax;
			//リトライ用タスク生成
			GameRetry::Object::Create(true);
		}
		//リトライ処理
		else {
			auto retry = ge->GetTask < GameRetry::Object>("リトライ");
			++retry->retryCount;
			//マップの生成
			auto map = Map2D::Object::Create(true);
			map->Load(ge->stageFilePath);
			//プレイヤの生成
			auto pl = Player::Object::Create(true);
			pl->SetRetryPos(this->dataForPlay.playerStartPos);
			pl->statHPMax = this->dataForPlay.playerHPMax;
			pl->statHP = this->dataForPlay.playerHPMax;
			pl->retryAppear = true;
		}
		//プレイヤーHPUI
		GameUIPlayerHP::Object::Create(true);
		//制限時間
		GameTimeDisplay::Object::Create(true);
		//雲
		for (int i = 0; i < 20; ++i) {
			auto effect = Effect04::Object::Create(true);
			int basePosX = i * 150;
			effect->pos = ML::Vec2((float)(basePosX + rand() % 100), (float)(rand() % 300));
		}
		//リトライフラグを消す
		ge->retryFlag = false;


		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		//リソースの常駐を解除する（書かなくても勝手に解除される）
		this->residentResorce.clear();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{

		//リトライ処理
		if (true == ge->retryFlag) {
			ge->KillAll_G("本編");
			ge->KillAll_G("フィールド");
			ge->KillAll_G("プレイヤ");
			ge->KillAll_G("敵");
			ge->KillAll_G("エフェクト");
			ge->KillAll_G("攻撃");
			ge->KillAll_G("ゲームUI");
			ge->KillAll_G("ゲームオーバーシャドウ");
			Play::Object::Create(true);
			return;
		}



		auto inp = ge->in1->GetState( );

		//アクセス効率化のため（敵の検出を減らす）
		ge->qa_Play = ge->GetTask<Play::Object>("本編", "統括");
		ge->qa_Enemys = ge->GetTasks<BChara>("敵");
		ge->qa_Map = ge->GetTask<Map2D::Object>("フィールド", "マップ");
		ge->qa_Player = ge->GetTask<Player::Object>("プレイヤ", "仮");

		


		//ゴール処理
		if (true == ge->goalFlag) {
			//一度だけ時間リセット
			if (false == this->timeReseted) {
				this->timeCnt = 0;
				this->timeReseted = true;
			}

			//指定した時間になったらシーン切り替え
			if (this->timeCnt >= 200) {
				if (this->timeCnt == 200) {
					AppearLoad(10, 3, 20, ML::Color(1.0f, 1.0f, 1.0f, 0.5f));
				}
				if (true == ge->finishedLoadAppear) {
					auto retry = ge->GetTask<GameRetry::Object>("リトライ");
					auto result = Result::Object::Create(true);
					result->clearTime = this->timeCntGameMax - this->timeCntGame;
					result->retryCount = retry->retryCount;
					result->filePath = ge->stageFilePath;
					ge->retryFlag = false;
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->ResultCheck;
					ge->KillAll_G("本編");
					ge->KillAll_G("フィールド");
					ge->KillAll_G("プレイヤ");
					ge->KillAll_G("敵");
					ge->KillAll_G("エフェクト");
					ge->KillAll_G("攻撃");
					ge->KillAll_G("ゲームUI");
					ge->KillAll_G("ゲームオーバーシャドウ");
					ge->KillAll_G("リトライ");
					DisappearLoad(3, 20);
				}
			}

		}
		//ゴールしてなければ時間計測
		else {
			//指定したフレーム数ごとに1カウント
			if (this->timeCnt % this->timeCntGameDistance == 0) {
				--this->timeCntGame;
			}
			//0になったらゲームオーバー
			if (this->timeCntGame < 0) {
				ge->gameOverflag = true;
				this->timeCntGame = 0;
			}

			//スタートボタンを押されたらゲーム中断
			if (inp.ST.down) {
				ge->pauseFlag = true;
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Pause;
				this->stoppedTask = false;
				//タスクを一時停止
				ge->StopAll_G("本編");
				ge->StopAll_G("フィールド");
				ge->StopAll_G("プレイヤ");
				ge->StopAll_G("敵");
				ge->StopAll_G("エフェクト");
				ge->StopAll_G("攻撃");
				ge->StopAll_G("ゲームUI");
				ge->StopAll_G("ゲームオーバーシャドウ");
				ge->StopAll_G("リトライ");
				GamePause::Object::Create(true);
			}



		}
		++this->timeCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}

	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------




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