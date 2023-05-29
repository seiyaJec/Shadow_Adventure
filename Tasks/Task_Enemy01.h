#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//敵01：鳥
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Enemy01
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("敵");	//グループ名
	const  string  defName("01");		//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//共有する変数はここに追加する
		DG::Image::SP imgLeft;
		DG::Image::SP imgRight;
		Anim		  animIdle;
		Anim		  animRunning;
		Anim		  animAttack;
		Anim		  animDamaged;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
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
		void  UpDate()		override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()	override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//追加したい変数・メソッドはここに追加する
		//BCharaに含まれないモノのみここに追加する
		enum class AnimState { IDLE, RUNNING, ATTACK, DAMAGED };
		AnimState	animState;
		float		defaultHeight;	//元の高さ
		float		attackSpeedRate;//プレイヤーを攻撃する速さ
		float		attackAngle;	//攻撃の向き
		float		attackCnt;		//攻撃移動の時間
		int			stayTimeToAttack;	//プレイヤーを発見後攻撃するまでの待機時間
		ML::Vec2	playerPos;		//プレイヤーの座標記憶
		ML::Vec2	targetVec;		//プレイヤーに向かうベクトル
		ML::Vec2	moveAttack;		//攻撃の移動速度

		void Anim(ML::Box2D& draw_);
		void ChangeAnim(AnimState anim_);	//アニメーション（モーション）変更処理

	};
}