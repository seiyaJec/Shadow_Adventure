//-------------------------------------------------------------------
//敵00：スライム
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy00.h"

namespace  Enemy00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		imgLeft = DG::Image::Create("./data/image/Enemy/Enemy00L.png");
		imgRight = DG::Image::Create("./data/image/Enemy/Enemy00R.png");
		LoadAnim(this->animIdle, "./data/animation/enemy/enemy00/Idle.txt");
		LoadAnim(this->animRunning, "./data/animation/enemy/enemy00/Running.txt");
		LoadAnim(this->animAttack, "./data/animation/enemy/enemy00/Attack.txt");
		LoadAnim(this->animDamaged, "./data/animation/enemy/enemy00/Damaged.txt");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		imgLeft.reset();
		imgRight.reset();
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
		//矩形
		this->hitBase = setCenterPoint(44, 36);
		this->MovePosToAngle(this->hitBase, 0, 6);
		this->drawBase = setCenterPoint(48, 48);
		this->srcBase = ML::Box2D(0, 0, 48, 48);
		this->playerSerchBox = setCenterPoint(200,50);
		//アニメーション
		this->animState = AnimState::IDLE;
		//ステータス
		this->statHPMax = 10;
		this->statHP = this->statHPMax;
		this->statAttack = 10;
		//移動
		this->jumpPow = -3;
		this->fallSpeedMax = 10;
		//被ダメージ処理関係
		this->damagedVecMax = ML::Vec2(7, -7);
		this->damagedCntMax = 20;
		//攻撃時の向き
		this->attackAngle = Angle_LR::Right;
		//デバッグ
		this->debugFlag = false;
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
				this->animState = AnimState::DAMAGED;
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
					this->animState = AnimState::IDLE;
					return;
				}
			}
		}
		else {
			//プレイヤーに攻撃処理
			this->CheckDamageToPlayer();
			if (this->animState != AnimState::ATTACK) {
				if (true == this->SerchPlayer()) {
					this->animState = AnimState::ATTACK;
					this->moveCnt = 0;
					this->animCnt = 0;
				}
			}

			//モーションごとに処理を変更
			switch (this->animState) {
				//待機
			case AnimState::IDLE:
				if (this->moveCnt >= 200) {
					this->animState = AnimState::RUNNING;
					this->moveCnt = 0;
					this->animCnt = 0;
				}
				break;
				//移動
			case AnimState::RUNNING: {
				//int checkCnt = animCnt / 10 % this->res->animRunning.Max;
				//if(checkCnt <)
				est.x += 0.1f * Angle_LR_ChangeAngleToInt(this->angle_LR);
				if (this->moveCnt >= 200) {
					this->animState = AnimState::IDLE;
					this->moveCnt = 0;
					this->animCnt = 0;
					}
				}
				break;
				//攻撃
			case AnimState::ATTACK:

				if (this->moveCnt >= 20) {
					est.x += 1 * Angle_LR_ChangeAngleToInt(this->angle_LR);
				}
				if (this->moveCnt >= 100) {
					this->animState = AnimState::IDLE;
					this->moveCnt = 0;
					this->animCnt = 0;
				}
				break;
				
			}
			//落下速度の制限
			if (this->fallSpeed > this->fallSpeedMax) {
				this->fallSpeed = this->fallSpeedMax;
			}
			//落下処理
			est.y += this->fallSpeed;

			this->CheckMove(est);
			
			//着地判定
			this->hitFlagFoot = this->CheckFoot();
			//着地処理
			if (true == this->hitFlagFoot) {
				if (this->fallSpeed > 0) {
					this->fallSpeed = 0; //落下速度0
				}
			}
			else {
				this->fallSpeed += ge->gravity;
			}
		}

		

		++this->moveCnt;
		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase;
		this->Anim(draw);
		ML::Box2D src = this->srcBase;
		//現在地へ移動
		draw.Offset(this->pos);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//チュートリアル時は描画を変更
		DrawSetIfTuto(draw,src);
		//色設定
		ML::Color drawColor(1, 1, 1, 1);
		if (true == this->damaged) {
			drawColor = ML::Color(0.5f, 1, 1, 1);
			if (moveCnt <= 20) {
				this->drawBase.x += (rand() % 10) - 5;
			}
		}
		if (this->angle_LR == Angle_LR::Right) {
			this->res->imgRight->Draw(draw, src, drawColor);
		}
		else {
			this->res->imgLeft->Draw(draw, src, drawColor);
		}

		//デバッグ用矩形表示
		if (this->moveCnt >= 10) {
			this->DebugDrawBox(this->hitBase);
		}
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	void Object::Anim(ML::Box2D& draw_)
	{
		int imgSizeW = 48;
		int imgSizeH = 48;
		draw_.x = imgSizeW / -2;
		draw_.y = imgSizeH / -2;
		int animNum = 0;
		switch (this->animState) {
			//静止
		case AnimState::IDLE:
			AnimSet(this->res->animIdle, this->animCnt / 10);
			break;

			//走り
		case AnimState::RUNNING:
			AnimSet(this->res->animRunning, this->animCnt / 10);
			break;
			//攻撃
		case AnimState::ATTACK:
			AnimSet(this->res->animAttack, this->animCnt / 10);
			break;
			//被ダメージ
		case AnimState::DAMAGED:
			AnimSet(this->res->animDamaged, this->animCnt / 10);
			break;

		}
			//地面につける
		MovePosToAngle(draw_, 0, 2);
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