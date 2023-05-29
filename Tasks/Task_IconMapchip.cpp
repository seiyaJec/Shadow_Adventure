//-------------------------------------------------------------------
//マップ選択アイコン
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_IconMapchip.h"
#include  "Task_DataMapChip.h"
#include  "Task_Map2D.h"

namespace  IconMapchip
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgMapchip = ge->qa_Map->img;
		this->cantPutColor = ge->qa_MakeMapUI->res->cantPutColor;
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
		this->render2D_Priority[1] = 0.25f;
		this->SetImgSize(32, 32);
		//矩形
		this->hitBase = setCenterPoint(32, 32);
		this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
		this->srcBase = ML::Box2D(0, 0, 32, 32);
		//現在のマップチップの種類
		this->mapchipMode = MapchipMode::GROUND;
		this->mapchipModeSize = 4;
		//★タスクの生成
		DataMapChip::Object::Create(true);

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
		auto inp = ge->qa_MapMaker->controller->GetState();
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		this->pos = ge->qa_MakeMapUI->pos;
		this->pos += ML::Vec2(50, 0);

		//クリックされたらモード変更
		if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				ge->qa_Cursor->cursorMode = CursorMode::MAP;
			}
		}

		//B3でそれぞれタイプ切り替え
		if (true == inp.B3.down) {
			switch (ge->qa_Cursor->cursorMode) {
			case CursorMode::MAP:
				++this->mapchipMode;
				if (this->mapchipMode == this->mapchipModeSize) {
					this->mapchipMode = 1;
				}
				break;
			}
		}
		
		//アイコンの画像をタイプごとに変更
		this->srcBase.x = (ge->qa_Map->chipArr[this->mapchipMode][0] % ge->qa_Map->columns)
							* ge->qa_Map->length;
		this->srcBase.y = (ge->qa_Map->chipArr[this->mapchipMode][0] / ge->qa_Map->columns)
							* ge->qa_Map->length;

		//サイズの変更
		this->InCursorSizeUp();

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		this->Draw(this->res->imgMapchip);
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