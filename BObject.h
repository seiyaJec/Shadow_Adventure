#pragma once
#pragma warning(disable:4996)
#pragma once
#include "GameEngine_Ver3_83.h"
#include "MyStructure.h"
//--------------------------------------------------------------------------------------------
// 省データオブジェクトクラス
//--------------------------------------------------------------------------------------------

class BObject :public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BObject>		SP;
	typedef weak_ptr<BObject>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	ML::Box2D hitBase;			//当たり判定矩形
	ML::Box2D drawBase;			//描画用矩形
	ML::Box2D srcBase;			//元画像選択の矩形
	ML::Vec2 pos;				//位置情報
	int	imgSizeW;				//画像横幅
	int imgSizeH;				//画像縦幅
	int addSize;				//数値分だけスケールアップ
	int addSizeMax;				//スケールアップ最大
	bool puted;					//設置しているか
	bool moveMode;				//移動モード
	bool addedToMoveCount;		//カーソルに移動モード中を伝えたか
	bool created;				//プレイモード変換時にキャラを生成したか


	BObject() :
		hitBase(0, 0, 0, 0),
		drawBase(0, 0, 0, 0),
		srcBase(0, 0, 0, 0),
		pos(0, 0),
		imgSizeW(0),
		imgSizeH(0),
		addSize(0),
		addSizeMax(6),
		puted(false),
		moveMode(false),
		addedToMoveCount(false),
		created(false)
	{}

	void SetImgSize(int w_, int h_);										//画像設定
	void ApplySize(ML::Box2D& box_);										//スケールアップ
	void ApplySizeMove(ML::Box2D& box_);									//スケールを適用
	void InCursorSizeUp();													//カーソル照準時にスケールアップ
	void Draw(DG::Image::SP	img_);											//描画簡略化のための関数
	void AnimSet(Anim anim_, int cnt_, ML::Box2D& src_);					//アニメーション実行用関数
	void DataEnemyUpDate();													//敵データオブジェクトの更新処理
	void DataEnemyDraw(DG::Image::SP cantPutImg_,DG::Image::SP img_);		//敵データオブジェクトの描画処理
	bool CheckHitToDataObject(const ML::Box2D& me_);						//データオブジェクト同士の当たり判定
	void ApplyToMoveCount();												//カーソルに照準されていることを伝える関数
};
