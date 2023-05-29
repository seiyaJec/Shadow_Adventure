//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Effect.h"
#include  "Task_Attack.h"
#include  "Task_Player.h"
#include  "Task_GameUIGoalDisplay.h"
#include  "Task_GameOverShadow.h"

namespace  Player
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
		LoadAnim(this->animDeath, "./data/animation/player/Death.txt");
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
		//参照用
		ge->qa_Player = ge->GetTask<Player::Object>("プレイヤ", "仮");
		//コントローラー
		this->controller = ge->in1;
		//画像優先度
		this->render2D_Priority[1] = 0.5f;
		//矩形
		this->hitBase = ML::Box2D(-15, -21, 30, 58);
		this->drawBase = ML::Box2D(-50, -37, 100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		this->ChkWSpikeDecrease = 22;
		//アニメーション
		this->animState = AnimState::IDLE;
		//ステータス
		this->statHPMax = 50;
		this->statHP = this->statHPMax;
		//攻撃関係
		this->attackTime = 0;
		this->attackCnt = 0;
		this->attacked = false;
		//カメラ関係
		this->cameraShakeTime = 0;
		this->cameraShakePosMax.x = 0;
		this->cameraShakePosMax.y = 0;
		//移動
		this->walkSpeed = 3;
		this->dashRate = 1.8f;
		this->firstJump = false;
		this->firstJumpTime = 10;
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
		//死亡関係
		this->deathCount = 0;
		this->deathCountMax = 60;
		this->deathDisappearSpeed = 30;
		//リトライ
		this->retryAppear = false;
		this->retryPos = ML::Vec2(0, 0);
		this->retryAppearSpeed = 10;
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

		//ゴールしたなら処理しない
		if (true == ge->goalFlag) {
			++this->animCnt;
			return;
		}

		//ゲームオーバーの時も移動処理を消す
		if (true == ge->gameOverflag) {
			this->DeathUpDate();
			return;
		}

		//リトライが実行されたら
		if (true == this->retryAppear) {
			this->animState = AnimState::JUMPING;
			this->fallSpeed = 6;
			this->pos.y += this->retryAppearSpeed;
			if (this->pos.y >= this->retryPos.y) {
				this->pos.y = this->retryPos.y;
				this->retryAppear = false;
			}
			return;
		}

		//ダメージを受けていないか
		if (false == this->damagedStopMove) {
			//移動処理
			this->MoveLR(inp, est);
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

			//ゴール処理
			ge->qa_Map->CheckHit(this->hitBase.OffsetCopy(this->pos));
			if (true == ge->qa_Map->foundGoal) {
				GameUIGoalDisplay::Object::Create(true);
				ge->goalFlag = true;
				return;
			}
			//落下死亡処理
			ML::Box2D pHit = this->hitBase.OffsetCopy(this->pos);
			if (pHit.y > ge->qa_Map->hitBase.y + ge->qa_Map->hitBase.h) {
				this->statHP = 0;
				ge->gameOverflag = true;
			}

			//着地判定(めり込んでないか確認した後に行う）
			this->hitFlagFoot = this->CheckFoot();
			CheckSpike();

			//ジャンプボタンが押されたら予測入力として受け付ける
			if (inp.B1.down) {
				this->inputJump = 0;
			}
			else if (inp.B1.on) {
				++this->inputJump;

			}
			else {
				this->inputJump = 99999;
			}

			//ジャンプ
			this->Jump();
			//二段ジャンプ
			if (true == this->IsCanDoubleJump(inp)) {
				this->DoubleJump();
			}
			//壁関係
			this->hitFlagFront = this->CheckFront();	//ここは壁処理中に針判定する
			WallAction(inp);
			//着地処理
			this->IsOnGround();
			//天井処理
			this->hitFlagHead = this->CheckHead();
			CheckSpike();
			this->IsOnCeiling();
			//攻撃開始
			this->Attack(inp);
		}
		//ダメージを受けている場合
		else {
			this->DamagedMove(est);
		}

		//カメラの位置を再調整
		{
			//プレイヤを画面のどこに置くか（今回は画面中央）
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h / 2;
			//プレイヤを画面中央に置いた時のカメラの左上座標を求める
			int cpx = int(this->pos.x) - px;
			int cpy = int(this->pos.y) - py;
			//カメラの座標を更新
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
			//マップの外側が映らないようにカメラを調整する
			ge->qa_Map->AdjustCameraPos();

		}
		//移動の指示があれば実行する
		if (this->cameraShakeTime > 0) {
			if (this->cameraShakeTime % this->cameraShakeInterval == 0) {
				ge->camera2DMovePos.x = -ge->camera2DMovePos.x;
				ge->camera2DMovePos.y = -ge->camera2DMovePos.y;
			}
			ge->camera2D.x += ge->camera2DMovePos.x;
			ge->camera2D.y += ge->camera2DMovePos.y;

			--this->cameraShakeTime;
		}

		//被ダメージ中の処理
		if (true == this->damaged) {
			if (this->damagedCount > damagedCntMax) {
				this->damaged = false;
			}
			++this->damagedCount;
		}



		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		this->Anim(draw);
		ML::Box2D src = this->srcBase;
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//チュートリアル時は描画を変更
		DrawSetIfTuto(draw,src);
		//色設定
		ML::Color color(1, 1, 1, 1);
		//ダメージ時に点滅
		if (true == this->damaged) {
			if (this->damagedCount % 10 == 0) {
				color = ML::Color(0.2f, 1, 1, 1);
			}
		}
		//向きによって使う画像を変更
		if (this->angle_LR == Angle_LR::Left) {
			this->res->imgLeft->Draw(draw, src, color);
		}
		else {
			this->res->imgRight->Draw(draw, src, color);
		}
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//アニメーション
	void Object::Anim(ML::Box2D& draw_)
	{
		int animNum = 0;
		switch (this->animState) {

			//静止
		case AnimState::IDLE:
			AnimSet(this->res->animIdle, this->animCnt / 10);
			draw_.y += 3;
			break;

			//走り
		case AnimState::RUNNING:
			AnimSet(this->res->animRunning, this->animCnt / 10);
			MovePosToAngle(draw_, -7, 3);
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
			break;
		case AnimState::DEATH:
			AnimSet(this->res->animDeath, this->animCnt / 10);
			break;
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
	void Object::MoveLR(XI::VGamePad inp_, ML::Vec2& est_)
	{
		if (inp_.LStick.BL.on) {
			if (inp_.B3.on) {
				est_.x -= this->walkSpeed * this->dashRate;
			}
			else {
				est_.x -= this->walkSpeed;
			}
			this->angle_LR = Angle_LR::Left;
			this->animState = AnimState::RUNNING;
		}
		//横移動（右）
		if (inp_.LStick.BR.on) {
			if (inp_.B3.on) {
				est_.x += this->walkSpeed * this->dashRate;
			}
			else {
				est_.x += this->walkSpeed;
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
		//ジャンプボタン長押し中の処理
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

	bool Object::IsCanDoubleJump(XI::VGamePad inp_)
	{
		if (this->inputJump >= inputJumpTimeMax) {
			return false;
		}
		if (true == this->hitFlagFront) {
			if (true == inp_.LStick.BR.on) {
				return false;
			}
			if (true == inp_.LStick.BL.on) {
				return false;
			}
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
	void Object::WallAction(XI::VGamePad inp_)
	{
		if (true == this->hitFlagFront) {//壁際にいるか
			if (false == this->hitFlagFoot) {//空中にいるか
				if (false == this->attackFlag) {//攻撃中じゃないか
					if (false == firstJump) {//一回目のジャンプ処理中じゃないか
						if (false == this->isWallJump) {//壁ジャンプ中じゃないか
							//左向き
							if (inp_.LStick.BL.on) {
								//針があったなら処理しない
								if (true == this->CheckWallSpike()) {
									this->CheckSpike();
									return;
								}
								//落下処理
								this->WallFaling();
								//ジャンプボタンで壁ジャンプ
								if (this->inputJump <= inputJumpTimeMax) {
									this->WallJumpStart(Angle_LR::Right);		//飛ぶ方向を指定
								}
								return;
							}
							//右向き
							if (inp_.LStick.BR.on) {
								//針があったら処理しない
								if (true == this->CheckWallSpike()) {
									this->CheckSpike();
									return;
								}
								//落下処理
								this->WallFaling();
								//ジャンプボタンで壁ジャンプ
								if (this->inputJump <= inputJumpTimeMax) {
									this->WallJumpStart(Angle_LR::Left);	//飛ぶ方向を指定
								}
								return;
							}
						}
						else {
							if (this->inputJump <= inputJumpTimeMax) {
								auto ang = this->angle_LR;
								this->Angle_LR_Reverse(ang);
								this->WallJumpStart(ang);
								return;
							}
						}
					}
				}
			}
		}
		//壁に向かって歩いていたら針検知
		if (inp_.LStick.BL.on || inp_.LStick.BR.on) {
			this->CheckSpike();
		}
	}

	//壁のとげ判定
	bool Object::CheckWallSpike()
	{
		//当たり判定をもとにして正面矩形を生成
		ML::Box2D front(
			this->hitBase.x,
			this->hitBase.y + this->ChkWSpikeDecrease,
			1,
			this->hitBase.h - this->ChkWSpikeDecrease * 2
		);
		//キャラの向きによって判定する位置を変更
		if (this->angle_LR == Angle_LR::Right) {
			front.x += this->hitBase.w;
		}
		else {
			front.x += -1;
		}
		front.Offset(this->pos);

		if (nullptr == ge->qa_Map) { return false; }//マップがなければ判定しない（できない）
		//前面に針があるときは判定する
		if (true == ge->qa_Map->foundSpike) {
			//判定位置に壁があるか
			bool iswall = ge->qa_Map->CheckHit(front);
			return !iswall || ge->qa_Map->foundSpike;
		}
		return false;
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
	void Object::Attack(XI::VGamePad inp_)
	{
		if (false == this->attacked) {
			if (inp_.Trigger.volumeR != 0) {
				//アニメーションの変更
				switch (this->animState) {
					//地面にいるとき
				case AnimState::IDLE:
				{
					auto atk = Attack00::Object::Create(true);
					this->AttackInitialize(atk);
					atk->MovePosToAngle(atk->attackBase, 22, -5);
					break;
				}
				//走っているとき
				case AnimState::RUNNING:
				{
					auto atk = Attack01::Object::Create(true);
					this->AttackInitialize(atk);
					atk->MovePosToAngle(atk->attackBase, 2, 10);
					break;
				}
				//空中にいるとき
				case AnimState::JUMPING:
				case AnimState::ONWALL:
				{
					auto atk = Attack02::Object::Create(true);
					this->AttackInitialize(atk);
					atk->fallSpeedMax = this->fallSpeedMax;
					if (inp_.RStick.BR.on) {
						atk->angle_LR = Angle_LR::Right;
					}
					else  if (inp_.LStick.BL.on) {
						atk->angle_LR = Angle_LR::Left;
					}
					atk->moveVec.x = 4.0f * Angle_LR_ChangeAngleToInt(this->angle_LR);
					break;
				}
				}
				this->attacked = true;
			}
		}
		else if (inp_.Trigger.volumeR == 0) {
			this->attacked = false;
		}
	}
	void Object::AttackInitialize(BChara::SP atk_) 
	{
		atk_->angle_LR = this->angle_LR;
		atk_->pos = this->pos;
	}
	//----------------------------------------------------------------------------------
	//プレイヤーにダメージ
	void Object::DamageToMe(Angle_LR ang_, int damage_)
	{
		//プレイヤー
		this->statHP -= damage_;
		//HPが0ならゲームオーバー
		if (this->statHP <= 0) {
			ge->gameOverflag = true;
		}
		else {
			this->damaged = true;
			this->damagedStopMove = true;
			this->damagedCount = 0;
			//プレイヤーが敵の右側にいる場合
			this->damagedAngle_LR = ang_;
			//プレイヤー吹っ飛ばし処理
			this->moveVec.x = ge->qa_Player->damagedVecMax.x * Angle_LR_ChangeAngleToInt(ge->qa_Player->damagedAngle_LR);
			this->moveVec.y = ge->qa_Player->damagedVecMax.y;
			this->StartCameraShake(6, 2, 6, 6);
		}
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
	//針ダメージ判定（マップ当たり判定の直後でないと効果なし）
	bool Object::CheckSpike()
	{
		if (true == this->damaged) {
			return false;
		}
		if (true == ge->qa_Map->foundSpike) {
			Angle_LR reverseAngle = this->angle_LR;
			Angle_LR_Reverse(reverseAngle);
			this->DamageToMe(reverseAngle, 20);
			return true;
		}
		return false;
	}

	//死亡時の処理
	void Object::DeathUpDate()
	{
		this->animState = AnimState::DEATH;
		if (this->deathCount == 0) {
			this->animCnt = 0;
			GameOverShadow::Object::Create(true);
		}
		if (this->animCnt >= this->deathCountMax - 1) {
			//アニメーション最後の画像を継続させる
			this->animCnt = this->deathCountMax -1;	

			//死亡時消滅の処理
			this->pos.y -= deathDisappearSpeed;

			//プレイヤーが完全に消えたらキル
			if (this->pos.y <= ge->camera2D.y - this->drawBase.h) {
				ge->retryFlag = true;
			}
		}
		else {
			++this->animCnt;
		}
		++this->deathCount;
	}

	void Object::SetRetryPos(const ML::Vec2& pos_)
	{
		this->retryPos = pos_;
		this->pos = pos_;
		this->pos.y -= ge->screen2DHeight;
		//カメラの位置を再調整
		{
			//プレイヤを画面のどこに置くか（今回は画面中央）
			int px = ge->camera2D.w / 2;
			int py = ge->camera2D.h / 2;
			//プレイヤを画面中央に置いた時のカメラの左上座標を求める
			int cpx = int(pos_.x) - px;
			int cpy = int(pos_.y) - py;
			//カメラの座標を更新
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
			//マップの外側が映らないようにカメラを調整する
			ge->qa_Map->AdjustCameraPos();

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