//-------------------------------------------------------------------
//リザルト画面用スコアボード
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_ResultScoreBoard.h"

namespace  ResultScoreBoard
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Result/Result_ScoreBoard.png");
		this->font = DG::Font::Create("ＭＳ ゴシック", 33, 66);
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
		this->render2D_Priority[1] = 0.8f;
		//画像サイズ
		this->SetImgSize(592, 220);
		//フォントサイズ
		this->fontSizeW = 33;
		this->fontSizeH = 66;
		//矩形
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//表示用
		this->clearTime = "クリアタイム　";
		this->retryConut = "リトライ回数　";
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
		//背景
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			this->res->img->Draw(draw, this->srcBase);
		}
		//クリアタイム
		{
			string clearTimeDisplay = this->clearTime + to_string(ge->qa_Result->clearTime);
			ML::Box2D draw = setCenterPoint(
				this->fontSizeW * (int)clearTimeDisplay.size(),
				this->fontSizeH
			);
			ML::Vec2 clearTimePos;
			clearTimePos.x = this->pos.x;
			clearTimePos.y = this->pos.y - 50;
			draw.Offset(clearTimePos);
			this->res->font->Draw(draw, clearTimeDisplay, ML::Color(1, 0, 0, 0));
		}
		//リトライ回数
		{
			string retryCountDisplay = this->retryConut + to_string(ge->qa_Result->retryCount);
			ML::Box2D draw = setCenterPoint(
				this->fontSizeW * (int)retryCountDisplay.size(),
				this->fontSizeH
			);
			ML::Vec2 retryCountPos;
			retryCountPos.x = this->pos.x;
			retryCountPos.y = this->pos.y + this->fontSizeH - 10;
			draw.Offset(retryCountPos);
			this->res->font->Draw(draw, retryCountDisplay, ML::Color(1, 0, 0, 0));
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