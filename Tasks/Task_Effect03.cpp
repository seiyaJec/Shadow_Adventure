//-------------------------------------------------------------------
//エフェクト3　飛び散る光の粒
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect03.h"

namespace  Effect03
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Effect/Light.png");
		LoadAnim(this->anim, "./data/animation/Effect/Effect03.txt");
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
		this->render2D_Priority[1] = 0.2f;
		//画像サイズ
		this->SetImgSize(24, 24);
		//矩形
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//角度
		this->angle = 0;
		//アニメーション
		this->animCnt = 0;
		this->animDistance = 4;
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
		this->pos.x += cos(this->angle);
		this->pos.y += sin(this->angle);
		//アニメーションが最後まで行ったらキル
		if (this->animCnt >= this->res->anim.Max * this->animDistance) {
			this->Kill();
		}
		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		int animNum = this->animCnt / this->animDistance;

		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D src = this->srcBase.OffsetCopy(this->res->anim.Pos[animNum]);
		//回転
		float centerPosX = this->imgSizeW / 2.0f;
		float centerPosY = this->imgSizeH / 2.0f;
		float imgAng = this->angle + ML::ToRadian(90);
		this->res->img->Rotation(imgAng, ML::Vec2(centerPosX, centerPosY));
		//描画
		this->res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//まとめて数値を変えるときに使用
	void Object::Set(ML::Vec2 pos_, float angRad_, int animDistance_)
	{
		this->pos = pos_;
		this->angle = angRad_;
		this->animDistance = animDistance_;
	}
	//円形に出現させる
	void Object::Circle(ML::Vec2 pos_, float distance_, int efctSize_)
	{
		this->pos.x = pos_.x + cos(this->angle) * distance_;
		this->pos.y = pos_.y + sin(this->angle) * distance_;

		for (int i = 0; i < efctSize_; ++i) {
			float efctAngle = this->angle + ML::ToRadian(360.0f / efctSize_ * i);

			auto efct = Effect03::Object::Create(true);
			efct->pos.x = pos_.x + cos(efctAngle) * distance_;
			efct->pos.y = pos_.y + sin(efctAngle) * distance_;
			efct->angle = efctAngle;
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