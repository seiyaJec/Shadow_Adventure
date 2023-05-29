//-------------------------------------------------------------------
//マップ選択時の確認画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSelectCheck.h"
#include  "Task_MapSelectCheckSelector.h"

namespace  MapSelectCheck
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/MapSelectCheck.png");
		this->font = DG::Font::Create("ＭＳ ゴシック", 22, 44);
		this->fontMin = DG::Font::Create("ＭＳ ゴシック", 14, 28);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->font.reset();
		this->fontMin.reset();
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
		this->render2D_Priority[1] = 0.3f;
		//コントローラー
		this->controller = ge->in1;
		//画像サイズ
		this->SetImgSize(610, 260);
		//位置情報
		this->pos.x = (float)ge->screen2DWidth / 2;
		this->pos.y = (float)ge->screen2DHeight / 2;
		//矩形
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		this->srcBase.x = ge->qa_MapSelect->selectMode;
		//フラグ
		this->selectedFlag = false;
		this->selectOK = false;
		//マップ名
		this->mapName = "「" + ge->qa_MapSelect->mapData[ge->qa_MapSelect->displayPos].mapName + "」";
		//OKまでの距離
		this->OKNum = 1;
		if (ge->qa_MapSelect->selectMode == ge->qa_MapSelect->REMOVE) {
			this->OKNum = 2;
		}
		//選択
		this->selectingNum = 0;

		//★タスクの生成
		auto selector = MapSelectCheckSelector::Object::Create(true);
		selector->pos.y = this->pos.y + 70;
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
					//セレクター追加したらキル処理を入れるように

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		if (false == this->selectedFlag) {
			//左右ボタンで選択
			if (inp.LStick.BR.down) {
				if (this->selectingNum < this->OKNum) {
					++this->selectingNum;
				}
			}
			else if (inp.LStick.BL.down) {
				if (this->selectingNum > 0) {
					--this->selectingNum;
				}
			}
			//決定
			if (inp.B1.down) {
				this->selectedFlag = true;
				if (this->selectingNum == this->OKNum) {
					this->selectOK = true;
				}
				ge->KillAll_GN("マップ選択確認画面", "セレクター");
			}
		}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//画像
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			ML::Box2D src = this->srcBase;
			src.x = ge->qa_MapSelect->selectMode * this->imgSizeW;
			this->res->img->Draw(draw, src);
		}
		//文字
		{
			ML::Box2D draw = setCenterPoint((int)this->mapName.size() * 22, 44);
			draw.Offset(this->pos.x,this->pos.y - 44);
			this->res->font->Draw(draw, this->mapName, ML::Color(1, 0, 0, 0));
		}
		//メッセージ
		{
			ML::Box2D draw = setCenterPoint((int)this->checkStr[ge->qa_MapSelect->selectMode].size() * 14, 28);
			draw.Offset(this->pos);
			this->res->fontMin->Draw(draw, this->checkStr[ge->qa_MapSelect->selectMode], ML::Color(1, 0, 0, 0));
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