//-------------------------------------------------------------------
//マップの名前文字列
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapNameDisplay.h"

namespace  MapNameDisplay
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
		this->render2D_Priority[1] = 0.49f;
		//マップ名
		this->fileNumString = "File";
		//位置情報
		this->pos = ML::Vec2(285, 100);		//中心215,100
		//順番
		this->orderNum = 0;
		//色
		this->color = ML::Color(1, 0, 0, 0);
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
		//自分のファイル
		int fileNum = (ge->qa_MapSelect->page * 3) + this->orderNum;
		//ファイルのマップ名を受け取る
		nameString = ge->qa_MapSelect->mapData[fileNum].mapName;

		//ファイル名を文字列化
		this->fileNumString = "File" + to_string(fileNum);

		//自分が選択されていたら色変更
		if (ge->qa_MapSelect->selectingPos == this->orderNum) {
			this->color = ML::Color(1, 1, 1, 1);
		}
		else {
			this->color = ML::Color(1, 0, 0, 0);
		}
		this->drawBase = setCenterPoint(((int)this->nameString.size() * 22), 44);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			this->res->font->Draw(draw, this->nameString, this->color);
		}
		{
			ML::Box2D draw = setCenterPoint(((int)this->fileNumString.size() * 22), 44);
			draw.Offset(100.0f, this->pos.y);
			this->res->font->Draw(draw, this->fileNumString, this->color);
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