//-------------------------------------------------------------------
//敵00（スライム）のマップデータ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy.h"
#include  "Task_DataEnemy.h"
#include  "Task_IconEnemy.h"
#include  "MyStructure.h"

namespace  Data_Enemy
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
		this->render2D_Priority[1] = 0.49f;
		this->type = ge->qa_IconEnemy->enemyType;
		this->InitializeByType();
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
		//セーブモード中は処理しない
		if (ge->qa_MapMaker->gamemode == GameMode::SAVE) {
			return;
		}
		//ゲームモードへ切り替えるときの処理
		if (ge->qa_MapMaker->gamemode == GameMode::Game) {
			//開始時に敵を一度だけ出現させる
			if (false == this->created) {
				this->EnemyAppearByType();
				this->created = true;
				this->moveMode = false;
				this->addedToMoveCount = false;
			}
			return;
		}
			this->DataEnemyUpDate();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		{
			ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->ApplySize(draw);
			ML::Box2D src(0, 0, 1, 1);
			ge->qa_MakeMapUI->res->cantPutColor->Draw(draw, src);
		}
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			this->ApplySize(draw);
			ML::Box2D src(0, 0, imgSizeW, imgSizeH);
			this->img->Draw(draw, src);
		}

	}

	//敵のタイプごとに初期化させる
	void Object::InitializeByType()
	{
		//画像の優先度
		this->render2D_Priority[1] = 0.49f;
		//設置判定
		this->puted = false;
		//サイズアップ最大
		this->addSizeMax = 2;
		switch (this->type) {
		case 0:
			this->img = ge->qa_IconEnemy->res->img00;
			this->SetImgSize(48, 48);
			this->hitBase = setCenterPoint(44, 36);
			this->hitBase.Offset(0, 6);
			this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
			break;
		case 1:
			this->img = ge->qa_IconEnemy->res->img01;
			this->SetImgSize(80, 80);
			this->hitBase = setCenterPoint(44, 36);
			this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
			break;
		}
	}
	
	//タイプごとに違う敵を出現させる
	void Object::EnemyAppearByType()
	{
		switch (this->type) {
		case 0:
		{
			auto enemy = Enemy00::Object::Create(true);
			enemy->pos = this->pos;
			break;
		}
		case 1:
		{
			auto enemy = Enemy01::Object::Create(true);
			enemy->pos = this->pos;
			break;
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
