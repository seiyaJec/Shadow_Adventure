#include "MyPG.h"
#include "MyStructure.h"
#include "BChara.h"
#include "Task_LoadBG.h"

//�A�j���[�V�������̐ݒ�
bool  LoadAnim(Anim& anim_, const string& fpath_)
{
	//�t�@�C�����J���i�ǂݍ��݁j
	ifstream fin(fpath_);
	if (!fin) { return false; }//�ǂݍ��ݎ��s

	fin >> anim_.imgSizeW;
	fin >> anim_.imgSizeH;
	//�A�j���[�V�����̏��Ԃ̓ǂݍ���
	for (int a = 0; a < 20; ++a) {
		fin >> anim_.Pos[a].x;
		fin >> anim_.Pos[a].y;
		if (!fin) { break; }
		anim_.Pos[a].x *= anim_.imgSizeW;
		anim_.Pos[a].y *= anim_.imgSizeH;
		++anim_.Max;
	}
	fin.close();
	return true;
}


//���[�h�^�X�N�o������
void AppearLoad(int cuts_, int distance_, float speed_, ML::Color color_)
{
	//���[�h�^�X�N�o��
	for (int i = 0; i < cuts_; ++i) {
		auto load = LoadBG::Object::Create(true);
		load->Appear(cuts_, i, distance_, speed_);
		load->color = color_;
		if (i == cuts_ - 1) {
			load->returnFinish = true;
		}
	}
	ge->finishedLoadAppear = false;
}

//���[�h�^�X�N���ŏ���
void DisappearLoad(int distance_, float speed_)
{
	ge->finishedLoadDisappear = false;
	auto load = ge->GetTasks<LoadBG::Object>("�^�X�N���[�h�w�i");
	if (load->size() != 0) {
		int order = 0;
		for (auto it = load->begin();
			it != load->end();
			++it)
		{
			(*it)->Disappear(distance_, speed_);
			++order;
		}
	}
}

//�`���[�g���A�����̕`��
void DrawSetIfTuto(ML::Box2D& drawBox_, ML::Box2D& srcBox_)
{
	if (ge->inTutorial) {
		ge->qa_Tutorial->SetToScreen(drawBox_, srcBox_);
	}
}

//1�ȏ�Ȃ�1�����Z������
void KeyInput::DecrementAll() {
	if (LStickBR > 0) { --LStickBR; }
	if (LStickBL > 0) { --LStickBL; }
	if (B3 > 0) { --B3; }
	if (B1 > 0) { --B1; }
	if (RT > 0) { --RT; }
}

void DataForPlay::Load(const string& fpath_) {
		int nameStrCnt = 0;	//�t�@�C�����̕�����
		int ext = 4;	//�u.txt�v�g���q�̕�����(extension)
		//�}�b�v���𒊏o
		for (int c = (int)fpath_.size() - ext - 1; fpath_[c] != '/'; --c) {
			++nameStrCnt;
		}
		string mapFilePath = fpath_.substr(0, fpath_.size() - ext - nameStrCnt);			//�}�b�v�t�@�C���܂ł̃p�X
		string mapFileName = fpath_.substr(fpath_.size() - ext - nameStrCnt, nameStrCnt);	//�}�b�v�t�@�C����

		//�t�@�C���ǂݍ���
		ifstream fin(mapFilePath + mapFileName + "_" + "DataForPlay.txt");
		//�t�@�C�����Ȃ������ꍇ�����l��ݒ�
		if (!fin) { return; }
		//�t�@�C�����������琔�l����
		fin >> this->gameTime;
		fin >> this->playerHPMax;
		fin >> this->playerStartPos.x;
		fin >> this->playerStartPos.y;
		fin.close();
}

//----------------------------------------------------------------------------------
//��`�̒��S�����_��
ML::Box2D setCenterPoint(int w_, int h_)
{
	return ML::Box2D(w_ / -2, h_ / -2, w_, h_);
}
