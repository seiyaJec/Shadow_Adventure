//-------------------------------------------------------------------
//マップ制作UIの表示・非表示ボタン
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_HideMakeMapUIButton.h"

namespace  HideMakeMapUIButton
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/HideUIBarButton.png");
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
		//画像
		this->render2D_Priority[1] = 0.3f;
		//画像サイズ
		this->SetImgSize(56, 12);
		//矩形
		this->hitBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//位置
		this->pos = ge->qa_MakeMapUI->pos;
		this->pos.y -= (ge->qa_MakeMapUI->imgSizeH / 2) + this->imgSizeH / 2;
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

		//UIバーの位置を原点として移動
		this->pos = ge->qa_MakeMapUI->pos;
		this->pos.y -= (ge->qa_MakeMapUI->imgSizeH / 2) + this->imgSizeH / 2;

		if (nullptr != ge->qa_Cursor) {
			//ボタン範囲内でクリックされたら表示非表示処理
			if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
					ge->qa_MakeMapUI->hideFlag = !ge->qa_MakeMapUI->hideFlag;
				}
			}
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
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

		//カーソル照準時等のアニメーション
		ML::Box2D src = this->srcBase;
		if (nullptr != ge->qa_Cursor) {
			//カーソル照準時
			if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				src.x += this->imgSizeW;
			}
			//UIバーが隠れている時
			if (true == ge->qa_MakeMapUI->hideFlag) {
				src.y += this->imgSizeH;
			}
		}
		this->res->img->Draw(draw, src);
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