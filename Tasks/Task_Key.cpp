//-------------------------------------------------------------------
//ゲーム内キーボードのキー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Key.h"

namespace  Key
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/Key.png");
		this->font = DG::Font::Create("ＭＳ ゴシック", 18, 36);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->pos = ML::Vec2(100, 100);
		//文字
		this->mychar = '\0';
		//画像サイズ
		this->SetImgSize(40,40);
		//矩形
		this->hitBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, 40, 40);
		this->fontDrawBase = setCenterPoint(18, 36);
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
		//カーソル照準、左クリック
		if (true == ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			if (true == ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				//文字をキーボードに入れる
				string tentativeStr = ge->qa_KeyBoard->userInput;
				tentativeStr += this->mychar;
				if (tentativeStr.size() <= ge->qa_KeyBoard->userInputLimit) {
					ge->qa_KeyBoard->userInput = tentativeStr;
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			ML::Box2D src = this->srcBase;
			//カーソルヒットアニメーション
			if (true == ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
				src.x = this->imgSizeW;
				if (true == ge->qa_Cursor->CheckLeftState(BuState::ON)) {
					src.x = this->imgSizeW * 2;
				}
			}
			this->res->img->Draw(draw, src);
		}
		{
			ML::Box2D draw = this->fontDrawBase.OffsetCopy(this->pos);
			this->res->font->Draw(draw, mychar);
			//ge->qa_KeyBoard->res->fontObj.Draw(mychar,this->pos,48,48);
			
		}

	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	


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