//-------------------------------------------------------------------
//プレイヤーのデータ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_DataPlayer.h"

namespace  DataPlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/Player/PlayerR.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		//画像の優先度
		this->render2D_Priority[1] = 0.48f;
		//位置情報
		this->screenPos = ML::Vec2(ge->camera2D.w / 2.0f, ge->camera2D.h / 2.0f);
		//設置
		this->puted = true;
		//画像サイズ
		this->SetImgSize(100, 74);
		//サイズ上限
		this->addSizeMax = 1;
		//矩形
		this->hitBase = ML::Box2D(-15, -21, 30, 58);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//★タスクの生成

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
		//ゲーム中の処理
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			this->moveMode = false;			//移動モードリセット
			this->addedToMoveCount = false; //こちらも同様
			return;
		}

		//未設置の処理
		if (false == this->puted) {
			this->addSize = 0;
			this->moveMode = true;
			this->screenPos.x = -(ge->camera2D.x - ge->qa_Cursor->pos.x);
			this->screenPos.y = -(ge->camera2D.y - ge->qa_Cursor->pos.y);
			if (ge->qa_Cursor->CheckLeftState(BuState::OFF)) {
				if (false == ge->qa_Cursor->inGameMap) {
					this->screenPos = ML::Vec2(ge->camera2D.w / 2.0f, ge->camera2D.h / 2.0f);
				}
				this->puted = true;
			}
		}
		//設置中の処理
		else {
			this->InCursorSizeUp();
			if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				this->moveMode = true;
				if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
					this->puted = false;
				}
			}
		}


		this->pos.x = ge->camera2D.x + screenPos.x;
		this->pos.y = ge->camera2D.y + screenPos.y;
		this->ApplyToMoveCount();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		this->Draw(this->res->img);
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