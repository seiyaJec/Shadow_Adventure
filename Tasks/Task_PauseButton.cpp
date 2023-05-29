//-------------------------------------------------------------------
//中断のボタン
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_PauseButton.h"

namespace  PauseButton
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/GamePause/GamePauseButton.png");
		this->font = DG::Font::Create("ＭＳ ゴシック", 12, 24);
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
		this->render2D_Priority[1] = 0.15f;
		//画像サイズ
		this->SetImgSize(400, 60);
		//フォントサイズ
		this->fontSizeW = 12;
		this->fontSizeH = 24;
		//矩形
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, 8, 8);
		//出現
		this->appearWait = 0;
		this->appearTimeMax = 20;
		this->appearTime = this->appearTimeMax;
		this->appearSpeed = 8;
		//選択中移動
		this->selectingMove = 0;
		this->selectingMoveSpeed = 10;
		this->selectingMoveMax = 40;
		//選択中待機
		this->selectingAlphaDec = 0;
		this->selectingAlphaDecSpeed = 1;
		this->selectingAlphaDecMax = 45;
		//透明度減算中か
		this->isAlphaDec = false;
		//出現終了フラグ
		this->finishedAppear = false;
		//文字列
		this->displayStr = "";
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
		//出現待ち（順番により変わる）
		if (this->appearWait > 0) {
			--appearWait;
			return;
		}
		//出現処理
		if (this->appearTime > 0) {
			--this->appearTime;
			//0になったら終了を返す
			if (this->appearTime == 0) {
				this->finishedAppear = true;
			}
		}
		//自分が選択されていたら
		else if (ge->pauseSelectingButton == this->orderNum) {
			//アニメーション用のカウントを増やす
			if (this->selectingMove < this->selectingMoveMax) {
				this->selectingMove += this->selectingMoveSpeed;
				//最大ならカウントを止める
				if (this->selectingMove >= this->selectingMoveMax) {
					this->selectingMove = this->selectingMoveMax;
				}
			}
			//透明度変更処理
			if (true == this->isAlphaDec) {
				//透明度を上げる
				this->selectingAlphaDec += this->selectingAlphaDecSpeed;
				if (this->selectingAlphaDec >= this->selectingAlphaDecMax) {
					this->isAlphaDec = false;
					this->selectingAlphaDec = this->selectingAlphaDecMax;
				}
			}
			else {
				//透明度を下げる
				this->selectingAlphaDec -= this->selectingAlphaDecSpeed;
				if (this->selectingAlphaDec <= 0) {
					this->isAlphaDec = true;
					this->selectingAlphaDec = 0;
				}
			}
		}
		else {
			//アニメーション用のカウントを増やす
			if (this->selectingMove > 0) {
				this->selectingMove -= this->selectingMoveSpeed;
				//最大ならカウントを止める
				if (this->selectingMove <= 0) {
					this->selectingMove = 0;
				}
			}

		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//透明度
		float alpha = 1.0f;
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			//登場アニメーション処理
			if (this->appearTime > 0) {
				alpha = (this->appearTimeMax - this->appearTime) / (float)this->appearTimeMax;
				draw.x += this->appearTime * this->appearSpeed;
			}
			//選択時アニメーション処理
			else if(ge->pauseSelectingButton == this->orderNum) {
				draw.x += this->selectingMove;
				alpha = 1.0f - (this->selectingAlphaDec * 0.01f);
			}
			ML::Color color(alpha, 0.9f, 0.9f, 0.9f);
			this->res->img->Draw(draw, this->srcBase, color);
		}
		alpha = 1.0f;
		{
			ML::Box2D draw = setCenterPoint(this->fontSizeW * (int)this->displayStr.size(), this->fontSizeH);
			draw.Offset(this->pos);
			//登場アニメーション処理
			if (this->appearTime > 0) {
				draw.x += this->appearTime * this->appearSpeed;
				alpha = (this->appearTimeMax - this->appearTime) / (float)this->appearTimeMax;
			}
			//選択時アニメーション処理
			else if (ge->pauseSelectingButton == this->orderNum) {
				draw.x += this->selectingMove;
				alpha = 1.0f - (this->selectingAlphaDec * 0.01f);
			}
			ML::Color color(alpha, 0, 0, 0);
			this->res->font->Draw(draw, this->displayStr, color);
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