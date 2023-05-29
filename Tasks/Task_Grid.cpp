//-------------------------------------------------------------------
//グリッド
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Grid.h"

namespace  Grid
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/Grid.png");
		this->canPutColor = DG::Image::Create("./data/image/MapMaker/canPut.png");
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
		this->render2D_Priority[1] = 0.5f;

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
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

		ML::Box2D drawBase(0, 0, 32, 32);
		//スクロール対応
		drawBase.Offset(-ge->camera2D.x, -ge->camera2D.y);
		{
			ML::Box2D src(0, 0, 32, 32);
			for (int y = 0; y < 100; ++y) {
				for (int x = 0; x < 100; ++x) {
					ML::Box2D draw = drawBase.OffsetCopy(x * 32, y * 32);
					this->res->img->Draw(draw, src);
				}
			}
		}

		//カーソルの位置のマスが黄色くなる処理
		if (ge->qa_Cursor != nullptr) {
			if (false == ge->qa_Cursor->movingObject) {
				if (ge->qa_Cursor->cursorMode == CursorMode::MAP) {
					if (true == ge->qa_Cursor->inGameMap) {
						int gridPosX = (int)ge->qa_Cursor->pos.x / 32;
						int gridPosY = (int)ge->qa_Cursor->pos.y / 32;
						gridPosX *= 32;
						gridPosY *= 32;
						ML::Box2D draw = drawBase.OffsetCopy(gridPosX, gridPosY);
						ML::Box2D src(0, 0, 32, 32);
						this->res->canPutColor->Draw(draw, src);
					}
				}
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