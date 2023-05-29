//-------------------------------------------------------------------
//�񎟌��z��}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Enemy.h"

namespace  Map2D
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
		this->render2D_Priority[1] = 0.9f;
		//�}�b�v�̃[���N���A
		for (int y = 0; y < 100; ++y) {
			for (int x = 0; x < 100; ++x) {
				this->arr[y][x] = 0;
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);
		this->drawBase = ML::Box2D(0, 0, 0, 0);
		//�}�b�v�`�b�v���̏�����
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		this->img.reset();
		delete[] this->chipArr;

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
		//�J���������S�Ƀ}�b�v�O���w���Ă��邩���ׂ�
		if (false == this->hitBase.Hit(ge->camera2D)) {
			return;	//���S�ɊO�ɏo�Ă����炻�̎��_�ŕ`�揈��������߂�
		}
		//�J�����ƃ}�b�v���d�Ȃ��Ă��邾���̋�`�����
		//�J����
		ML::Rect c = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h
		};
		//�}�b�v
		ML::Rect m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h
		};

		//�Q�̋�`���d�Ȃ��Ă���͈͂����̋�`�����߂�
		ML::Rect isr;
		isr.left = max(c.left, m.left);
		isr.top = max(c.top, m.top);
		isr.right = min(c.right, m.right);
		isr.bottom = min(c.bottom, m.bottom);

		//���[�v�͈͂�����
		int sx, sy, ex, ey;
		sx = isr.left / this->length;
		sy = isr.top / this->length;
		ex = (isr.right - 1) / this->length;
		ey = (isr.bottom - 1) / this->length;

		//�w�i�̕`��
		for (int i = 0; i < this->backGrounds; ++i) {
			ML::Box2D draw(0, 0, this->backGround[i].imgSizeW, this->backGround[i].imgSizeH);
			ML::Box2D src(0, 0, this->backGround[i].imgSizeW, this->backGround[i].imgSizeH);

			//�X�N���[���Ή�
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			//�`���[�g���A�����͕`���ύX
			DrawSetIfTuto(draw,src);
			this->backGround[i].img->Draw(draw, src);
		}

		//��ʓ��͈̔͂����`��
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				ML::Box2D draw = this->drawBase;
				draw.Offset(x * this->drawBase.w, y * this->drawBase.h);//�\���ʒu�𒲐�
				ML::Box2D src = this->chipData[this->arr[y][x]].srcBox;
				//�X�N���[���Ή�
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				//�`���[�g���A�����͕`���ύX
				DrawSetIfTuto(draw,src);
				this->img->Draw(draw, src);
			}
		}
	}
	//-------------------------------------------------------------------
	//�֐���`
	//-------------------------------------------------------------------
	//�J�������}�b�v�[���璴�������Ȃ��悤��
	void Object::AdjustCameraPos()
	{
		//�J�����ƃ}�b�v�͈̔͂�p��
		ML::Rect c = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h
		};
		ML::Rect m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h
		};

		//�J�����̈ʒu�𒲐�
		if (c.right > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top < m.top) { ge->camera2D.y = m.top; }

		//�}�b�v���J������菬�����ꍇ
		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = m.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = m.top; }

		//�}�b�v���ŏ��Ȃ�J������y�͌Œ肳����
		int mapSizeMinY = ge->camera2D.h / ge->qa_Map->length;
		if (ge->camera2D.h % ge->qa_Map->length != 0) {
			++mapSizeMinY;
		}
		if (this->sizeY <= mapSizeMinY) {
			ge->camera2D.y = m.top;
		}

	}

	//�w�肵����`�ƃ}�b�v�Ƃ̓����蔻��
	bool Object::CheckHit(const ML::Box2D& hit_)
	{
		//��`
		ML::Rect r = {
			hit_.x,
			hit_.y,
			hit_.x + hit_.w,
			hit_.y + hit_.h
		};
		//�}�b�v
		ML::Rect m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h
		};
		//�e�픻���������
		bool foundGround = false;
		this->foundSpike = false;
		this->foundGoal = false;


		//��`���}�b�v�O�ɏo�Ă�����T�C�Y��ύX����
		//���E�Ȃ�ǔ����t����
		if (r.left < m.left) {
			r.left = m.left;
			foundGround = true;
		}
		if (r.right > m.right) { 
			r.right = m.right;
			foundGround = true;
		}

		if (r.top < m.top) { r.top = m.top; }
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }

		//���[�v�͈͒����i���肵�������̂̍��W���}�b�v�`�b�v�P�ʂ̒l�ɂ���j
		int sx, sy, ex, ey;
		sx = r.left / 32;
		sy = r.top / 32;
		ex = (r.right - 1) / 32;
		ey = (r.bottom - 1) / 32;

		//�͈͓��̏�Q����T��
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				//�n��
				if (this->chipData[this->arr[y][x]].chipType == ChipType::GROUND) {
					foundGround = true;
				}
				//�j
				else if (this->chipData[this->arr[y][x]].chipType == ChipType::SPIKE) {
					this->foundSpike = true;
					foundGround = true;
				}
				//�S�[��
				else if (this->chipData[this->arr[y][x]].chipType == ChipType::GOAL) {
					this->foundGoal = true;
				}
			}
		}
		return foundGround;
	}

	//�}�b�v��G�̃��[�h
	bool Object::Load(const string& fpath_)
	{
		//�t�@�C�����J���i�ǂݍ��݁j
		ifstream fin(fpath_);
		if (!fin) { return false; }//�ǂݍ��ݎ��s

		//�w�i�̓ǂݍ���
		//�w�i�摜�̐�
		fin >> this->backGrounds;
		//���Ԃ�ǂݍ���
		for (int i = 0; i < this->backGrounds; ++i)
		{
			string bgFileName, bgFilePath;
			fin >> bgFileName;
			bgFilePath = "./data/image/Map/" + bgFileName;
			this->backGround[i].img = DG::Image::Create(bgFilePath);
			fin >> this->backGround[i].imgSizeW;
			fin >> this->backGround[i].imgSizeH;
		}

		//�`�b�v�t�@�C�����̓ǂݍ��݂ƁA�摜�̃��[�h
		string  chipFileName, chipFilePath;
		fin >> chipFileName;
		chipFilePath = "./data/map/" + chipFileName;
		this->LoadChipData(chipFilePath);

		//�G�̓ǂݍ���
		int enemyCnt;
		fin >> enemyCnt;
		//�G��0�łȂ���΍s��
		if (enemyCnt != 0) {
			int nameStrCnt = 0;	//�t�@�C�����̕�����
			int ext = 4;	//�u.txt�v�g���q�̕�����(extension)
			//�}�b�v���𒊏o
			for (int c = (int)fpath_.size() - ext - 1; fpath_[c] != '/'; --c) {
				++nameStrCnt;
			}
			string mapFilePath = fpath_.substr(0, fpath_.size() - ext - nameStrCnt);			//�}�b�v�t�@�C���܂ł̃p�X
			string mapFileName = fpath_.substr(fpath_.size() - ext - nameStrCnt, nameStrCnt);	//�}�b�v�t�@�C����

			//�G��1��ނ��ǂݍ���
			for (int i = 1; i <= enemyCnt; ++i) {
				string enemyFileName;
				fin >> enemyFileName;
				this->LoadEnemy(mapFilePath + mapFileName + "_" + enemyFileName);
			}
		}
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->sizeX >> this->sizeY;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * this->length, this->sizeY * this->length);

		//�}�b�v�z��f�[�^�̓ǂݍ���
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				fin >> this->arr[y][x];
			}
		}
		fin.close();

		return true;
	}

	//�`�b�v�f�[�^���t�@�C������擾
	bool Object::LoadChipData(const string& fpath_)
	{
		//�t�@�C�����J���i�ǂݍ��݁j
		ifstream fin(fpath_);
		if (!fin) { return false; }//�ǂݍ��ݎ��s
		//��̐�
		fin >> this->columns;

		//�摜�ǂݍ���
		string fileNameImg;
		fin >> fileNameImg;
		string imgPath = "./data/image/Map/" + fileNameImg;
		this->img = DG::Image::Create(imgPath);

		//�摜�̔z��f�[�^�ǂݍ���
		string fileNameArr;
		fin >> fileNameArr;
		LoadMapChipArray("./data/mapChip/" + fileNameArr);

		//�摜�T�C�Y�c
		fin >> this->imgSizeH;

		//�摜�T�C�Y��
		fin >> this->imgSizeW;

		//�`�b�v��
		fin >> this->tiles;

		//��ӂ̒���
		fin >> this->length;

		//�`��p�̋�`��ݒ�
		this->drawBase = ML::Box2D(0, 0, this->length, this->length);

		//�^�C�����Ƃɏ���ǂݍ���
		for (int i = 0; i < tiles; ++i) {
			int x = (i % columns) * 32;
			int y = (i / columns) * 32;
			this->chipData[i].srcBox = ML::Box2D(x, y, this->length, this->length);

			string type;
			//�^�C�v���ǂݍ���
			fin >> type;
			if (type == "ground") {
				this->chipData[i].chipType = ChipType::GROUND;
			}
			else if (type == "spike") {
				this->chipData[i].chipType = ChipType::SPIKE;
			}
			else if (type == "goal") {
				this->chipData[i].chipType = ChipType::GOAL;
			}
			else {
				this->chipData[i].chipType = ChipType::AIR;
			}
		}

		fin.close();
		return true;
	}
	//�`�b�v�̌����p�z�񃍁[�h
	bool  Object::LoadMapChipArray(const string& fpath_)
	{
		ifstream fin(fpath_);
		if (!fin) { return false; }//�ǂݍ��ݎ��s
		fin >> this->chipArrTypeMax;

		this->chipArr = new ChipArr[this->chipArrTypeMax];

		for (int t = 0; t < this->chipArrTypeMax; ++t) {
			for (int arr = 0; arr < ChipArr::max; ++arr) {
				fin >> this->chipArr[t][arr];
			}
		}
		fin.close();
		return true;
	}

	//�G�̏���ǂݍ���
	bool Object::LoadEnemy(const string& efpath_)
	{
		ifstream fin(efpath_);
		if (!fin) { return false; }
		int enemyType;		//�G�̃^�C�v
		int enemyCnt;		//�G�̐�
		fin >> enemyType;
		fin >> enemyCnt;
		for (int e = 0; e < enemyCnt; ++e) {
			ML::Vec2 enemyPos;	//�G�̏����ʒu
			fin >> enemyPos.x;
			fin >> enemyPos.y;
			//�G�o������
			switch (enemyType) {
			case 0: {
				auto enemy = Enemy00::Object::Create(true);
				enemy->pos = enemyPos;
				break;
			}
			case 1: {
				auto enemy = Enemy01::Object::Create(true);
				enemy->pos = enemyPos;
				break;
			}
			}
		}
		fin.close();
		return true;
	}

	//�}�b�v�T�C�Y�ύX
	void Object::ChangeMapSize(int sizeX_, int sizeY_)
	{
		this->sizeX = sizeX_;
		this->sizeY = sizeY_;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * this->length, this->sizeY * this->length);
	}


	//�`�b�v���Ƃ̃G�t�F�N�g
	void Object::chipEffect() {
		
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