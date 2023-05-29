//----------------------------------------------------------------------------------
// マップ作成用オブジェクトクラス
//----------------------------------------------------------------------------------
#include "BObject.h"
#include "MyPG.h"

//画像サイズの設定
void BObject::SetImgSize(int w_, int h_)
{
	imgSizeW = w_;
	imgSizeH = h_;
}

//サイズ追加の適用
void BObject::ApplySize(ML::Box2D& box_)
{
	float sizeRate = 1 + (this->addSize * 0.1f);
	box_.x -= (int)(box_.w * sizeRate) - box_.w;
	box_.y -= (int)(box_.h * sizeRate) - box_.h;
	box_.w = (int)(box_.w * sizeRate);
	box_.h = (int)(box_.h * sizeRate);
}

//サイズ追加の適用
void BObject::ApplySizeMove(ML::Box2D& box_)
{
	float sizeRate = 1 + (this->addSize * 0.1f);
	box_.x -= (int)(box_.w / 2 * sizeRate) - box_.w / 2;
	box_.y -= (int)(box_.h / 2 * sizeRate) - box_.h / 2;
	box_.w = (int)(box_.w * sizeRate);
	box_.h = (int)(box_.h * sizeRate);
}

//画像の描画
void BObject::Draw(DG::Image::SP img_)
{
	ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);//this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
	draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
	this->ApplySize(draw);
	ML::Box2D src = this->srcBase;
	img_->Draw(draw, src);
}

//アニメーション簡略化関数
void BObject::AnimSet(Anim anim_, int cnt_, ML::Box2D& src_)
{
	int animNum = cnt_ % anim_.Max;
	src_.x = anim_.Pos[animNum].x;
	src_.y = anim_.Pos[animNum].y;
}

//カーソルの照準が合ったらサイズを上げる関数
void BObject::InCursorSizeUp()
{
	//カーソルが当たっていたらサイズアップ
	if (true == ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
		if (this->addSize < this->addSizeMax) {
			++this->addSize;
		}
	}
	//カーソルが当たっていなければ元のサイズ
	else if (this->addSize > 0) {
		--this->addSize;
	}
}

//オブジェクトとの当たり判定
bool BObject::CheckHitToDataObject(const ML::Box2D& me_)
{
	//敵すべてと当たり判定を行う;
	for (auto it = ge->qa_DataObjects->begin();
		it != ge->qa_DataObjects->end();
		++it)
	{
		//すでに「タスク状態が死亡」になっている敵は当たり判定から除外する
		if ((*it)->CheckState() == BTask::State::Kill) {
			continue;
		}
		//自分だったら除外
		if ((*it)->pos == this->pos) {
			continue;
		}
		//当たり判定
		ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
		if (you.Hit(me_)) {
			return true;
		}
		else {
			continue;
		}
	}
	return false;

}

//敵データオブジェクトのタイプごとの更新処理
void BObject::DataEnemyUpDate()
{
	//ゲームモード時に敵を生成したかというフラグをリセット
	if (true == this->created) {
		this->created = false;
	}
	//未設置時の処理
	if (false == this->puted) {
		this->addSize = 0;
		//マウスが離されたら
		if (ge->qa_Cursor->CheckLeftState(BuState::OFF)) {
			//マップ上にカーソルがない（ツールバーの上など）場合、設置せず消滅
			if (false == ge->qa_Cursor->inGameMap) {
				this->Kill();
				--ge->qa_Cursor->moveCount;
				return;
			}
			//設置しようとした場所に他のデータオブジェクトがある場合も消滅
			else if (true == this->CheckHitToDataObject(this->hitBase.OffsetCopy(this->pos))) {
				this->Kill();
				--ge->qa_Cursor->moveCount;
				return;
			}
			//大丈夫なら設置完了
			this->puted = true;
		}
		//マウスボタン長押し中なら、カーソルのモードは固定
		this->moveMode = true;
		this->pos = ge->qa_Cursor->pos;
	}
	//設置済みの場合
	else {
		//カーソル照準時サイズアップ
		this->InCursorSizeUp();
		//カーソル照準時
		if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			//左クリックなら未設置にする処理
			if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				this->puted = false;
			}
			//右クリック中なら消滅
			if (ge->qa_Cursor->cursorMode == CursorMode::ENEMY) {
				if (ge->qa_Cursor->CheckRightState(BuState::ON)) {
					this->Kill();
					if (true == this->addedToMoveCount) {
						--ge->qa_Cursor->moveCount;
					}
					return;
				}
			}
			//移動モードにする
			this->moveMode = true;
		}
	}

	this->ApplyToMoveCount();
}

//敵データオブジェクトの描画処理
void BObject::DataEnemyDraw(DG::Image::SP cantPutImg_, DG::Image::SP img_)
{
	//データオブジェクトの当たり判定矩形描画
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		this->ApplySize(draw);
		ML::Box2D src(0, 0, 1, 1);
		cantPutImg_->Draw(draw, src);
	}
	//データオブジェクト描画
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		this->ApplySize(draw);
		ML::Box2D src(0, 0, imgSizeW, imgSizeW);
		img_->Draw(draw, src);
	}
}

//カーソルに照準されていることを伝える
void BObject::ApplyToMoveCount()
{
	//移動モードなら
	if (true == this->moveMode) {
		//カーソルの照準中に追加
		if (false == this->addedToMoveCount) {
			++ge->qa_Cursor->moveCount;
			this->addedToMoveCount = true;
		}
	}
	//移動モードでなければ
	else {
		//カーソルの照準中に追加していたら減らす
		if (true == this->addedToMoveCount) {
			--ge->qa_Cursor->moveCount;
			this->addedToMoveCount = false;
		}
	}
	this->moveMode = false;
}