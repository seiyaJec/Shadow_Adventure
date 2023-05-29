//-------------------------------------------------------------------
//ゲームオーバー時に出現するシャドウ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameOverShadow.h"

namespace  GameOverShadow
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgR = DG::Image::Create("./data/image/Player/PlayerShadowR.png");
		this->imgL = DG::Image::Create("./data/image/Player/PlayerShadowL.png");
		LoadAnim(this->anim, "./data/animation/gameOverShadow.txt");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgR.reset();
		this->imgL.reset();
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
		this->render2D_Priority[1] = 0.51f;
		//矩形
		this->drawBase = setCenterPoint(100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		//位置情報
		this->pos = ge->qa_Player->pos;
		//左右
		this->angle_LR = ge->qa_Player->angle_LR;
		Angle_LR_Reverse(this->angle_LR);

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
		//プレイヤーと一緒に上昇する
		if (ge->qa_Player->deathCount >= ge->qa_Player->deathCountMax) {
			this->pos.y -= ge->qa_Player->deathDisappearSpeed;
			this->moveCnt = 1;
		}


	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase;
		this->AnimSet(this->res->anim, this->moveCnt);
		ML::Box2D src = this->srcBase;
		//座標位置へ変換
		draw.Offset(this->pos);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//チュートリアル時は描画を変更
		DrawSetIfTuto(draw,src);
		//向きによって使う画像を変更
		if (this->angle_LR == Angle_LR::Left) {
			this->res->imgL->Draw(draw, src);
		}
		else {
			this->res->imgR->Draw(draw, src);
		}

		//デバッグ用矩形表示
		this->DebugDrawBox(this->hitBase);

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