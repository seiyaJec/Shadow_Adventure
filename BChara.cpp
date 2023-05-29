//----------------------------------------------------------------------------------
// キャラクタ汎用スーパークラス
//----------------------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map2D.h"
#include "Task_EnemyTest.h"


//----------------------------------------------------------------------------------
//めり込まない移動処理
void BChara::CheckMove(ML::Vec2& e_)
{
	//マップが存在するか調べてからアクセス
	if (nullptr == ge->qa_Map) { return; }//マップがなければ判定しない

	//横軸に対する移動
	while (e_.x != 0) {
		float preX = this->pos.x;
		if (e_.x >= 1) {
			this->pos.x += 1;
			e_.x -= 1;
		}
		else if (e_.x <= -1) {
			this->pos.x -= 1;
			e_.x += 1;
		}
		else {
			this->pos.x += e_.x;
			e_.x = 0;
		}
		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		if (true == ge->qa_Map->CheckHit(hit)) {
			this->pos.x = preX;		//移動をキャンセル
		}
	}

	//縦軸に対する移動
	while (e_.y != 0) {
		float preY = this->pos.y;
		if (e_.y >= 1) {
			this->pos.y += 1;
			e_.y -= 1;
		}
		else if (e_.y <= -1) {
			this->pos.y -= 1;
			e_.y += 1;
		}
		else {
			this->pos.y += e_.y;
			e_.y = 0;
		}
		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		if (true == ge->qa_Map->CheckHit(hit)) {
			this->pos.y = preY;		//移動をキャンセル
		}
	}	

}
//----------------------------------------------------------------------------------
//アングル情報を数値に変更
int BChara::Angle_LR_ChangeAngleToInt(Angle_LR ang_) 
{
	if (ang_ == Angle_LR::Right) {
		return 1;
	}
	else {
		return -1;
	}
}
//----------------------------------------------------------------------------------
//アングルを反転
void BChara::Angle_LR_Reverse(Angle_LR& ang_)
{
	if (ang_ == Angle_LR::Right) {
		ang_ = Angle_LR::Left;
		return;
	}
	else {
		ang_ = Angle_LR::Right;
		return;
	}
	return;
}
//----------------------------------------------------------------------------------
//int型からアングルに変換
void BChara::Angle_LR_ChangeIntToAngle(Angle_LR& ang_, int aInt_) {
	if (aInt_ >= 0) {
		ang_ = Angle_LR::Right;
	}
	else {
		ang_ = Angle_LR::Left;
	}
}
//----------------------------------------------------------------------------------
//敵との当たり判定
float BChara::CheckDamageToEnemy(ML::Box2D hitBox_)
{
	//サイズが0なら処理しない
	if (ge->qa_Enemys == nullptr) {
		return 0;
	}
	//敵すべてと当たり判定を行う;
	for (auto it = ge->qa_Enemys->begin();
		it != ge->qa_Enemys->end();
		++it)
	{
		//すでに「タスク状態が死亡」になっている敵は当たり判定から除外する
		if ((*it)->CheckState() == BTask::State::Kill) { continue; }
		//被ダメージ中の場合も除外する
		if (true == (*it)->damaged) { continue; }
		//敵対象と衝突判定&ダメージを与える処理
		ML::Box2D me = hitBox_.OffsetCopy(this->pos);
		//敵キャラクタの当たり判定矩形を用意
		ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
		//重なりを判定
		if (true == you.Hit(me)) {
			(*it)->moveCnt = 0;
			(*it)->statHP -= this->statAttack;
			(*it)->damaged = true;
			(*it)->damagedAngle_LR = this->angle_LR;
			//敵吹っ飛ばし処理
			float vecRate = (float)this->statAttack / (*it)->statHPMax;
			(*it)->moveVec.x = (vecRate * (*it)->damagedVecMax.x)
				* Angle_LR_ChangeAngleToInt(this->angle_LR);
			(*it)->moveVec.y = (vecRate * (*it)->damagedVecMax.y);
			return vecRate;
		}
	}
	return 0;
}
//----------------------------------------------------------------------------------
//プレイヤーとの当たり判定
bool BChara::CheckDamageToPlayer()
{
	//プレイヤーと当たり判定を行う
	if (nullptr == ge->qa_Player) {
		return false;
	}

	//すでに「タスク状態が死亡」になっている敵は当たり判定から除外する
	if (ge->qa_Player->CheckState() == BTask::State::Kill) {
		return false;
	}

	//被ダメージ中の場合も除外する
	if (true == ge->qa_Player->damaged) {
		return false;
	}

	//敵対象と衝突判定&ダメージを与える処理
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	//敵キャラクタの当たり判定矩形を用意
	ML::Box2D you = ge->qa_Player->hitBase.OffsetCopy(ge->qa_Player->pos);
	//重なりを判定
	if (false == you.Hit(me)) {
		return false;
	}

	//プレイヤー
	ge->qa_Player->statHP -= this->statAttack;
	//HPが0ならキル
	if (ge->qa_Player->statHP <= 0) {
		ge->gameOverflag = true;
		return true;
	}
	else {
		//被ダメージ開始処理
		ge->qa_Player->damaged = true;			
		ge->qa_Player->damagedStopMove = true;	
		ge->qa_Player->damagedCount = 0;		
		//プレイヤーが敵の右側にいる場合
		if (you.x >= me.x + me.w / 2) {
			ge->qa_Player->damagedAngle_LR = Angle_LR::Right;
		}
		//左側の場合
		else {
			ge->qa_Player->damagedAngle_LR = Angle_LR::Left;
		}
		//プレイヤー吹っ飛ばし処理
		ge->qa_Player->moveVec.x = ge->qa_Player->damagedVecMax.x * Angle_LR_ChangeAngleToInt(ge->qa_Player->damagedAngle_LR);
		ge->qa_Player->moveVec.y = ge->qa_Player->damagedVecMax.y;
		ge->qa_Player->StartCameraShake(6, 2, 6, 6);
	}

	return true;

}
//----------------------------------------------------------------------------------
//足元接触判定
bool BChara::CheckFoot()
{
	//当たり判定をもとにして足元矩形を生成
	ML::Box2D  foot(
		this->hitBase.x,
		this->hitBase.y + this->hitBase.h,
		this->hitBase.w,
		1
	);
	foot.Offset(this->pos);

	if (nullptr == ge->qa_Map) { return false; }//マップがなければ判定しない（できない）
	//マップと接触判定
	return ge->qa_Map->CheckHit(foot);
}
//----------------------------------------------------------------------------------
//頭接触判定
bool BChara::CheckHead()
{
	//当たり判定をもとにして頭矩形を生成
	ML::Box2D head(
		this->hitBase.x,
		this->hitBase.y - 1,
		this->hitBase.w,
		1
	);
	head.Offset(this->pos);

	if (nullptr == ge->qa_Map) { return false; }//マップが無ければ判定しない（できない）
	//マップと接触判定
	return ge->qa_Map->CheckHit(head);
}

//----------------------------------------------------------------------------------
//壁接触判定
bool BChara::CheckFront()
{
	//当たり判定をもとにして正面矩形を生成
	ML::Box2D front(
		this->hitBase.x,
		this->hitBase.y,
		1,
		this->hitBase.h
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
	//マップと接触判定
	return ge->qa_Map->CheckHit(front);
}
//----------------------------------------------------------------------------------
//向きを踏まえて位置変更
void BChara::MovePosToAngle(ML::Box2D& box_, int x_, int y_)
{
	box_.x += x_ * this->Angle_LR_ChangeAngleToInt(this->angle_LR);
	box_.y += y_;
}
//----------------------------------------------------------------------------------
//被ダメージ時の移動がなくなったか
bool BChara::IsZeroDamagedMove()
{
	//ダメージ処理じゃなければfalse
	if (false == this->damaged) { 
		return false; 
	}
	//y方向が0か確認
	if (this->moveVec.y <= 0) {
		return false;
	}
	//x方向が0か確認
	if (this->damagedAngle_LR == Angle_LR::Right) {
		if (this->moveVec.x > 0) {
			return false;
		}
	}
	else {
		if (this->moveVec.x < 0) {
			return false;
		}
	}
	return true;
}
//----------------------------------------------------------------------------------
//プレイヤー探知の処理
bool BChara::SerchPlayer()
{
	//プレイヤーが生成されてなければ処理しない
	if (nullptr == ge->qa_Player) {
		return false;
	}
	//プレイヤーと探知矩形の当たり判定
	ML::Box2D pl = ge->qa_Player->hitBase.OffsetCopy(ge->qa_Player->pos);
	ML::Box2D me = this->playerSerchBox.OffsetCopy(this->pos);
	if (true == pl.Hit(me)) {
		if (ge->qa_Player->pos.x >= this->pos.x) {
			this->angle_LR = Angle_LR::Right;
		}
		else {
			this->angle_LR = Angle_LR::Left;
		}
		return true;
	}
	else {
		return false;
	}
}

//----------------------------------------------------------------------------------
//当たり判定のデバッグ
void BChara::DebugDrawBox(ML::Box2D const box_)
{
	//デバッグモードが無効なら終了
	if (false == ge->debugFlagAll
		&& false == this->debugFlag) {
		return;
	}
	//まだ矩形が用意されていなければ終了
	if (nullptr == ge->imgDebug) {
		return;
	}
	//デバッグ用の矩形を表示
	ML::Box2D drawD = box_.OffsetCopy(this->pos);
	drawD.Offset(-ge->camera2D.x, -ge->camera2D.y);
	ML::Box2D srcD(0, 0, 1, 1);
	ge->imgDebug->Draw(drawD, srcD);
}
//----------------------------------------------------------------------------------
//アニメーションする画像の選択
void BChara::AnimSet(Anim anim_, int cnt_)
{
	int animNum = cnt_ % anim_.Max;
	this->srcBase.x = anim_.Pos[animNum].x;
	this->srcBase.y = anim_.Pos[animNum].y;
}
//----------------------------------------------------------------------------------