//-------------------------------------------------------------------
//キーコンフィグ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_KeyConfig.h"

namespace  KeyConfig
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/KeyConfig.png");
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
		this->render2D_Priority[1] = 0.11f;
		//画像サイズ設定
		SetImgSize(440, 480);
		//ゲームモード
		this->gamemode = GameMode::Select;
		//矩形
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//座標
		this->pos.x = ge->screen2DWidth + this->imgSizeW / 2;
		this->pos.y = this->imgSizeH / 2 + 20;
		//移動関係
		this->defaultSpeed = 16;
		this->speedRate = 1;
		this->speedAttenuationRate = 0.964f;
		this->speedAcceleration = 1.1f;
		this->attenuations = 14;
		//フラグ
		this->appearFlag = false;
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
		auto inp = ge->in1->GetState();

		if (true == this->appearFlag) {
			//スクリーン内に表示するときの位置
			float onScreenPos = ge->screen2DWidth - this->imgSizeW / 2;
			if (this->pos.x > onScreenPos) {
				//スピードが減衰しながら表示
				float speed = this->defaultSpeed * this->speedRate;
				this->speedRate *= this->speedAttenuationRate;
				pos.x -= speed;
				if (this->pos.x <= onScreenPos) {
					this->pos.x = onScreenPos;
				}
			}
		}
		else {
			//スクリーンから完全に消える位置
			float offScreenPos = ge->screen2DWidth + this->imgSizeW / 2;
			if (this->pos.x < offScreenPos) {
				//スピードが加速しながら消滅
				float speed = this->defaultSpeed * this->speedRate;
				this->speedRate *= this->speedAcceleration;
				pos.x += speed;
				if (this->pos.x >= offScreenPos) {
					this->pos.x = offScreenPos;
				}
			}
		}

		//出現・消滅
		if (inp.Trigger.volumeL != 0) {
			if (false == this->triggerDowned) {
				this->appearFlag = !this->appearFlag;
				this->speedRate = 1;
				this->triggerDowned = true;
			}
		}
		else {
			this->triggerDowned = false;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		src.x += this->imgSizeW * this->gamemode;
		this->res->img->Draw(draw, src);
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