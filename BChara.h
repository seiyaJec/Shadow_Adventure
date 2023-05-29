#pragma once
#pragma warning(disable:4996)
#pragma once
#include "GameEngine_Ver3_83.h"
#include "MyStructure.h"

//--------------------------------------------------------------------------------------------
// キャラクタ汎用スーパークラス
//--------------------------------------------------------------------------------------------

class BChara :public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//定数として用意する変数
	//キャラクタ共通メンバ変数
		//左右の向き（2D横視点ゲーム専用）
	enum class Angle_LR { Left, Right };
	Angle_LR	angle_LR;
	ML::Vec2	pos;					//キャラクタ位置
	ML::Box2D	hitBase;				//当たり判定範囲
	ML::Box2D   attackBase;				//攻撃用の当たり判定範囲
	ML::Box2D	drawBase;				//描画時矩形のスケール
	ML::Box2D	srcBase;				//描画元範囲
	ML::Box2D	playerSerchBox;			//プレイヤー探知範囲
	ML::Vec2	moveVec;				//移動ベクトル
	ML::Vec2	damagedVecMax;			//被ダメージ時の移動速度最大
	int			moveCnt;				//行動カウンタ
	int			moveTime;				//移動時間設定
	int			statHP;					//体力
	int			statHPMax;				//体力の上限
	int			statAttack;				//攻撃力
	int			animCnt;				//アニメーションカウンタ
	float		jumpPow;				//ジャンプ時の初速
	float		fallSpeed;				//落下速度
	float		fallSpeedMax;			//落下速度最大
	bool		hitFlagFoot;			//着地しているか
	bool		damaged;				//攻撃を受けたか
	bool		touchToSpike;			//針にあたっているか
	int			damagedCntMax;				//被ダメージ時のカウンタ
	float		damagedSlowdownX;		//Xの減速（Yは重力を使用）
	Angle_LR	damagedAngle_LR;		//吹っ飛ばされる方向
	bool		debugFlag;				//個別にデバッグモード有効にしたいときに利用				
	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BChara()
		:pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, attackBase(0, 0, 0, 0)
		, drawBase(0, 0, 0, 0)
		, srcBase(0, 0, 0, 0)
		, playerSerchBox(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, statHPMax(0)
		, statHP(0)
		, statAttack(0)
		, animCnt(0)
		, jumpPow(0)
		, fallSpeed(0)
		, fallSpeedMax(10)
		, moveTime(0)
		, hitFlagFoot(false)
		, damaged(false)
		, damagedCntMax(0)
		, damagedSlowdownX(0.1f)
		, damagedAngle_LR(Angle_LR::Right)
		, debugFlag(false)
		, angle_LR(Angle_LR::Right)
	{
	}
	virtual ~BChara() {}

	//キャラクタ共通メソッド
	//めり込まない移動処理
	void CheckMove(ML::Vec2& est_);
	//アングルを数値として返す
	int Angle_LR_ChangeAngleToInt(Angle_LR ang_);
	//int型をアングルとして返す
	void Angle_LR_ChangeIntToAngle(Angle_LR& ang_, int aInt_);
	//アングルを反転
	void Angle_LR_Reverse(Angle_LR& ang_);
	//敵キャラとの当たり判定
	float CheckDamageToEnemy(ML::Box2D hitBox_);
	//プレイヤーとの当たり判定
	bool  CheckDamageToPlayer();
	//足元接触判定
	bool CheckFoot();
	//頭接触判定
	bool CheckHead();
	//壁接触判定
	bool CheckFront();
	//向き情報を踏まえて位置変換
	void MovePosToAngle(ML::Box2D& box_, int x_, int y_);
	//被ダメージ時の移動がなくなったか
	bool IsZeroDamagedMove();
	//プレイヤー探知の処理
	bool SerchPlayer();
	//デバッグモード：当たり判定矩形表示
	void DebugDrawBox(ML::Box2D const box_);
	//アニメーション画像選択
	void AnimSet(Anim anim_, int cnt_);
};

