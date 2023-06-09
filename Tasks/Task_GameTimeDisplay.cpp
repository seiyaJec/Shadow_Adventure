//-------------------------------------------------------------------
//時間計測表示
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameTimeDisplay.h"
#include  "Task_MapMakerGame.h"

namespace  GameTimeDisplay
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->font = DG::Font::Create("ＭＳ ゴシック", 22, 44);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->font.reset();
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
		this->render2D_Priority[1] = 0.25f;
		//位置情報
		this->pos.x = ge->screen2DWidth / 2.0f;
		//時間
		this->timeDisplay = "";
		//フォントサイズ
		this->fontSizeW = 22;
		this->fontSIzeH = 44;
		//矩形
		this->drawBase = setCenterPoint(this->fontSizeW, this->fontSIzeH);
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
		//ゲームタスク
		if (nullptr != ge->qa_Play) {
			this->timeDisplay = "TIME " + to_string(ge->qa_Play->timeCntGame);
			if (ge->qa_Play->timeCntGame > 100) {
				this->color = ML::Color(1, 0, 0, 0);
			}
			else {
				this->color = ML::Color(1, 1, 0.5f, 0.5f);
			}
		}
		//マップ制作内ゲームタスク
		else {
			auto play = ge->GetTask<MapMakerGame::Object>("ゲームモード", "マップ制作");
			if (nullptr == play) {
				return;
			}
			this->timeDisplay = "TIME" + to_string(play->timeCntGame);
			if (play->timeCntGame > 100) {
				this->color = ML::Color(1, 0, 0, 0);
			}
			else {
				this->color = ML::Color(1, 1, 0.5f, 0.5f);
			}
		}

		//サイズを文字列に合わせる
		this->drawBase.w = (int)this->timeDisplay.size() * this->fontSizeW;


		this->pos.x = ge->screen2DWidth / 2.0f;
		this->pos.y = this->drawBase.h / 2.0f;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		this->res->font->Draw(draw, this->timeDisplay,this->color);
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