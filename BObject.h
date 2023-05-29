#pragma once
#pragma warning(disable:4996)
#pragma once
#include "GameEngine_Ver3_83.h"
#include "MyStructure.h"
//--------------------------------------------------------------------------------------------
// �ȃf�[�^�I�u�W�F�N�g�N���X
//--------------------------------------------------------------------------------------------

class BObject :public BTask
{
	//�ύX�s����������������������������������������
public:
	typedef shared_ptr<BObject>		SP;
	typedef weak_ptr<BObject>		WP;
public:
	//�ύX������������������������������������������
	ML::Box2D hitBase;			//�����蔻���`
	ML::Box2D drawBase;			//�`��p��`
	ML::Box2D srcBase;			//���摜�I���̋�`
	ML::Vec2 pos;				//�ʒu���
	int	imgSizeW;				//�摜����
	int imgSizeH;				//�摜�c��
	int addSize;				//���l�������X�P�[���A�b�v
	int addSizeMax;				//�X�P�[���A�b�v�ő�
	bool puted;					//�ݒu���Ă��邩
	bool moveMode;				//�ړ����[�h
	bool addedToMoveCount;		//�J�[�\���Ɉړ����[�h����`������
	bool created;				//�v���C���[�h�ϊ����ɃL�����𐶐�������


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

	void SetImgSize(int w_, int h_);										//�摜�ݒ�
	void ApplySize(ML::Box2D& box_);										//�X�P�[���A�b�v
	void ApplySizeMove(ML::Box2D& box_);									//�X�P�[����K�p
	void InCursorSizeUp();													//�J�[�\���Ə����ɃX�P�[���A�b�v
	void Draw(DG::Image::SP	img_);											//�`��ȗ����̂��߂̊֐�
	void AnimSet(Anim anim_, int cnt_, ML::Box2D& src_);					//�A�j���[�V�������s�p�֐�
	void DataEnemyUpDate();													//�G�f�[�^�I�u�W�F�N�g�̍X�V����
	void DataEnemyDraw(DG::Image::SP cantPutImg_,DG::Image::SP img_);		//�G�f�[�^�I�u�W�F�N�g�̕`�揈��
	bool CheckHitToDataObject(const ML::Box2D& me_);						//�f�[�^�I�u�W�F�N�g���m�̓����蔻��
	void ApplyToMoveCount();												//�J�[�\���ɏƏ�����Ă��邱�Ƃ�`����֐�
};
