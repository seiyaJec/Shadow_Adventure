//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_TutorialPlayer.h"

namespace  TutorialPlayer
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
		//画像優先度
		this->render2D_Priority[1] = 0.5f;
		//矩形
		this->hitBase = ML::Box2D(-15, -21, 30, 58);
		this->drawBase = ML::Box2D(-50, -37, 100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		//色
		this->color = ML::Color(0.5f, 1, 1, 1);
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
		this->walkSpeed = 3;
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

		//操作関係
		this->ctrlAng = Angle_LR::Right;
		this->moveLRFlag = false;
		this->dashFlag = false;
		this->attackFlag_T = false;
		this->pushJumpCnt = 0;
		this->attackNum = 0;
		this->tutorialMoveCnt = 0;
		this->imgSwap = false;
		this->tutorialAnimation = 0;
		this->jumpFlag = false;
		this->preAttack = false;

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

		//移動データ初期化
		ML::Vec2  est(0, 0);
		this->animState = AnimState::IDLE;


		//ダメージを受けていないか
		if (false == this->damagedStopMove) {


			//横移動
			if (this->moveLRFlag == true) {
				MoveLR(this->ctrlAng, est);
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


			//ジャンプボタンが押されたら予測入力として受け付ける
			if (true == this->jumpFlag && this->inputJump == 99999) {
				this->inputJump = 0;
			}
			else if (true == this->jumpFlag) {
				++this->inputJump;

			}
			else {
				this->inputJump = 99999;
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
			if (true == this->attackFlag) {
				if (false == this->preAttack) {
					this->Attack();
					this->preAttack = true;
				}
				this->attackFlag = false;
			}
			else {
				this->preAttack = false;
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


		++this->tutorialMoveCnt;
		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		//アニメーションに合わせて描画矩形を設定
		this->Anim(draw);
		ML::Box2D src = this->srcBase;
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//色
		ML::Color color = this->color;
		//チュートリアル時は描画を変更
		DrawSetIfTuto(draw, src);
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

	//アニメーション
	void Object::Anim(ML::Box2D& drawBox_)
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
			MovePosToAngle(drawBox_, -7, 0);
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
				MovePosToAngle(drawBox_, 0, 6);
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
	void Object::Attack()
	{
		//アニメーションの変更
		switch (this->animState) {
			//地面にいるとき
		case AnimState::IDLE:
		{
			auto atk = Attack00::Object::Create(true);
			atk->color = this->color;
			this->AttackInitialize(atk);
			break;
		}
		//走っているとき
		case AnimState::RUNNING:
		{
			auto atk = Attack01::Object::Create(true);
			atk->color = this->color;
			this->AttackInitialize(atk);
			break;
		}
		//空中にいるとき
		case AnimState::JUMPING:
		case AnimState::ONWALL:
		{
			auto atk = Attack02::Object::Create(true);
			atk->color = this->color;
			this->AttackInitialize(atk);
			atk->fallSpeedMax = this->fallSpeedMax;
			atk->moveVec.x = 4.0f * Angle_LR_ChangeAngleToInt(this->angle_LR);
			break;
		}
		}
	}

	//攻撃
	void Object::AttackInitialize(BChara::SP atk_)
	{
		atk_->angle_LR = this->angle_LR;
		atk_->pos = this->pos;
		//当たり判定をなくす
		atk_->hitBase = ML::Box2D(0, 0, 0, 0);
		atk_->attackBase = ML::Box2D(0, 0, 0, 0);
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