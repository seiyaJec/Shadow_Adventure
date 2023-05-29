//-------------------------------------------------------------------
//フォント自動設定タスク
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Font.h"

namespace  Font
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
		this->render2D_Priority[1] = 0.11f;
		this->finishSet = false;
		this->font = nullptr;
		this->displayStr = "";
		this->drawBase = ML::Box2D(0, 0, 0, 0);
		this->color = ML::Color(1, 1, 1, 1);
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		this->font.reset();

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
		this->font->Draw(draw, this->displayStr);
	}

	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//位置の設定
	void Object::SetPos(float x_, float y_)
	{
		this->pos.x = x_;
		this->pos.y = y_;
	}
	void Object::SetPos(const ML::Vec2& pos_)
	{
		this->pos = pos_;
	}

	//サイズの設定
	void Object::SetSize(int w_, int h_)
	{
		//設定済みなら処理しない
		if (true == this->finishSet) {
			return;
		}

		//サイズの設定
		this->fontSizeW = w_;
		this->fontSizeH = h_;
		//フォント生成
		this->font = DG::Font::Create("ＭＳ ゴシック", this->fontSizeW, this->fontSizeH);
		//矩形の設定
		this->drawBase = setCenterPoint(this->fontSizeW, this->fontSizeH);
	}

	//文字列の設定+描画矩形の再設定
	void Object::SetString(const string& str_)
	{
		this->displayStr = str_;
		this->drawBase.w = this->fontSizeW * (int)this->displayStr.size();
		this->drawBase.x = this->drawBase.w / -2;
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