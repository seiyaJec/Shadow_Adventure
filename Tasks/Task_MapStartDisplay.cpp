//-------------------------------------------------------------------
//�}�b�v�̃X�^�[�g�n�_��\��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapStartDisplay.h"

namespace  MapStartDisplay
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������

		//�摜�̗p��
		for (int i = 0; i < 15; ++i) {
			//���O������΁A�o�^����������
			if (ge->qa_MapSelect->mapData[i].imageFileName != "") {

				bool foundSameFile = false;

				//�摜���z��ւ̓o�^����
				for (int j = 0; j < 15; ++j) {
					//�摜���z����ɓ������O���������珈�����~
					if (ge->qa_MapSelect->mapData[i].imageFileName == this->imgFileArr[j]) {
						foundSameFile = true;
						break;
					}
					//���o�^�̓Y���܂ŗ�����o�^�����E�����I��
					if (this->imgFileArr[j] == "") {
						this->imgFileArr[j] = ge->qa_MapSelect->mapData[i].imageFileName;
						this->img[j] = DG::Image::Create("./data/image/Map/" + imgFileArr[j]);
						break;
					}
				}
			}
		}
		
		//�摜�D��x
		this->render2D_Priority[1] = 0.49f;
		//�T�C�Y
		this->SetImgSize(800, 800);
		this->displaySizeH = 11;
		this->displaySizeW = 14;
		this->chipLength = 32;
		//�ʒu���
		this->pos = ML::Vec2(440, 32);			//���̃^�X�N�ł͍�����W
		//���^�X�N�̐���
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		for (int i = 0; i < 100; ++i) {
			this->img[i].reset();
		}

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		int displayPos = (ge->qa_MapSelect->page * 3) + ge->qa_MapSelect->selectingPos;//���݌��Ă���t�@�C���ԍ�

		//�t�@�C�����ɉ摜�������邩�m�F
		int imgFileNum = this->FindImageFile(displayPos);

		//�摜������������`��
		if (imgFileNum != -1) {
			//1�}�X���`��
			for (int y = 0; y < this->displaySizeH; ++y) {
				for (int x = 0; x < this->displaySizeW; ++x) {
					int chipNum = ge->qa_MapSelect->mapData[displayPos].startPointMapData[y][x];
					//�`���`
					ML::Box2D draw(
						x * this->chipLength,
						y * this->chipLength,
						this->chipLength,
						this->chipLength);

					draw.Offset(this->pos);

					//�ǂݍ��݌���`
					ML::Box2D src;
					src.x = (chipNum * this->chipLength) % this->imgSizeW;
					src.y = ((chipNum * this->chipLength) / this->imgSizeW) * chipLength;
					src.w = this->chipLength;
					src.h = this->chipLength;

					this->img[imgFileNum]->Draw(draw, src);
				}
			}
		}

	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�L�����̉摜���ɓ����摜�����邩�T��
	int Object::FindImageFile(int displayPos_)
	{
		for (int i = 0; i < 100; ++i) {
			if (ge->qa_MapSelect->mapData[displayPos_].imageFileName != ""
				&& ge->qa_MapSelect->mapData[displayPos_].imageFileName == this->imgFileArr[i]) {
				return i;
			}
			if (this->imgFileArr[i] == "") {
				return -1;
			}
		}
		return -1;
	}



	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}