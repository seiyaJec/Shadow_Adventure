#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Player
{
	
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤ");	//グループ名
	const  string  defName("仮");	//タスク名



	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		DG::Image::SP	imgLeft;
		DG::Image::SP   imgRight;
		DG::Image::SP	imgShadowLeft;
		DG::Image::SP	imgShadowRight;
		Anim			animIdle;
		Anim			animRunning;
		Anim			animJumping;
		Anim			animDoubleJump;
		Anim			animOnWall;
		Anim			animDamaged;
		Anim			animDeath;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//BCharaに持たせていない変数・メソッドのみここに書く
		XI::GamePad::SP  controller;
		Angle_LR wallJumpAngle;
		enum class AnimState { IDLE, RUNNING, JUMPING, DOUBLEJUMP, ONWALL, DAMAGED ,DEATH};
		AnimState animState;
		const float		wallJumpSpeed = 4;		//壁キックの横移動速度
		const float		fallSpeedMaxOnWall = 3;	//壁際の落下速度最大
		const int		wallJumpTime = 25;		//壁キックが固定される時間(フレーム)
		const int		inputJumpTimeMax = 5;		//ジャンプの予測入力を受け付ける時間（フレーム）
		int				attackTime;				//攻撃の総フレーム数
		int				attackCnt;				//攻撃用カウンタ
		bool			attacked;				//攻撃用長押し対策
		float			walkSpeed;				//歩く速さ
		float			dashRate;				//ダッシュで早くなる倍率
		bool			firstJump;				//一回目のジャンプ中ならtrue（ジャンプ長押し中もtrue）
		int				firstJumpTime;			//一度目のジャンプの長さ
		int				inputJump;				//ジャンプの予測入力
		bool			doubleJump;				//二段ジャンプ済みか
		float			doubleJumpPow;			//二段ジャンプの高さ
		int				cameraShakeTime;		//画面を揺らす時間
		int				cameraShakeInterval;	//画面を揺らす間隔
		ML::Point		cameraShakePosMax;		//画面を最大どれくらい揺らすか
		bool			isWallJump;				//壁キック状態か
		float			wallJumpPow;			//壁キックの高さ
		bool			hitFlagFront;			//正面が壁に接触しているか
		bool			hitFlagHead;			//頭が地形に接触しているか
		bool			attackFlag;				//攻撃中か
		int				damagedCount;			//被ダメージ時の無敵時間
		bool			damagedStopMove;		//被ダメージ時の入力拒否
		int				deathCount;				//死亡時の時間計測
		int				deathCountMax;			//死亡時の時間最大
		int				deathDisappearSpeed;	//死亡時消滅するスピード
		bool			retryAppear;			//リトライ時の登場処理実行フラグ
		ML::Vec2		retryPos;				//リトライ時登場予定の場所
		int				retryAppearSpeed;		//リトライ登場時の速さ
		int				ChkWSpikeDecrease;		//壁降り中、針検知の当たり判定の縦の減少値


		void Anim(ML::Box2D& draw_);
		void StartCameraShake(int time_, int interval_ , int xMax_, int yMax_);				//画面を揺らす
		void MoveLR(XI::VGamePad inp_, ML::Vec2& est_);
		void Jump();
		bool IsCanDoubleJump(XI::VGamePad inp_);
		void DoubleJump();
		void WallAction(XI::VGamePad inp_);
		void MoveWallJump(ML::Vec2& est_);
		void WallJumpStart(Angle_LR ang_);		//壁ジャンプの開始
		void WallFaling();						//壁際の落下制限
		void IsOnGround();
		void IsOnCeiling();						//天井の処理
		void Attack(XI::VGamePad inp_);
		void DamageToMe(Angle_LR ang_, int damage_);//プレイヤーにダメージを与える
		void DamagedMove(ML::Vec2& est_);
		void AttackInitialize(BChara::SP atk_);
		bool CheckWallSpike();					//壁にあるとげの当たり判定を行う
		bool CheckSpike();						//マップ当たり判定の直後に行う
		void DeathUpDate();						//死亡時の処理
		void SetRetryPos(const ML::Vec2& pos_);	//リトライ時の復活位置を設定
	};
}