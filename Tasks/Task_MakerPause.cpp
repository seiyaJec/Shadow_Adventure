//-------------------------------------------------------------------
//ゲーム中断
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MakerPause.h"
#include  "Task_PauseBG.h"
#include  "Task_PauseButton.h"
#include  "Task_PauseDisplay.h"

namespace  MakerPause
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
		ge->pauseSelectingButton = 0;
		//コントローラー
		this->controller = ge->in1;
		//カウント
		this->timeCnt = 0;
		//ボタン
		this->buttonMax = 4;
		this->buttonDistanceX = 0;
		this->buttonDistanceY = 100;
		this->buttonGenerateDistance = 10;
		this->appeardLastButton = false;
		//選択したボタンごとの処理フラグ
		this->loadingTutorial = false;
		this->finishedAppearTutorial = false;
		this->loadingSelect = false;
		this->loadingTitle = false;
		//★タスクの生成
		//ポーズ背景
		PauseBG::Object::Create(true);
		//ポーズ画面文字
		PauseDisplay::Object::Create(true);

		//ボタン出現処理
		ML::Vec2 firstButtonPos;
		firstButtonPos.x = (ge->screen2DWidth / 2.0f) - this->buttonDistanceX;
		firstButtonPos.y = (ge->screen2DHeight / 2.0f) - this->buttonDistanceY;
		this->GenerateButton(firstButtonPos, (float)this->buttonDistanceX, (float)this->buttonDistanceY, this->buttonGenerateDistance);

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//チュートリアル読み込み処理
		if (true == this->loadingTutorial) {
			//チュートリアル出現処理（ロード終了後1度だけ処理）
			if (false == this->finishedAppearTutorial) {
				if (true == ge->finishedLoadAppear) {
					this->SuspendMakerTask();
					ge->SuspendAll_G("ポーズ");
					this->Suspend(false);
					Tutorial::Object::Create(true);
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Tutorial;
					DisappearLoad(3, 20);
					this->finishedAppearTutorial = true;
				}
			}
			//チュートリアル中の処理
			else {
				auto tutorial = ge->GetTask<Tutorial::Object>("チュートリアル", "NoName");
				if (true == tutorial->finishTutorial) {
					if (true == ge->finishedLoadAppear) {

						tutorial->FinishTask();
						ge->SuspendAll_G("ポーズ", false);
						this->StopMakerTask();
						this->finishedAppearTutorial = false;
						this->loadingTutorial = false;
						ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Pause;
						DisappearLoad(3, 20);
					}
				}
			}
			return;
		}

		//ボタンがすべて出るまで処理させない
		if (false == this->appeardLastButton) {
			if (true == lastButton->finishedAppear) {
				this->appeardLastButton = true;
			}
			else {
				return;
			}
		}

		//マップセレクト読み込み
		if (true == this->loadingSelect) {
			if (true == ge->finishedLoadAppear) {
				this->KillMakerTask();
				ge->KillAll_G("ポーズ");
				MapSelect::Object::Create(true);
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Select;
				DisappearLoad(3, 20);
			}
		}

		//タイトル読み込み
		if (true == this->loadingTitle) {
			if (true == ge->finishedLoadAppear) {
				this->KillMakerTask();
				ge->KillAll_G("ポーズ");
				Title::Object::Create(true);
				DisappearLoad(3, 20);
			}
		}


		//コントローラー情報取得
		auto inp = this->controller->GetState();
		
		//ボタンの上下移動
		if (inp.LStick.BU.down) {
			if (ge->pauseSelectingButton > 0) {
				--ge->pauseSelectingButton;
			}
		}
		if (inp.LStick.BD.down) {
			if (ge->pauseSelectingButton < this->buttonMax - 1) {
				++ge->pauseSelectingButton;
			}
		}

		//決定でそれぞれの読み込み
		if (inp.B1.down) {
			switch (ge->pauseSelectingButton) {
			//再開
			case ButtonType::CONTINUE:
				this->RestartMakerTask();
				ge->KillAll_G("ポーズ", false);
				ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Maker;
				break;
			//チュートリアル
			case ButtonType::TUTORIAL:
				this->loadingTutorial = true;
				AppearLoad(10, 3, 20, ML::Color(1.0f, 0.3f, 0.6f, 0.3f));
				break;
			//マップセレクト
			case ButtonType::SELECT:
				this->loadingSelect = true;
				AppearLoad(10, 3, 20, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
				break;
			//タイトル
			case ButtonType::TITLE:
				this->loadingTitle = true;
				AppearLoad(10, 3, 20);
				break;
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
	void Object::GenerateButton(ML::Vec2 startPos_, float addPosX_, float addPosY_, int generateDistance_)
	{
		ML::Vec2 buttonPos = startPos_;		//ボタンの位置
		ML::Vec2 addPos(addPosX_, addPosY_);//追加する位置
		int orderNumSet = 0;		//順番
		int appearWaitSet = 0;		//出現待機時間
		//ゲーム再開ボタン生成
		{
			auto button = PauseButton::Object::Create(true);
			button->pos = buttonPos;
			button->displayStr = "再開";
			button->orderNum = orderNumSet;
			button->appearWait = appearWaitSet;
		}
		buttonPos += addPos;
		++orderNumSet;
		appearWaitSet += generateDistance_;
		//チュートリアルボタン生成
		{
			auto button = PauseButton::Object::Create(true);
			button->pos = buttonPos;
			button->displayStr = "チュートリアル";
			button->orderNum = orderNumSet;
			button->appearWait = appearWaitSet;
		}
		buttonPos += addPos;
		++orderNumSet;
		appearWaitSet += generateDistance_;
		//マップセレクト生成
		{
			auto button = PauseButton::Object::Create(true);
			button->pos = buttonPos;
			button->displayStr = "マップセレクトに戻る";
			button->orderNum = orderNumSet;
			button->appearWait = appearWaitSet;
		}
		buttonPos += addPos;
		++orderNumSet;
		appearWaitSet += generateDistance_;
		//タイトル戻るボタン生成
		{
			this->lastButton = PauseButton::Object::Create(true);
			this->lastButton->pos = buttonPos;
			this->lastButton->displayStr = "タイトルに戻る";
			this->lastButton->orderNum = orderNumSet;
			this->lastButton->appearWait = appearWaitSet;
		}

	}
	//制作タスクを再開させる
	void Object::RestartMakerTask()
	{
		ge->StopAll_G("マップ生成", false);
		ge->StopAll_G("マップ制作UI",false);
		ge->StopAll_G("フィールド", false);
		ge->StopAll_G("アイコン", false);
		ge->StopAll_G("オブジェクトデータ", false);
		ge->StopAll_G("マップデータ", false);
		ge->StopAll_G("グリッド", false);
		ge->StopAll_G("カーソル", false);
		ge->StopAll_G("ゲームモード", false);
		ge->StopAll_G("プレイヤ", false);
		ge->StopAll_G("敵", false);
		ge->StopAll_G("エフェクト", false);
		ge->StopAll_G("攻撃", false);
		ge->StopAll_G("ゲームUI", false);
	}
	
	//制作タスクを無効状態にする
	void Object::SuspendMakerTask()
	{
		ge->StopAll_G("マップ生成", false);
		ge->StopAll_G("マップ制作UI", false);
		ge->StopAll_G("フィールド", false);
		ge->StopAll_G("アイコン", false);
		ge->StopAll_G("オブジェクトデータ", false);
		ge->StopAll_G("マップデータ", false);
		ge->StopAll_G("グリッド", false);
		ge->StopAll_G("カーソル", false);
		ge->StopAll_G("ゲームモード", false);
		ge->StopAll_G("プレイヤ", false);
		ge->StopAll_G("敵", false);
		ge->StopAll_G("エフェクト", false);
		ge->StopAll_G("攻撃", false);
		ge->StopAll_G("ゲームUI", false);

		ge->SuspendAll_G("マップ生成");
		ge->SuspendAll_G("マップ制作UI");
		ge->SuspendAll_G("フィールド");
		ge->SuspendAll_G("アイコン");
		ge->SuspendAll_G("オブジェクトデータ");
		ge->SuspendAll_G("マップデータ");
		ge->SuspendAll_G("カーソル");
		ge->SuspendAll_G("グリッド");
		ge->SuspendAll_G("ゲームモード");
		ge->SuspendAll_G("プレイヤ");
		ge->SuspendAll_G("敵");
		ge->SuspendAll_G("エフェクト");
		ge->SuspendAll_G("攻撃");
		ge->SuspendAll_G("ゲームUI");
	}

	//制作タスクを無効から停止状態にする
	void Object::StopMakerTask()
	{
		ge->SuspendAll_G("マップ生成", false);
		ge->SuspendAll_G("マップ制作UI", false);
		ge->SuspendAll_G("フィールド", false);
		ge->SuspendAll_G("アイコン", false);
		ge->SuspendAll_G("オブジェクトデータ", false);
		ge->SuspendAll_G("マップデータ", false);
		ge->SuspendAll_G("グリッド", false);
		ge->SuspendAll_G("カーソル", false);
		ge->SuspendAll_G("ゲームモード", false);
		ge->SuspendAll_G("プレイヤ", false);
		ge->SuspendAll_G("敵", false);
		ge->SuspendAll_G("エフェクト", false);
		ge->SuspendAll_G("攻撃", false);
		ge->SuspendAll_G("ゲームUI", false);

		ge->StopAll_G("マップ生成");
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
	}

	//制作タスクをキルする
	void Object::KillMakerTask()
	{
		ge->KillAll_G("マップ生成");
		ge->KillAll_G("マップ制作UI");
		ge->KillAll_G("フィールド");
		ge->KillAll_G("アイコン");
		ge->KillAll_G("オブジェクトデータ");
		ge->KillAll_G("マップデータ");
		ge->KillAll_G("カーソル");
		ge->KillAll_G("グリッド");
		ge->KillAll_G("ゲームモード");
		ge->KillAll_G("プレイヤ");
		ge->KillAll_G("敵");
		ge->KillAll_G("エフェクト");
		ge->KillAll_G("攻撃");
		ge->KillAll_G("ゲームUI");
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