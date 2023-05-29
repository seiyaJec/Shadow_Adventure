//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Result.h"
#include  "Task_ResultBG.h"
#include  "Task_ResultScoreBoard.h"
#include  "Task_ResultButton.h"

namespace  Result
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
		ge->qa_Result = ge->GetTask<Result::Object>("リザルト");
		//コントローラー
		this->controller = ge->in1;
		//時間計測
		this->timeCnt = 0;
		this->clearTime = 0;
		//リトライ回数
		this->retryCount = 0;
		//ボタン
		this->selectingButton = 0;
		this->buttonDistanceClose = 80;
		//ファイルパス
		this->filePath = "";
		//★タスクの生成
		ResultBG::Object::Create(true);
		auto score = ResultScoreBoard::Object::Create(true);
		score->pos.x = ge->screen2DWidth / 2.0f;
		score->pos.y = 250;
		//戻るボタン
		{
			auto button = ResultButton::Object::Create(true);
			button->pos.x = ge->screen2DWidth / 2.0f - button->imgSizeW + buttonDistanceClose;
			button->pos.y = 440;
			button->orderNum = 0;
			button->displayStr = "back";
		}
		//リトライボタン
		{
			auto button = ResultButton::Object::Create(true);
			button->pos.x = ge->screen2DWidth / 2.0f + button->imgSizeW - buttonDistanceClose;
			button->pos.y = 440;
			button->orderNum = 1;
			button->displayStr = "again";
		}
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
		//ロード処理
		if (true == this->loading) {
			if (true == ge->finishedLoadAppear) {
				switch (this->selectingButton) {
					//ゲームもう一度
				case SelectMode::AGAIN:
					Play::Object::Create(true);
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Play;
					break;
					//マップセレクトへ
				case SelectMode::BACK:
					MapSelect::Object::Create(true);
					ge->qa_KeyConfig->gamemode = ge->qa_KeyConfig->Select;
					break;
				}
					DisappearLoad(3, 20);
					ge->KillAll_G("リザルト");
			}
		}



		auto inp = this->controller->GetState();

		//左右の移動
		if (inp.LStick.BR.down) {
			if (this->selectingButton == SelectMode::BACK) {
				this->selectingButton = SelectMode::AGAIN;
			}
			else {
				this->selectingButton = SelectMode::BACK;
			}
		}
		if (inp.LStick.BL.down) {
			if (this->selectingButton == SelectMode::AGAIN) {
				this->selectingButton = SelectMode::BACK;
			}
			else {
				this->selectingButton = SelectMode::AGAIN;
			}
		}

		//決定でロード開始
		if (inp.B1.down) {
			this->loading = true;
			switch (this->selectingButton) {
			case SelectMode::AGAIN:
				AppearLoad(10, 3, 20,ML::Color(1.0f, 1.0f, 0.8f, 0.5f));
				break;
				//マップセレクトへ
			case SelectMode::BACK:
				AppearLoad(10, 3, 20, ML::Color(1.0f, 0.8f, 1.0f, 0.8f));
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