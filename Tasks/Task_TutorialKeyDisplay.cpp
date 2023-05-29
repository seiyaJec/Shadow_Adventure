//-------------------------------------------------------------------
//チュートリアル用入力表示
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_TutorialKeyDisplay.h"

namespace  TutorialKeyDisplay
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgStd = DG::Image::Create("./data/image/Tutorial/KeyStandard.png");
		this->imgShift = DG::Image::Create("./data/image/Tutorial/KeyShift.png");
		this->imgRT = DG::Image::Create("./data/image/Tutorial/KeyRT.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgStd.reset();
		this->imgShift.reset();
		this->imgRT.reset();
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
		this->render2D_Priority[1] = 0.5f;
		//画像サイズ設定
		this->SetImgSize(32, 32);
		//矩形
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//フラグ関係
		this->useRT = false;
		this->useShift = false;
		this->pushing = false;
		this->playerInput = false;
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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		//ボタンが押されていたら画像変化
		if (true == this->pushing) {
			src.x += this->imgSizeW;
		}
		//Rトリガーの時の描画
		if (true == this->useRT) {
			this->res->imgRT->Draw(draw, src);
			//プレイヤーの入力がある場合は画像の追加
			if (true == this->playerInput) {
				src.x = this->imgSizeW * 2;
				this->res->imgRT->Draw(draw, src);
			}
		}
		//Shiftの時の描画
		else if (true == this->useShift) {
			this->res->imgShift->Draw(draw, src);
			//プレイヤーの入力がある場合は画像の追加
			if (true == this->playerInput) {
				src.x = this->imgSizeW * 2;
				this->res->imgShift->Draw(draw, src);
			}
		}
		//標準の描画
		else {
			this->res->imgStd->Draw(draw, src);
			//プレイヤーの入力がある場合は画像の追加
			if (true == this->playerInput) {
				src.x = this->imgSizeW * 2;
				this->res->imgStd->Draw(draw, src);
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