//----------------------------------------------------------------------------------
// �}�b�v�쐬�p�I�u�W�F�N�g�N���X
//----------------------------------------------------------------------------------
#include "BObject.h"
#include "MyPG.h"

//�摜�T�C�Y�̐ݒ�
void BObject::SetImgSize(int w_, int h_)
{
	imgSizeW = w_;
	imgSizeH = h_;
}

//�T�C�Y�ǉ��̓K�p
void BObject::ApplySize(ML::Box2D& box_)
{
	float sizeRate = 1 + (this->addSize * 0.1f);
	box_.x -= (int)(box_.w * sizeRate) - box_.w;
	box_.y -= (int)(box_.h * sizeRate) - box_.h;
	box_.w = (int)(box_.w * sizeRate);
	box_.h = (int)(box_.h * sizeRate);
}

//�T�C�Y�ǉ��̓K�p
void BObject::ApplySizeMove(ML::Box2D& box_)
{
	float sizeRate = 1 + (this->addSize * 0.1f);
	box_.x -= (int)(box_.w / 2 * sizeRate) - box_.w / 2;
	box_.y -= (int)(box_.h / 2 * sizeRate) - box_.h / 2;
	box_.w = (int)(box_.w * sizeRate);
	box_.h = (int)(box_.h * sizeRate);
}

//�摜�̕`��
void BObject::Draw(DG::Image::SP img_)
{
	ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);//this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
	draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
	this->ApplySize(draw);
	ML::Box2D src = this->srcBase;
	img_->Draw(draw, src);
}

//�A�j���[�V�����ȗ����֐�
void BObject::AnimSet(Anim anim_, int cnt_, ML::Box2D& src_)
{
	int animNum = cnt_ % anim_.Max;
	src_.x = anim_.Pos[animNum].x;
	src_.y = anim_.Pos[animNum].y;
}

//�J�[�\���̏Ə�����������T�C�Y���グ��֐�
void BObject::InCursorSizeUp()
{
	//�J�[�\�����������Ă�����T�C�Y�A�b�v
	if (true == ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
		if (this->addSize < this->addSizeMax) {
			++this->addSize;
		}
	}
	//�J�[�\�����������Ă��Ȃ���Ό��̃T�C�Y
	else if (this->addSize > 0) {
		--this->addSize;
	}
}

//�I�u�W�F�N�g�Ƃ̓����蔻��
bool BObject::CheckHitToDataObject(const ML::Box2D& me_)
{
	//�G���ׂĂƓ����蔻����s��;
	for (auto it = ge->qa_DataObjects->begin();
		it != ge->qa_DataObjects->end();
		++it)
	{
		//���łɁu�^�X�N��Ԃ����S�v�ɂȂ��Ă���G�͓����蔻�肩�珜�O����
		if ((*it)->CheckState() == BTask::State::Kill) {
			continue;
		}
		//�����������珜�O
		if ((*it)->pos == this->pos) {
			continue;
		}
		//�����蔻��
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

//�G�f�[�^�I�u�W�F�N�g�̃^�C�v���Ƃ̍X�V����
void BObject::DataEnemyUpDate()
{
	//�Q�[�����[�h���ɓG�𐶐��������Ƃ����t���O�����Z�b�g
	if (true == this->created) {
		this->created = false;
	}
	//���ݒu���̏���
	if (false == this->puted) {
		this->addSize = 0;
		//�}�E�X�������ꂽ��
		if (ge->qa_Cursor->CheckLeftState(BuState::OFF)) {
			//�}�b�v��ɃJ�[�\�����Ȃ��i�c�[���o�[�̏�Ȃǁj�ꍇ�A�ݒu��������
			if (false == ge->qa_Cursor->inGameMap) {
				this->Kill();
				--ge->qa_Cursor->moveCount;
				return;
			}
			//�ݒu���悤�Ƃ����ꏊ�ɑ��̃f�[�^�I�u�W�F�N�g������ꍇ������
			else if (true == this->CheckHitToDataObject(this->hitBase.OffsetCopy(this->pos))) {
				this->Kill();
				--ge->qa_Cursor->moveCount;
				return;
			}
			//���v�Ȃ�ݒu����
			this->puted = true;
		}
		//�}�E�X�{�^�����������Ȃ�A�J�[�\���̃��[�h�͌Œ�
		this->moveMode = true;
		this->pos = ge->qa_Cursor->pos;
	}
	//�ݒu�ς݂̏ꍇ
	else {
		//�J�[�\���Ə����T�C�Y�A�b�v
		this->InCursorSizeUp();
		//�J�[�\���Ə���
		if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			//���N���b�N�Ȃ疢�ݒu�ɂ��鏈��
			if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				this->puted = false;
			}
			//�E�N���b�N���Ȃ����
			if (ge->qa_Cursor->cursorMode == CursorMode::ENEMY) {
				if (ge->qa_Cursor->CheckRightState(BuState::ON)) {
					this->Kill();
					if (true == this->addedToMoveCount) {
						--ge->qa_Cursor->moveCount;
					}
					return;
				}
			}
			//�ړ����[�h�ɂ���
			this->moveMode = true;
		}
	}

	this->ApplyToMoveCount();
}

//�G�f�[�^�I�u�W�F�N�g�̕`�揈��
void BObject::DataEnemyDraw(DG::Image::SP cantPutImg_, DG::Image::SP img_)
{
	//�f�[�^�I�u�W�F�N�g�̓����蔻���`�`��
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		this->ApplySize(draw);
		ML::Box2D src(0, 0, 1, 1);
		cantPutImg_->Draw(draw, src);
	}
	//�f�[�^�I�u�W�F�N�g�`��
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		this->ApplySize(draw);
		ML::Box2D src(0, 0, imgSizeW, imgSizeW);
		img_->Draw(draw, src);
	}
}

//�J�[�\���ɏƏ�����Ă��邱�Ƃ�`����
void BObject::ApplyToMoveCount()
{
	//�ړ����[�h�Ȃ�
	if (true == this->moveMode) {
		//�J�[�\���̏Ə����ɒǉ�
		if (false == this->addedToMoveCount) {
			++ge->qa_Cursor->moveCount;
			this->addedToMoveCount = true;
		}
	}
	//�ړ����[�h�łȂ����
	else {
		//�J�[�\���̏Ə����ɒǉ����Ă����猸�炷
		if (true == this->addedToMoveCount) {
			--ge->qa_Cursor->moveCount;
			this->addedToMoveCount = false;
		}
	}
	this->moveMode = false;
}