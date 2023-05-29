//-------------------------------------------------------------------
//タスク読み込み時ロードの背景
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_LoadBG.h"

namespace  LoadBG
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/LoadBG.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->cuts = 0;
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
		this->render2D_Priority[1] = 0.1f;
		//yとhは呼び出し時に決定させる
		//矩形
		this->drawBase.w = ge->screen2DWidth;
		this->drawBase.x = -this->drawBase.w / 2;
		this->srcBase = ML::Box2D(8, 8, 8, 8);
		//位置
		this->pos.x = this->drawBase.w + (this->drawBase.w / 2.0f);
		//順番
		this->orderNum = 0;
		//移動
		this->moveSpeed = 0;
		//判定
		this->appearFlag = false;
		this->disappearFlag = false;
		//計測
		this->appearCnt = 0;
		this->appearCntMax = 0;
		this->disappearCnt = 0;
		this->disappearCntMax = 0;
		//色
		this->color = ML::Color(1, 1, 1, 1);
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
		float centerOfScreenX = ge->screen2DWidth / 2.0f;

		//出現処理
		if (this->appearFlag == true) {
			//画面端になるまで移動させる
			if (this->pos.x > centerOfScreenX) {		
				//関数で決めたカウント数値になったら移動開始
				if (this->appearCnt >= appearCntMax) {	
					this->pos.x -= this->moveSpeed;
					//端を超えたら戻す
					if (this->pos.x <= centerOfScreenX) {	
						this->pos.x = centerOfScreenX;
						//最後のロードタスクなら終了を伝える
						if (true == this->returnFinish) {	
							ge->finishedLoadAppear = true;
						}
					}
				}
				++this->appearCnt;
			}

			//消滅処理
			else if (this->disappearFlag == true) {
				//指定したカウントになったら移動開始
				if (this->disappearCnt >= disappearCntMax) {
					//移動
					if (this->pos.x > -centerOfScreenX) {
						this->pos.x -= this->moveSpeed;
					}
					//最後のロードタスクなら終了を伝える
					else {
						if (true == this->returnFinish) {
							ge->finishedLoadDisappear = true;
						}
						this->Kill();
					}
				}
				++this->disappearCnt;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		this->res->img->Draw(draw, src, this->color);
	}
	
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//出現処理
	void Object::Appear(int cuts_,int orderNum_,int distance_, float speed_)
	{
		//分割数を設定
		this->cuts = cuts_;
		//自分を設定
		this->orderNum = orderNum_;
		//矩形の設定
		this->drawBase.h = ge->screen2DHeight / this->cuts;
		this->drawBase.y = -this->drawBase.h / 2;
		//指定した順番の中心の高さを求める
		this->pos.y = (this->drawBase.h * this->orderNum) + (this->drawBase.h / 2.0f);
		//出現の間隔を設定
		this->appearCntMax = distance_ * this->orderNum;
		//移動の速さの設定
		this->moveSpeed = speed_;
		//出現処理開始
		this->appearFlag = true;
		//最後の順番ならロード終了を返す役目を持たせる
		if (this->orderNum == this->cuts - 1) {
			this->returnFinish = true;
		}
	}

	//消滅処理
	void Object::Disappear(int distance_, float speed_)
	{
		//移動の速さの設定
		this->moveSpeed = speed_;
		//消滅の間隔を設定（順番が1の場合は設定しない
		this->disappearCntMax = distance_ * this->orderNum;
		//消滅処理開始
		this->disappearFlag = true;
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