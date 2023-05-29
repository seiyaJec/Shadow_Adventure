//----------------------------------------------------------------------------------
// �L�����N�^�ėp�X�[�p�[�N���X
//----------------------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Map2D.h"
#include "Task_EnemyTest.h"


//----------------------------------------------------------------------------------
//�߂荞�܂Ȃ��ړ�����
void BChara::CheckMove(ML::Vec2& e_)
{
	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	if (nullptr == ge->qa_Map) { return; }//�}�b�v���Ȃ���Δ��肵�Ȃ�

	//�����ɑ΂���ړ�
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
			this->pos.x = preX;		//�ړ����L�����Z��
		}
	}

	//�c���ɑ΂���ړ�
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
			this->pos.y = preY;		//�ړ����L�����Z��
		}
	}	

}
//----------------------------------------------------------------------------------
//�A���O�����𐔒l�ɕύX
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
//�A���O���𔽓]
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
//int�^����A���O���ɕϊ�
void BChara::Angle_LR_ChangeIntToAngle(Angle_LR& ang_, int aInt_) {
	if (aInt_ >= 0) {
		ang_ = Angle_LR::Right;
	}
	else {
		ang_ = Angle_LR::Left;
	}
}
//----------------------------------------------------------------------------------
//�G�Ƃ̓����蔻��
float BChara::CheckDamageToEnemy(ML::Box2D hitBox_)
{
	//�T�C�Y��0�Ȃ珈�����Ȃ�
	if (ge->qa_Enemys == nullptr) {
		return 0;
	}
	//�G���ׂĂƓ����蔻����s��;
	for (auto it = ge->qa_Enemys->begin();
		it != ge->qa_Enemys->end();
		++it)
	{
		//���łɁu�^�X�N��Ԃ����S�v�ɂȂ��Ă���G�͓����蔻�肩�珜�O����
		if ((*it)->CheckState() == BTask::State::Kill) { continue; }
		//��_���[�W���̏ꍇ�����O����
		if (true == (*it)->damaged) { continue; }
		//�G�ΏۂƏՓ˔���&�_���[�W��^���鏈��
		ML::Box2D me = hitBox_.OffsetCopy(this->pos);
		//�G�L�����N�^�̓����蔻���`��p��
		ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
		//�d�Ȃ�𔻒�
		if (true == you.Hit(me)) {
			(*it)->moveCnt = 0;
			(*it)->statHP -= this->statAttack;
			(*it)->damaged = true;
			(*it)->damagedAngle_LR = this->angle_LR;
			//�G������΂�����
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
//�v���C���[�Ƃ̓����蔻��
bool BChara::CheckDamageToPlayer()
{
	//�v���C���[�Ɠ����蔻����s��
	if (nullptr == ge->qa_Player) {
		return false;
	}

	//���łɁu�^�X�N��Ԃ����S�v�ɂȂ��Ă���G�͓����蔻�肩�珜�O����
	if (ge->qa_Player->CheckState() == BTask::State::Kill) {
		return false;
	}

	//��_���[�W���̏ꍇ�����O����
	if (true == ge->qa_Player->damaged) {
		return false;
	}

	//�G�ΏۂƏՓ˔���&�_���[�W��^���鏈��
	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	//�G�L�����N�^�̓����蔻���`��p��
	ML::Box2D you = ge->qa_Player->hitBase.OffsetCopy(ge->qa_Player->pos);
	//�d�Ȃ�𔻒�
	if (false == you.Hit(me)) {
		return false;
	}

	//�v���C���[
	ge->qa_Player->statHP -= this->statAttack;
	//HP��0�Ȃ�L��
	if (ge->qa_Player->statHP <= 0) {
		ge->gameOverflag = true;
		return true;
	}
	else {
		//��_���[�W�J�n����
		ge->qa_Player->damaged = true;			
		ge->qa_Player->damagedStopMove = true;	
		ge->qa_Player->damagedCount = 0;		
		//�v���C���[���G�̉E���ɂ���ꍇ
		if (you.x >= me.x + me.w / 2) {
			ge->qa_Player->damagedAngle_LR = Angle_LR::Right;
		}
		//�����̏ꍇ
		else {
			ge->qa_Player->damagedAngle_LR = Angle_LR::Left;
		}
		//�v���C���[������΂�����
		ge->qa_Player->moveVec.x = ge->qa_Player->damagedVecMax.x * Angle_LR_ChangeAngleToInt(ge->qa_Player->damagedAngle_LR);
		ge->qa_Player->moveVec.y = ge->qa_Player->damagedVecMax.y;
		ge->qa_Player->StartCameraShake(6, 2, 6, 6);
	}

	return true;

}
//----------------------------------------------------------------------------------
//�����ڐG����
bool BChara::CheckFoot()
{
	//�����蔻������Ƃɂ��đ�����`�𐶐�
	ML::Box2D  foot(
		this->hitBase.x,
		this->hitBase.y + this->hitBase.h,
		this->hitBase.w,
		1
	);
	foot.Offset(this->pos);

	if (nullptr == ge->qa_Map) { return false; }//�}�b�v���Ȃ���Δ��肵�Ȃ��i�ł��Ȃ��j
	//�}�b�v�ƐڐG����
	return ge->qa_Map->CheckHit(foot);
}
//----------------------------------------------------------------------------------
//���ڐG����
bool BChara::CheckHead()
{
	//�����蔻������Ƃɂ��ē���`�𐶐�
	ML::Box2D head(
		this->hitBase.x,
		this->hitBase.y - 1,
		this->hitBase.w,
		1
	);
	head.Offset(this->pos);

	if (nullptr == ge->qa_Map) { return false; }//�}�b�v��������Δ��肵�Ȃ��i�ł��Ȃ��j
	//�}�b�v�ƐڐG����
	return ge->qa_Map->CheckHit(head);
}

//----------------------------------------------------------------------------------
//�ǐڐG����
bool BChara::CheckFront()
{
	//�����蔻������Ƃɂ��Đ��ʋ�`�𐶐�
	ML::Box2D front(
		this->hitBase.x,
		this->hitBase.y,
		1,
		this->hitBase.h
	);
	//�L�����̌����ɂ���Ĕ��肷��ʒu��ύX
	if (this->angle_LR == Angle_LR::Right) {
		front.x += this->hitBase.w;
	}
	else {
		front.x += -1;
	}

	front.Offset(this->pos);

	if (nullptr == ge->qa_Map) { return false; }//�}�b�v���Ȃ���Δ��肵�Ȃ��i�ł��Ȃ��j
	//�}�b�v�ƐڐG����
	return ge->qa_Map->CheckHit(front);
}
//----------------------------------------------------------------------------------
//�����𓥂܂��Ĉʒu�ύX
void BChara::MovePosToAngle(ML::Box2D& box_, int x_, int y_)
{
	box_.x += x_ * this->Angle_LR_ChangeAngleToInt(this->angle_LR);
	box_.y += y_;
}
//----------------------------------------------------------------------------------
//��_���[�W���̈ړ����Ȃ��Ȃ�����
bool BChara::IsZeroDamagedMove()
{
	//�_���[�W��������Ȃ����false
	if (false == this->damaged) { 
		return false; 
	}
	//y������0���m�F
	if (this->moveVec.y <= 0) {
		return false;
	}
	//x������0���m�F
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
//�v���C���[�T�m�̏���
bool BChara::SerchPlayer()
{
	//�v���C���[����������ĂȂ���Ώ������Ȃ�
	if (nullptr == ge->qa_Player) {
		return false;
	}
	//�v���C���[�ƒT�m��`�̓����蔻��
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
//�����蔻��̃f�o�b�O
void BChara::DebugDrawBox(ML::Box2D const box_)
{
	//�f�o�b�O���[�h�������Ȃ�I��
	if (false == ge->debugFlagAll
		&& false == this->debugFlag) {
		return;
	}
	//�܂���`���p�ӂ���Ă��Ȃ���ΏI��
	if (nullptr == ge->imgDebug) {
		return;
	}
	//�f�o�b�O�p�̋�`��\��
	ML::Box2D drawD = box_.OffsetCopy(this->pos);
	drawD.Offset(-ge->camera2D.x, -ge->camera2D.y);
	ML::Box2D srcD(0, 0, 1, 1);
	ge->imgDebug->Draw(drawD, srcD);
}
//----------------------------------------------------------------------------------
//�A�j���[�V��������摜�̑I��
void BChara::AnimSet(Anim anim_, int cnt_)
{
	int animNum = cnt_ % anim_.Max;
	this->srcBase.x = anim_.Pos[animNum].x;
	this->srcBase.y = anim_.Pos[animNum].y;
}
//----------------------------------------------------------------------------------