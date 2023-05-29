//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapMakerGame.h"
#include  "Task_Player.h"
#include  "Task_GameUIPlayerHP.h"
#include  "Task_DataPlayer.h"
#include  "Task_Cursor.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_Enemy.h"
#include  "Task_Map2D.h"
#include  "Task_Ending.h"
#include  "Task_MakerSetTimeDisplay.h"
#include  "Task_GameTimeDisplay.h"
#include  "Task_MakerSetPlayerHPDisplay.h"

namespace  MapMakerGame
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
		ge->imgDebug = DG::Image::Create("./data/image/DebugColor.png");
		ge->gameOverflag = false;
		ge->goalFlag = false;
		ge->retryFlag = false;
		//時間
		this->timeReseted = false;
		this->timeCntGameMax = this->translateTimeDisplay();
		this->timeCntGame = timeCntGameMax;
		this->timeCntGameDistance = 60;
		//フラグ
		this->stoppedTask = false;

		
		
		//取りあえず初期値設定
		//リソースを常駐させる
		this->residentResorce.push_back(Enemy00::Resource::Create());
		this->residentResorce.push_back(Effect00::Resource::Create());
		this->residentResorce.push_back(Effect01::Resource::Create());
		this->residentResorce.push_back(Attack00::Resource::Create());
		this->residentResorce.push_back(Attack01::Resource::Create());
		this->residentResorce.push_back(Attack02::Resource::Create());
		//★タスクの生成
		auto dataPlayer = ge->GetTask<DataPlayer::Object>("オブジェクトデータ", "プレイヤ");
		auto display = ge->GetTask<MakerSetPlayerHPDisplay::Object>("マップ制作UI", "プレイヤーHP設定ディスプレイ");
		//プレイヤーの生成
		auto player = Player::Object::Create(true);
		//制作時のプレイヤーオブジェクトがマップ範囲内にあれば座標設定
		if (ge->qa_Map->hitBase.Hit(dataPlayer->hitBase.OffsetCopy(dataPlayer->pos))) {
			player->pos = dataPlayer->pos;
		}
		else {
			player->pos = ML::Vec2(100, 100);
		}
		player->statHPMax = display->myNumber * 10;
		player->statHP = player->statHPMax;
		ge->qa_Player = ge->GetTask<Player::Object>("プレイヤ", "仮");
		//プレイヤーのHPの設定
		GameUIPlayerHP::Object::Create(true);
		//制限時間ディスプレイの設定
		GameTimeDisplay::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("ゲームモード");
		ge->KillAll_G("プレイヤ");
		ge->KillAll_G("ゲームオーバーシャドウ");
		ge->KillAll_G("敵");
		//ge->KillAll_G("エフェクト");
		ge->KillAll_G("攻撃");
		ge->KillAll_G("ゲームUI");
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
		auto inp = ge->in1->GetState( );

		//アクセス効率化のため（敵の検出を減らす）
		ge->qa_Enemys = ge->GetTasks<BChara>("敵");
		ge->qa_Player = ge->GetTask<Player::Object>("プレイヤ", "仮");

		//指定したフレーム数ごとに1カウント
		if (this->timeCnt % this->timeCntGameDistance == 0) {
			--this->timeCntGame;
		}
		//0になったらゲームオーバー
		if (this->timeCntGame < 0) {
			ge->gameOverflag = true;
			this->timeCntGame = 0;
		}

		++this->timeCnt;
		//マップ制作に切り替える処理
		if (this->timeCnt >= 10 && inp.B4.down
			 || true == ge->retryFlag) {
			//マップのサイズを最大に戻す
			ge->qa_Map->ChangeMapSize(100,100);

			ge->qa_MapMaker->gamemode = GameMode::Maker;
			ge->qa_MapMaker->timeCnt = 0;
			Cursor::Object::Create(true);
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
	//設定した制限時間を実装する処理
	int Object::translateTimeDisplay()
	{
		int gameTime = 0;

		auto displays = ge->GetTasks<MakerSetTimeDisplay::Object>("マップ制作UI", "時間設定ディスプレイ");
		for (auto it = displays->begin();
			it != displays->end();
			++it) 
		{
			gameTime += (*it)->myNumber * (*it)->numberPlace;
		}
		return gameTime;
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