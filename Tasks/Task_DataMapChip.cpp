//-------------------------------------------------------------------
//マップデータ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_DataMapChip.h"
#include  "Task_Map2D.h"

namespace  DataMapChip
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = ge->qa_Map->img;
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		for (int y = 0; y < 100; ++y) {
			for (int x = 0; x < 100; ++x) {
				this->arr[y][x] = 0;
			}
		}

		this->Load();
		//★タスクの生成

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
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}
		//カーソルがまだ無い時も処理しない
		if (nullptr == ge->qa_Cursor) {
			return;
		}

		//マップチップの設置処理
		if (ge->qa_Cursor->cursorMode == CursorMode::MAP) {
			//左クリック長押し中は設置
			if (ge->qa_Cursor->CheckLeftState(BuState::ON)) {
				if (false == ge->qa_Cursor->movingObject) {
					if (true == ge->qa_Cursor->inGameMap) {
						int gridPosX = (int)ge->qa_Cursor->pos.x / 32;
						int gridPosY = (int)ge->qa_Cursor->pos.y / 32;
						this->arr[gridPosY][gridPosX] = ge->qa_IconMapchip->mapchipMode;
						this->ApplyToMap();
					}
				}
			}

			//右クリック長押し中は削除
			else if (ge->qa_Cursor->CheckRightState(BuState::ON)) {
				if (true == ge->qa_Cursor->inGameMap) {
					int gridPosX = (int)ge->qa_Cursor->pos.x / 32;
					int gridPosY = (int)ge->qa_Cursor->pos.y / 32;
					this->arr[gridPosY][gridPosX] = 0;
					this->ApplyToMap();
				}
			}
		}
		

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

	}


	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------

	//設置したチップをマップに反映
	void Object::ApplyToMap()
	{
		//読み込み範囲をカメラの映る範囲内だけにする
		ML::Rect loadArea;
		loadArea.left = ge->camera2D.x / 32;
		if (loadArea.left != 0) {
			--loadArea.left;		//カメラの1マス外も読み込む
		}
		loadArea.top = ge->camera2D.y / 32;
		if (loadArea.top != 0) {
			--loadArea.top;
		}
		loadArea.right = (ge->camera2D.x + ge->camera2D.w) / 32;
		if (loadArea.right > 100 - 1) {
			loadArea.right = 100 - 1;
		}
		loadArea.bottom = (ge->camera2D.y + ge->camera2D.h) / 32;
		if (loadArea.bottom > 100 - 1) {
			loadArea.bottom = 100 - 1;
		}
		
		//設置・結合処理
		for (int y = loadArea.top; y <= loadArea.bottom; ++y) {
			for (int x = loadArea.left; x <= loadArea.right; ++x) {
				int find = this->arr[y][x];
				int arrNum = 0;

				//くっついている部分を探す（上下左右）
				if (y - 1 >= 0 && this->arr[y - 1][x] == find) {
					arrNum += 1;
				}
				if (x + 1 <= 100 - 1 && this->arr[y][x + 1] == find) {
					arrNum += 2;
				}
				if (y + 1 <= 100 - 1 && this->arr[y + 1][x] == find) {
					arrNum += 4;
				}
				if (x - 1 >= 0 && this->arr[y][x - 1] == find) {
					arrNum += 8;
				}

				//マップに反映
				ge->qa_Map->arr[y][x] = ge->qa_Map->chipArr[find][arrNum];
			}
		}

	}

	//メーカー用マップデータを読み込む
	void Object::Load()
	{
		ifstream fin(ge->qa_MapMaker->folderPath + "OriginalMap_MakerData.txt");
		if (!fin) { return; }
		//マップのサイズを読み込む
		int mapSizeW;
		fin >> mapSizeW;
		int mapSizeH;
		fin >> mapSizeH;
		//1マスずつデータを読み込む
		for (int y = 0; y < mapSizeH; ++y) {
			for (int x = 0; x < mapSizeW; ++x) {
				fin >> this->arr[y][x];
			}
		}
		fin.close();
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