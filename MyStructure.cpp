#include "MyPG.h"
#include "MyStructure.h"
#include "BChara.h"
#include "Task_LoadBG.h"

//アニメーション情報の設定
bool  LoadAnim(Anim& anim_, const string& fpath_)
{
	//ファイルを開く（読み込み）
	ifstream fin(fpath_);
	if (!fin) { return false; }//読み込み失敗

	fin >> anim_.imgSizeW;
	fin >> anim_.imgSizeH;
	//アニメーションの順番の読み込み
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


//ロードタスク出現処理
void AppearLoad(int cuts_, int distance_, float speed_, ML::Color color_)
{
	//ロードタスク出現
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

//ロードタスク消滅処理
void DisappearLoad(int distance_, float speed_)
{
	ge->finishedLoadDisappear = false;
	auto load = ge->GetTasks<LoadBG::Object>("タスクロード背景");
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

//チュートリアル時の描画
void DrawSetIfTuto(ML::Box2D& drawBox_, ML::Box2D& srcBox_)
{
	if (ge->inTutorial) {
		ge->qa_Tutorial->SetToScreen(drawBox_, srcBox_);
	}
}

//1以上なら1ずつ減算させる
void KeyInput::DecrementAll() {
	if (LStickBR > 0) { --LStickBR; }
	if (LStickBL > 0) { --LStickBL; }
	if (B3 > 0) { --B3; }
	if (B1 > 0) { --B1; }
	if (RT > 0) { --RT; }
}

void DataForPlay::Load(const string& fpath_) {
		int nameStrCnt = 0;	//ファイル名の文字数
		int ext = 4;	//「.txt」拡張子の文字数(extension)
		//マップ名を抽出
		for (int c = (int)fpath_.size() - ext - 1; fpath_[c] != '/'; --c) {
			++nameStrCnt;
		}
		string mapFilePath = fpath_.substr(0, fpath_.size() - ext - nameStrCnt);			//マップファイルまでのパス
		string mapFileName = fpath_.substr(fpath_.size() - ext - nameStrCnt, nameStrCnt);	//マップファイル名

		//ファイル読み込み
		ifstream fin(mapFilePath + mapFileName + "_" + "DataForPlay.txt");
		//ファイルがなかった場合初期値を設定
		if (!fin) { return; }
		//ファイルがあったら数値を代入
		fin >> this->gameTime;
		fin >> this->playerHPMax;
		fin >> this->playerStartPos.x;
		fin >> this->playerStartPos.y;
		fin.close();
}

//----------------------------------------------------------------------------------
//矩形の中心を原点に
ML::Box2D setCenterPoint(int w_, int h_)
{
	return ML::Box2D(w_ / -2, h_ / -2, w_, h_);
}
