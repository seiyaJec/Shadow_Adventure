//-------------------------------------------------------------------
//タイトル用ゲームセレクター
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameSelector.h"
#include  "Task_Button.h"
#include  "Task_Effect.h"

namespace  GameSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Title/GameSelector.png");
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
		this->render2D_Priority[1] = 0.29f;
		this->SetImgSize(208, 130);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		this->animDistance = 40;
		this->pressStartFlag = false;
		this->pos = ge->qa_Title->qa_ButtonPlay->pos;
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
		if (false == this->pressStartFlag) {
			switch (ge->qa_Title->gameMode) {
			case 0:
			{
				this->pos = ge->qa_Title->qa_ButtonPlay->pos;
				break;
			}
			case 1:
			{
				this->pos = ge->qa_Title->qa_ButtonMake->pos;
				break;
			}
			case 2:
			{
				this->pos = ge->qa_Title->qa_ButtonEndless->pos;
				break;
			}
			}


			//アニメーション（画像拡縮）
			if (this->animCnt < this->animDistance) {
				this->addSize = 0;
			}
			else if (this->animCnt < this->animDistance * 2) {
				this->addSize = 1;
			}
			else {
				this->animCnt = 0;
			}

			if (ge->qa_Title->startingGame == true) {
				this->pressStartFlag = true;
				auto efct = Effect03::Object::Create(true);
				efct->Circle(this->pos, 50, 8);
			}
		}
		else {
		}

		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if (false == this->pressStartFlag) {
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			this->ApplySizeMove(draw);
			ML::Box2D src = this->srcBase;
			this->res->img->Draw(draw, src);
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