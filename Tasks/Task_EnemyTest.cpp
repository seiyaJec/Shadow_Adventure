//-------------------------------------------------------------------
//敵キャラクタ（当たり判定テスト用）
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_EnemyTest.h"

namespace  EnemyTest
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/HitTest.bmp");
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
		//表示優先度
		this->render2D_Priority[1] = 0.4f;
		//当たり判定矩形
		this->hitBase = setCenterPoint(32, 32);
		//ステータス
		this->statHPMax = 20;
		this->statHP = this->statHPMax;
		this->statAttack = 10;
		//被ダメージ処理関係
		this->damagedVecMax = ML::Vec2(10, -10);
		this->damagedCntMax = 20;
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
		ML::Vec2 est(0, 0);
		if (true == this->damaged) {
			if (this->moveCnt > damagedCntMax) {
				//落下速度の制限
				if (this->fallSpeed > this->fallSpeedMax) {
					this->fallSpeed = this->fallSpeedMax;
				}
				//移動
				this->moveVec.x -= damagedSlowdownX * Angle_LR_ChangeAngleToInt(this->damagedAngle_LR);
				//落下処理
				this->moveVec.y += ge->gravity;
				est = this->moveVec;

				this->CheckMove(est);

				//移動がなくなったら終了処理
				if (true == this->IsZeroDamagedMove()) {
					if (this->statHP <= 0) {
						this->Kill();
						return;
					}
					this->damaged = false;
					return;
				}
			}
		}
		else {
			this->CheckDamageToPlayer();
		}
		++this->moveCnt;

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-16, -16, 32, 32);
		draw.Offset(this->pos);
		ML::Box2D src(100, 0, 100, 100);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//色設定
		ML::Color drawColor(1, 1, 1, 1);
		if (true == this->damaged) {
			drawColor = ML::Color(0.5f, 1, 1, 1);
			if (moveCnt <= 20) {
				draw.x += (rand() % 10) - 5;
			}
		}
		this->res->img->Draw(draw, src, drawColor);

		//デバッグ用矩形表示
		if (this->moveCnt >= 10) {
			this->DebugDrawBox(this->hitBase);
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