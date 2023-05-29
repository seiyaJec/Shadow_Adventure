#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//チュートリアル用見本プレイヤ
//-------------------------------------------------------------------
#include "BChara.h"
#include "MyStructure.h"

namespace  TutorialPlayer
{

	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("プレイヤ");	//グループ名
	const  string  defName("チュートリアル");	//タスク名

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
		Angle_LR wallJumpAngle;
		enum class AnimState { IDLE, RUNNING, JUMPING, DOUBLEJUMP, ONWALL, DAMAGED };
		AnimState animState;
		const float		wallJumpSpeed = 4;		//壁キックの横移動速度
		const float		fallSpeedMaxOnWall = 3;	//壁際の落下速度最大
		const int		wallJumpTime = 25;		//壁キックが固定される時間(フレーム)
		const int		inputJumpTimeMax = 5;		//ジャンプの予測入力を受け付ける時間（フレーム）
		int				attackTime;				//攻撃の総フレーム数
		int				attackCnt;				//攻撃用カウンタ
		float			walkSpeed;				//歩く速さ
		float			dashRate;				//ダッシュで早くなる倍率
		bool			firstJump;				//ジャンプ済みか
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

		Angle_LR		ctrlAng;				//タイトル専用、プレイヤーを動かす方向
		bool			moveLRFlag;				//移動フラグ
		bool			dashFlag;				//ダッシュフラグ
		bool			attackFlag_T;			//攻撃フラグ
		int				attackNum;				//どの攻撃を出すか
		int				pushJumpCnt;			//ジャンプボタン（1以上で押されている判定）
		int				tutorialMoveCnt;
		int				tutorialAnimation;		//プレイヤーの動き
		bool			imgSwap;				//絵をシャドウに変更


		KeyInput playerAction;				//プレイヤーへの入力
		bool			jumpFlag;			//ジャンプが入力されたか
		ML::Color		color;				//半透明にさせるため
		bool			preAttack;			//1フレーム前に攻撃をしたか



		void Anim(ML::Box2D& drawBox_);
		void StartCameraShake(int time_, int interval_, int xMax_, int yMax_);				//画面を揺らす
		void MoveLR(Angle_LR ang_, ML::Vec2& est_);
		void Jump();
		bool IsCanDoubleJump();
		void DoubleJump();
		void WallAction(Angle_LR ang_);
		void MoveWallJump(ML::Vec2& est_);
		void WallJumpStart(Angle_LR ang_);		//壁ジャンプの開始
		void WallFaling();						//壁際の落下制限
		void IsOnGround();
		void IsOnCeiling();						//
		void Attack();
		void DamagedMove(ML::Vec2& est_);
		void AttackInitialize(BChara::SP atk_);
	};
}