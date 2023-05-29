//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_TitlePlayer.h"

namespace  TitlePlayer
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgLeft = DG::Image::Create("./data/image/Player/PlayerL.png");
		this->imgRight = DG::Image::Create("./data/image/Player/PlayerR.png");
		this->imgShadowLeft = DG::Image::Create("./data/image/Player/PlayerShadowL.png");
		this->imgShadowRight = DG::Image::Create("./data/image/Player/PlayerShadowR.png");
		LoadAnim(this->animIdle, "./data/animation/player/Idle.txt");
		LoadAnim(this->animRunning, "./data/animation/player/Running.txt");
		LoadAnim(this->animJumping, "./data/animation/player/Jumping.txt");
		LoadAnim(this->animDoubleJump, "./data/animation/player/DoubleJump.txt");
		LoadAnim(this->animOnWall, "./data/animation/player/OnWall.txt");
		LoadAnim(this->animDamaged, "./data/animation/player/Damaged.txt");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgRight.reset();
		this->imgLeft.reset();
		this->imgShadowLeft.reset();
		this->imgShadowRight.reset();
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
		//コントローラー
		this->controller = ge->in1;
		//画像優先度
		this->render2D_Priority[1] = 0.5f;
		//矩形
		this->hitBase = ML::Box2D(-15, -21, 30, 58);
		this->drawBase = ML::Box2D(-50, -37, 100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		//アニメーション
		this->animState = AnimState::IDLE;
		//ステータス
		this->statHPMax = 50;
		this->statHP = this->statHPMax;
		//攻撃関係
		this->attackTime = 0;
		this->attackCnt = 0;
		//カメラ関係
		this->cameraShakeTime = 0;
		this->cameraShakePosMax.x = 0;
		this->cameraShakePosMax.y = 0;
		//移動
		this->walkSpeed = 1;
		this->dashRate = 1.8f;
		this->firstJump = false;
		this->firstJumpTime = 10;
		this->inputJump = 99999;
		this->jumpPow = -8;
		this->wallJumpPow = -6;
		this->doubleJumpPow = -7;
		this->fallSpeedMax = 10;
		//判定
		this->hitFlagFront = false;
		this->hitFlagHead = false;
		//被ダメージ関係
		this->damagedStopMove = false;
		this->damagedCount = 0;
		this->damagedCntMax = 100;	//無敵時間
		this->damagedVecMax = ML::Vec2(2, -3);


		this->ctrlAng = Angle_LR::Right;
		this->moveLRFlag = false;
		this->dashFlag = false;
		this->attackFlag_T = false;
		this->pushJumpCnt = 0;
		this->attackNum = 0;
		this->titleMoveCnt = 0;
		this->imgSwap = false;
		this->titleAnimation = 0;
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
		//コントローラー情報の取得
		auto inp = this->controller->GetState();

		//移動データ初期化
		ML::Vec2  est(0, 0);
		this->animState = AnimState::IDLE;

		//ダメージを受けていないか
		if (false == this->damagedStopMove) {

			//タイトル専用移動指定
			this->TitleMove();

			//横移動
			if (this->moveLRFlag == true) {
				MoveLR(this->ctrlAng, est);
				this->moveLRFlag = false;
			}




			//壁キック中の処理
			this->MoveWallJump(est);

			//落下速度の制限
			if (this->fallSpeed > this->fallSpeedMax) {
				this->fallSpeed = this->fallSpeedMax;
			}
			//落下処理
			est.y += this->fallSpeed;

			//めり込まない移動処理
			this->CheckMove(est);

			//着地判定(めり込んでないか確認した後に行う）
			this->hitFlagFoot = this->CheckFoot();
			//壁接触判定
			this->hitFlagFront = this->CheckFront();
			//頭接触判定
			this->hitFlagHead = this->CheckHead();


			//ジャンプカウント処理
			if (this->inputJump < 99999) {
				++this->inputJump;
			}

			//ジャンプ
			this->Jump();
			//二段ジャンプ
			if (true == this->IsCanDoubleJump()) {
				this->DoubleJump();
			}
			//壁関係
			this->WallAction(this->ctrlAng);
			//着地処理
			this->IsOnGround();
			//天井処理
			this->IsOnCeiling();

			//攻撃
			if (true == this->attackFlag_T) {
				this->Attack(this->attackNum);
				this->attackFlag_T = false;
			}



		}
		//ダメージを受けている場合
		else {
			this->DamagedMove(est);
		}

		//被ダメージ中の処理
		if (true == this->damaged) {
			if (this->damagedCount > damagedCntMax) {
				this->damaged = false;
			}
			++this->damagedCount;
		}


		++this->titleMoveCnt;
		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto draw = this->drawBase;
		this->Anim(draw);
		auto src = this->srcBase;
		//座標位置へ変換
		draw.Offset(this->pos);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//色設定
		ML::Color color(1, 1, 1, 1);
		//ダメージ時に点滅
		if (true == this->damaged) {
			if (this->damagedCount % 10 == 0) {
				color = ML::Color(0.2f, 1, 1, 1);
			}
		}
		//向きによって使う画像を変更
		if (false == this->imgSwap) {
			if (this->angle_LR == Angle_LR::Left) {
				this->res->imgLeft->Draw(draw, src, color);
			}
			else {
				this->res->imgRight->Draw(draw, src, color);
			}
		}
		else {
			if (this->angle_LR == Angle_LR::Left) {
				this->res->imgShadowLeft->Draw(draw, src, color);
			}
			else {
				this->res->imgShadowRight->Draw(draw, src, color);
			}
		}

		//デバッグ用矩形表示
		this->DebugDrawBox(this->hitBase);
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	void Object::TitleMove()
	{
		switch (this->titleAnimation) {
		case 0:
			this->ctrlAng = Angle_LR::Right;
			this->CheckAnimFinish(0);
			break;
		case 1:
			this->moveLRFlag = true;
			this->CheckAnimFinish(400);
			break;
		case 2:
			this->CheckAnimFinish(50);
			break;
		case 3:
			ge->qa_Title->qa_Shadow->Kill();
			this->moveLRFlag = true;
			this->attackFlag_T = true;
			this->attackNum = 1;
			this->CheckAnimFinish(0);
			break;
		case 4:
			this->CheckAnimFinish(150);
			break;
		case 5:
			this->walkSpeed = 2;
			this->moveLRFlag = true;
			this->CheckAnimFinish(0);
			break;
		case 6:
			this->moveLRFlag = true;
			this->CheckAnimFinish(170);
			break;
		case 7:
			this->moveLRFlag = true;
			this->inputJump = 0;
			this->CheckAnimFinish(0);
			break;
		case 8:
			this->moveLRFlag = true;
			this->CheckAnimFinish(60);
			break;
		case 9:
			this->walkSpeed = 3;
			this->moveLRFlag = true;
			this->inputJump = 0;
			this->CheckAnimFinish(0);
			break;
		case 10:
			this->moveLRFlag = true;
			this->CheckAnimFinish(20);
			break;
		case 11:
			this->moveLRFlag = true;
			if (this->titleMoveCnt % 30 == 1) {
				this->inputJump = 0;
			}
			else if (this->titleMoveCnt % 30 == 2) {
				this->Angle_LR_Reverse(this->ctrlAng);
			}
			this->CheckAnimFinish(120);
			break;
		case 12:
			this->moveLRFlag = true;
			this->CheckAnimFinish(10);

		}
		//}
		//else if (this->titleMoveCnt < 900) {
		//	dashFlag = true;
		//	this->moveLRFlag = true;

		//	if (this->titleMoveCnt == 800) {
		//		this->inputJump = 0;
		//	}
		//	if (this->titleMoveCnt == 899) {
		//		this->inputJump = 0;
		//		this->walkSpeed = 3;
		//	}
		//}
		//else if (this->titleMoveCnt < 3000) {
		//	this->moveLRFlag = true;
		//	if (this->titleMoveCnt >= 950) {
		//		if (this->titleMoveCnt % 50 == 0) {
		//			inputJump = 0;
		//			this->Angle_LR_Reverse(this->ctrlAng);
		//		}
		//	}
		//}
		
	}
	 
	//アニメーションが終わったか確認
	void Object::CheckAnimFinish(int limit_)
	{
		if (this->titleMoveCnt >= limit_) {
			++this->titleAnimation;
			this->titleMoveCnt = 0;
		}
	}

	//アニメーション
	void Object::Anim(ML::Box2D& draw_)
	{
		int animNum = 0;
		switch (this->animState) {

			//静止
		case AnimState::IDLE:
			AnimSet(this->res->animIdle, this->animCnt / 10);
			break;

			//走り
		case AnimState::RUNNING:
			AnimSet(this->res->animRunning, this->animCnt / 10);
			MovePosToAngle(draw_, -7, 0);
			break;

			//ジャンプ
		case AnimState::JUMPING:
			if (this->fallSpeed <= -4) {
				animNum = 0;
			}
			else if (this->fallSpeed <= 0) {
				animNum = 1;
			}
			else if (this->fallSpeed <= 5) {
				animNum = 2;
			}
			else {
				animNum = 3;
			}
			AnimSet(this->res->animJumping, animNum);
			if (animNum == 3) {
				MovePosToAngle(draw_, 0, 6);
			}
			break;
		case AnimState::DOUBLEJUMP:
			AnimSet(this->res->animDoubleJump, this->animCnt / 10);
			break;
			//壁につく
		case AnimState::ONWALL:
			AnimSet(this->res->animOnWall, this->animCnt / 10);
			break;
			//ダメージを受ける
		case AnimState::DAMAGED:
			AnimSet(this->res->animDamaged, this->animCnt / 10);
		default:
			break;
		}
	}
	//画面を揺らす
	void Object::StartCameraShake(int time_, int interval_, int xMax_, int yMax_)
	{
		this->cameraShakeTime = time_;
		this->cameraShakeInterval = interval_;
		ge->camera2DMovePos.x = xMax_;
		ge->camera2DMovePos.y = yMax_;
	}
	//横移動
	void Object::MoveLR(Angle_LR ang_, ML::Vec2& est_)
	{
		if (ang_ == Angle_LR::Left) {
			if (false == this->dashFlag) {
				est_.x -= this->walkSpeed;
			}
			else {
				est_.x -= this->walkSpeed * this->dashRate;
				this->dashFlag = false;
			}
			this->angle_LR = Angle_LR::Left;
			this->animState = AnimState::RUNNING;
		}
		//横移動（右）
		if (ang_ == Angle_LR::Right) {
			if (false == this->dashFlag) {
				est_.x += this->walkSpeed;
			}
			else {
				est_.x += this->walkSpeed * this->dashRate;
				this->dashFlag = false;
			}
			this->angle_LR = Angle_LR::Right;
			this->animState = AnimState::RUNNING;
		}
	}
	//壁キック中の移動
	void Object::MoveWallJump(ML::Vec2& est_)
	{
		if (true == this->isWallJump) {
			if (this->moveCnt > 0) {
				est_.x = this->moveVec.x;
				this->angle_LR = wallJumpAngle;
			}
			else {
				this->isWallJump = false;
			}
			--this->moveCnt;
		}
	}
	//ジャンプ判定処理
	void Object::Jump()
	{
		if (this->inputJump < inputJumpTimeMax) {
			if (true == this->hitFlagFoot) {
				this->fallSpeed = this->jumpPow;
				this->firstJump = true;
				this->inputJump = 0;
				this->attackFlag = false;
			}
		}
		else if (firstJump == true) {
			if (this->inputJump < firstJumpTime) {
				this->fallSpeed = this->jumpPow;
			}
			else {
				this->firstJump = false;
				this->fallSpeed = -4;
			}
		}


	}

	//二段ジャンプ可能か
	bool Object::IsCanDoubleJump()
	{
		if (this->inputJump >= inputJumpTimeMax) {
			return false;
		}
		if (true == this->firstJump) {
			return false;
		}
		if (true == this->doubleJump) {
			return false;
		}
		if (true == this->hitFlagFoot) {
			return false;
		}
		if (true == this->isWallJump) {
			return false;
		}
		return true;
	}
	//二段ジャンプ判定処理
	void Object::DoubleJump()
	{
		this->fallSpeed = this->doubleJumpPow;
		this->doubleJump = true;
		this->attackFlag = false;
	}
	//壁際の処理
	void Object::WallAction(Angle_LR ang_)
	{
		if (true == this->hitFlagFront) {//壁際にいるか
			if (false == this->hitFlagFoot) {//空中にいるか
				if (false == this->attackFlag) {//攻撃中じゃないか
					if (false == firstJump) {
						//左向き
						if (ang_ == Angle_LR::Left) {
							//落下処理
							this->WallFaling();
							//ジャンプボタンで壁ジャンプ
							if (this->inputJump <= inputJumpTimeMax) {
								this->WallJumpStart(Angle_LR::Right);		//飛ぶ方向を指定
							}
						}
						//右向き
						if (ang_ == Angle_LR::Right) {
							//落下処理
							this->WallFaling();
							//ジャンプボタンで壁ジャンプ
							if (this->inputJump <= inputJumpTimeMax) {
								this->WallJumpStart(Angle_LR::Left);	//飛ぶ方向を指定
							}
						}
					}
				}
			}
		}
	}
	//壁での落下制限
	void Object::WallFaling()
	{
		//落下中の処理
		if (this->fallSpeed > this->fallSpeedMaxOnWall) {
			//落下速度を制限する
			this->fallSpeed = this->fallSpeedMaxOnWall;
			//アニメーションを変更
			this->animState = AnimState::ONWALL;
		}
	}
	//壁ジャンプ
	void Object::WallJumpStart(Angle_LR ang_)
	{
		//向きを設定
		this->wallJumpAngle = ang_;

		//ジャンプ中の横移動の設定
		if (wallJumpAngle == Angle_LR::Right) {
			this->moveVec.x = this->walkSpeed * this->dashRate;
		}
		else {
			this->moveVec.x = -this->walkSpeed * this->dashRate;
		}
		//壁ジャンプ処理開始の合図
		this->isWallJump = true;

		//一定時間操作を固定
		this->moveCnt = this->wallJumpTime;

		//ジャンプの高さを設定
		this->fallSpeed = this->wallJumpPow;

	}
	//地面にいるか
	void Object::IsOnGround()
	{
		if (true == this->hitFlagFoot) {
			if (this->fallSpeed > 0) {
				if (this->fallSpeed > fallSpeedMax / 2) {
					auto smoke = Effect01::Object::Create(true);
					smoke->pos += this->pos;
				}
				this->fallSpeed = 0; //落下速度0
				this->doubleJump = false;
			}
		}
		else {
			this->fallSpeed += ML::Gravity(32) * 3;
			if (this->animState != AnimState::ONWALL) {
				this->animState = AnimState::JUMPING;
			}
		}
	}
	//天井に触れているか
	void Object::IsOnCeiling()
	{
		if (true == this->hitFlagHead) {
			if (this->fallSpeed <= -1) {
				this->fallSpeed *= -0.5f;
			}
			this->firstJump = false;
		}
	}
	//攻撃
	void Object::Attack(int atkNum_)
	{
		//アニメーションの変更
		switch (atkNum_) {
			//地面にいるとき
		case 0:
			{
				auto atk = Attack00::Object::Create(true);
				this->AttackInitialize(atk);
				break;
			}
			break;
			//走っているとき
		case 1:
			{
				auto atk = Attack01::Object::Create(true);
				this->AttackInitialize(atk);
				break;
			}
			break;
			//空中にいるとき
		case 2:
			{
				auto atk = Attack02::Object::Create(true);
				this->AttackInitialize(atk);
				atk->fallSpeedMax = this->fallSpeedMax;
				break;
			}
			break;
		}
	}
	//攻撃
	void Object::AttackInitialize(BChara::SP atk_)
	{
		atk_->angle_LR = this->angle_LR;
		atk_->pos = this->pos;
	}

	//被ダメージ処理
	void Object::DamagedMove(ML::Vec2& est_)
	{
		this->animState = AnimState::DAMAGED;
		//落下速度の制限
		if (this->moveVec.y > this->fallSpeedMax) {
			this->moveVec.y = this->fallSpeedMax;
		}
		//移動
		this->moveVec.x -= 0.1f * Angle_LR_ChangeAngleToInt(this->damagedAngle_LR);
		//落下処理
		this->moveVec.y += ge->gravity;
		est_ = this->moveVec;

		this->CheckMove(est_);

		//移動がなくなったら終了処理
		if (true == this->IsZeroDamagedMove()) {
			this->damagedStopMove = false;
			this->fallSpeed = this->moveVec.y;
			return;
		}
	}

	//-------------------------------------------------------------------
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