//-------------------------------------------------------------------
//マップ制作時間設定用ボタン
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MakerSetTimeButton.h"

namespace  MakerSetTimeButton
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/SetTimeButton.png");
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
		//画像優先度
		this->render2D_Priority[1] = 0.24f;
		//画像サイズ
		this->SetImgSize(21, 13);
		//矩形
		this->hitBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//ボタンモード
		this->buttonMode = TOP;
		//位置情報
		this->addPosY = 1;
		//オブジェクト
		this->linkingDisplay = nullptr;
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

		//位置調整
		this->pos = this->linkingDisplay->pos;
		this->pos.y += (this->linkingDisplay->fontSizeH / 2 + this->imgSizeH / 2 + this->addPosY) * (this->buttonMode * -1);
		//カーソルが無効なら処理しない
		if (nullptr != ge->qa_Cursor) {
			//カーソルが照準していて、
			if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				//左クリックされたら
				if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
					//数字を変更
					this->linkingDisplay->myNumber += this->buttonMode;

					//上限・下限を超えたらループ
					if (this->linkingDisplay->myNumber > 9) {
						this->linkingDisplay->myNumber = 0;
					}
					else if (this->linkingDisplay->myNumber < 0) {
						this->linkingDisplay->myNumber = 9;
					}

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

		//アニメーション
		ML::Box2D src = this->srcBase;
		if (this->buttonMode == ButtonMode::BOTTOM) {
			src.y += this->imgSizeH;
		}
		if (nullptr != ge->qa_Cursor) {
			if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				src.x += this->imgSizeW;
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