#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�񎟌��z��}�b�v
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Map2D
{
	struct ChipData {
		ML::Box2D srcBox;
		ChipType chipType;
		ChipData() :
			srcBox(0, 0, 0, 0),
			chipType(ChipType::AIR)
		{}
	};
	struct BackGroundData {
		DG::Image::SP img;
		int imgSizeW;
		int imgSizeH;
	};
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�t�B�[���h");	//�O���[�v��
	const  string  defName("�}�b�v");	//�^�X�N��
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
		//���L����ϐ��͂����ɒǉ�����
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate()			override;//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF()		override;//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		DG::Image::SP		img;
		DG::Image::SP		bgImg[10];
		int					arr[100][100];
		int					sizeY, sizeX;
		int					imgSizeW, imgSizeH;	//�摜�T�C�Y
		int					tiles;				//�^�C������
		int					length;				//��ӂ̒���
		int					backGrounds;		//�w�i�̐�
		int					columns;			//��̐�
		bool				foundSpike;			//�j������������
		bool				foundGoal;			//�S�[��������������
		BackGroundData		backGround[10];		//�w�i�̑傫��
		ML::Box2D			drawBase;			//�`��p�̋�`
		ML::Box2D			hitBase;			//�s�N�Z���P�ʂ̃}�b�v�T�C�Y������
		ChipData			chipData[1000];		//�e�}�b�v�`�b�v�̃f�[�^�i�^�C�v�����j
		int					chipArrTypeMax;		//�}�b�v�`�b�v�̃^�C�v����
		ChipArr* chipArr;						//�}�b�v�`�b�v�����z��i�錾��cpp�j
		


		bool Load(const string& fpath_);//�}�b�v�ǂݍ���
		bool LoadChipData(const string& fpath_);//�`�b�v�f�[�^�ǂݍ���
		bool LoadMapChipArray(const string& fpath_);//�}�b�v�`�b�v���m�̌����p�z��ǂݍ���
		bool LoadEnemy(const string& efpath_);//�G���ǂݍ���
		bool CheckHit(const ML::Box2D& hit_);//�����蔻��
		
		void ChangeMapSize(int sizeX_, int sizeY_);
		void chipEffect();			//�G�t�F�N�g
		void AdjustCameraPos();
	};
}