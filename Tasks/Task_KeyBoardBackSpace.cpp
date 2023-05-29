//-------------------------------------------------------------------
//キーボードのバックスペースキー
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_KeyBoardBackSpace.h"

namespace  KeyBoardBackSpace
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/KeyBoard_BackSpace.png");
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
		this->render2D_Priority[1] = 0.25f;
		//位置情報
		this->pos = ML::Vec2(420, 460);
		//画像サイズ
		this->SetImgSize(60, 60);
		//矩形
		this->hitBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
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
		//照準、カーソル左クリックで処理
		if (true == ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			if (true == ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				string& userInput = ge->qa_KeyBoard->userInput;
				if (userInput.size() > 0) {
					if (userInput.size() == 1) {
						userInput = userInput.substr(0, userInput.size() - 1);
						return;
					}
					//半角なら1個分消す
					if (false == this->CheckLastFullWidth(userInput)) {
						string newStr = userInput.substr(0, userInput.size() - 1);
						userInput = newStr;
					}
					//全角なら2個分消す
					else {
						string newStr = userInput.substr(0, userInput.size() - 2);
						userInput = newStr;
					}
				}
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//通常処理
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		this->res->img->Draw(draw, src);
		//カーソル照準時の追加処理
		if (true == ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			src.x += this->imgSizeW;
			if (true == ge->qa_Cursor->CheckLeftState(BuState::ON)) {
				src.x += this->imgSizeW;
			}
			this->res->img->Draw(draw, src);
		}
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//全角か半角か調べる
	bool Object::CheckLastFullWidth(const string& str_)
	{
		string checkStr = str_.substr(str_.size() - 2, 2);

		//キーボードにある配列から判定する
		for (int i = 0; i < ge->qa_KeyBoard->res->HiraganaArrSize; ++i)
		{
			string fullChar = ge->qa_KeyBoard->res->HiraganaArr[i];
			if (checkStr == fullChar) {
				return true;
			}
		}
		return false;
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