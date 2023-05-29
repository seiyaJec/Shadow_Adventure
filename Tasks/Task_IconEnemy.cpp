//-------------------------------------------------------------------
//敵選択アイコン
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_IconEnemy.h"
#include  "Task_DataEnemy.h"
#include  "Task_MakeMapUI.h"
#include  "Task_MapMaker.h"

namespace  IconEnemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img00 = DG::Image::Create("./data/image/Enemy/Enemy00R.png");
		this->img01 = DG::Image::Create("./data/image/Enemy/Enemy01R.png");
		this->imgNow = nullptr;	//現在選択している画像
		this->cantPutColor = ge->qa_MakeMapUI->res->cantPutColor;

		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img00.reset();
		this->img01.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		//読み込みを可能にさせる
		ge->qa_IconEnemy = ge->GetTask<IconEnemy::Object>("アイコン", "敵");
		//画像優先度
		this->render2D_Priority[1] = 0.25f;
		//画像
		this->SetImgSize(48, 48);
		//矩形
		this->hitBase = setCenterPoint(44, 36);
		this->hitBase.Offset(0, 6);
		this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
		this->srcBase = ML::Box2D(0, 0, imgSizeW, imgSizeH);
		//現在の敵の種類
		this->enemyType = 0;
		//★タスクの生成
		this->Load();

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->qa_MapMaker->controller->GetState();
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		this->pos = ge->qa_MakeMapUI->pos;
		this->pos += ML::Vec2(-50, 0);

		//B3でそれぞれタイプ切り替え
		if (true == inp.B3.down) {
			switch (ge->qa_Cursor->cursorMode) {
			case CursorMode::ENEMY:
				++ge->qa_IconEnemy->enemyType;
				if (ge->qa_IconEnemy->enemyType == ge->qa_IconEnemy->enemyTypeSize) {
					ge->qa_IconEnemy->enemyType = 0;
				}
				break;
			}
		}

		//クリックされたらモード変更
		if (ge->qa_Cursor->Hit(this->hitBase.OffsetCopy(this->pos))) {
			if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
				ge->qa_Cursor->cursorMode = CursorMode::ENEMY;
			}
		}

		//タイプごとに違った画像・サイズを用意
		this->SetImageByType();

		//サイズの変更
		this->InCursorSizeUp();

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

		if (nullptr != this->res->imgNow) {
			this->Draw(this->res->imgNow);
		}

	}

	//-------------------------------------------------------------------
	// 関数定義
	//-------------------------------------------------------------------
	//敵読み込み
	void Object::Load()
	{
		ge->KillAll_G("敵");
		for (int i = 0; i < this->enemyTypeSize; ++i) {
			char num[3];
			sprintf_s(num, "%02d", i);
			string enemyTypeNum = num;
			//ファイルがない場合は読み込まない
			ifstream fin(ge->qa_MapMaker->folderPath + "OriginalMap_Enemy" + enemyTypeNum + "Data.txt");
			if (!fin) { continue; }
			int enemyCnt;
			fin >> this->enemyType;
			this->SetImageByType();
			fin >> enemyCnt;
			for (int e = 0; e < enemyCnt; ++e) {
				ML::Vec2 enemyPos;
				auto enemy = Data_Enemy::Object::Create(true);
				fin >> enemy->pos.x;
				fin >> enemy->pos.y;
				enemy->puted = true;
			}
			fin.close();
		}
	}
	//生成
	void Object::Generate()
	{
		auto enemy = Data_Enemy::Object::Create(true);
		enemy->pos = ge->qa_Cursor->pos;
	}

	//タイプごとに画像を変更
	void Object::SetImageByType() {
		switch (enemyType) {
		case 0:
			this->ChangeImage(this->res->img00, 48, 48);
			this->hitBase = setCenterPoint(44, 36);
			this->hitBase.Offset(0, 6);
			break;
		case 1:
			this->ChangeImage(this->res->img01, 80, 80);
			this->hitBase = setCenterPoint(44, 36);
			this->hitBase.Offset(0, 6);
			break;
		default:
			break;
		}
	}

	//画像変更
	void Object::ChangeImage(DG::Image::SP img_,int imgW_, int imgH_)
	{
		this->res->imgNow = img_;
		this->SetImgSize(imgW_, imgH_);
		this->drawBase = setCenterPoint(imgSizeW, imgSizeH);
		this->srcBase = ML::Box2D(0, 0, imgSizeW, imgSizeH);
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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