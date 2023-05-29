//-------------------------------------------------------------------
//二次元配列マップ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"
#include  "Task_Enemy.h"

namespace  Map2D
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
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
		this->render2D_Priority[1] = 0.9f;
		//マップのゼロクリア
		for (int y = 0; y < 100; ++y) {
			for (int x = 0; x < 100; ++x) {
				this->arr[y][x] = 0;
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);
		this->drawBase = ML::Box2D(0, 0, 0, 0);
		//マップチップ情報の初期化
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		this->img.reset();
		delete[] this->chipArr;

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//カメラが完全にマップ外を指しているか調べる
		if (false == this->hitBase.Hit(ge->camera2D)) {
			return;	//完全に外に出ていたらその時点で描画処理を取りやめる
		}
		//カメラとマップが重なっているだけの矩形を作る
		//カメラ
		ML::Rect c = {
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h
		};
		//マップ
		ML::Rect m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h
		};

		//２つの矩形が重なっている範囲だけの矩形を求める
		ML::Rect isr;
		isr.left = max(c.left, m.left);
		isr.top = max(c.top, m.top);
		isr.right = min(c.right, m.right);
		isr.bottom = min(c.bottom, m.bottom);

		//ループ範囲を決定
		int sx, sy, ex, ey;
		sx = isr.left / this->length;
		sy = isr.top / this->length;
		ex = (isr.right - 1) / this->length;
		ey = (isr.bottom - 1) / this->length;

		//背景の描画
		for (int i = 0; i < this->backGrounds; ++i) {
			ML::Box2D draw(0, 0, this->backGround[i].imgSizeW, this->backGround[i].imgSizeH);
			ML::Box2D src(0, 0, this->backGround[i].imgSizeW, this->backGround[i].imgSizeH);

			//スクロール対応
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			//チュートリアル時は描画を変更
			DrawSetIfTuto(draw,src);
			this->backGround[i].img->Draw(draw, src);
		}

		//画面内の範囲だけ描画
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				ML::Box2D draw = this->drawBase;
				draw.Offset(x * this->drawBase.w, y * this->drawBase.h);//表示位置を調整
				ML::Box2D src = this->chipData[this->arr[y][x]].srcBox;
				//スクロール対応
				draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
				//チュートリアル時は描画を変更
				DrawSetIfTuto(draw,src);
				this->img->Draw(draw, src);
			}
		}
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//カメラをマップ端から超えさせないように
	void Object::AdjustCameraPos()
	{
		//カメラとマップの範囲を用意
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

		//カメラの位置を調整
		if (c.right > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top < m.top) { ge->camera2D.y = m.top; }

		//マップがカメラより小さい場合
		if (this->hitBase.w < ge->camera2D.w) { ge->camera2D.x = m.left; }
		if (this->hitBase.h < ge->camera2D.h) { ge->camera2D.y = m.top; }

		//マップが最小ならカメラのyは固定させる
		int mapSizeMinY = ge->camera2D.h / ge->qa_Map->length;
		if (ge->camera2D.h % ge->qa_Map->length != 0) {
			++mapSizeMinY;
		}
		if (this->sizeY <= mapSizeMinY) {
			ge->camera2D.y = m.top;
		}

	}

	//指定した矩形とマップとの当たり判定
	bool Object::CheckHit(const ML::Box2D& hit_)
	{
		//矩形
		ML::Rect r = {
			hit_.x,
			hit_.y,
			hit_.x + hit_.w,
			hit_.y + hit_.h
		};
		//マップ
		ML::Rect m = {
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h
		};
		//各種判定を初期化
		bool foundGround = false;
		this->foundSpike = false;
		this->foundGoal = false;


		//矩形がマップ外に出ていたらサイズを変更する
		//左右なら壁判定を付ける
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

		//ループ範囲調整（判定したいものの座標をマップチップ単位の値にする）
		int sx, sy, ex, ey;
		sx = r.left / 32;
		sy = r.top / 32;
		ex = (r.right - 1) / 32;
		ey = (r.bottom - 1) / 32;

		//範囲内の障害物を探す
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				//地面
				if (this->chipData[this->arr[y][x]].chipType == ChipType::GROUND) {
					foundGround = true;
				}
				//針
				else if (this->chipData[this->arr[y][x]].chipType == ChipType::SPIKE) {
					this->foundSpike = true;
					foundGround = true;
				}
				//ゴール
				else if (this->chipData[this->arr[y][x]].chipType == ChipType::GOAL) {
					this->foundGoal = true;
				}
			}
		}
		return foundGround;
	}

	//マップや敵のロード
	bool Object::Load(const string& fpath_)
	{
		//ファイルを開く（読み込み）
		ifstream fin(fpath_);
		if (!fin) { return false; }//読み込み失敗

		//背景の読み込み
		//背景画像の数
		fin >> this->backGrounds;
		//数ぶん読み込み
		for (int i = 0; i < this->backGrounds; ++i)
		{
			string bgFileName, bgFilePath;
			fin >> bgFileName;
			bgFilePath = "./data/image/Map/" + bgFileName;
			this->backGround[i].img = DG::Image::Create(bgFilePath);
			fin >> this->backGround[i].imgSizeW;
			fin >> this->backGround[i].imgSizeH;
		}

		//チップファイル名の読み込みと、画像のロード
		string  chipFileName, chipFilePath;
		fin >> chipFileName;
		chipFilePath = "./data/map/" + chipFileName;
		this->LoadChipData(chipFilePath);

		//敵の読み込み
		int enemyCnt;
		fin >> enemyCnt;
		//敵が0でなければ行う
		if (enemyCnt != 0) {
			int nameStrCnt = 0;	//ファイル名の文字数
			int ext = 4;	//「.txt」拡張子の文字数(extension)
			//マップ名を抽出
			for (int c = (int)fpath_.size() - ext - 1; fpath_[c] != '/'; --c) {
				++nameStrCnt;
			}
			string mapFilePath = fpath_.substr(0, fpath_.size() - ext - nameStrCnt);			//マップファイルまでのパス
			string mapFileName = fpath_.substr(fpath_.size() - ext - nameStrCnt, nameStrCnt);	//マップファイル名

			//敵を1種類ずつ読み込み
			for (int i = 1; i <= enemyCnt; ++i) {
				string enemyFileName;
				fin >> enemyFileName;
				this->LoadEnemy(mapFilePath + mapFileName + "_" + enemyFileName);
			}
		}
		//マップ配列サイズの読み込み
		fin >> this->sizeX >> this->sizeY;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * this->length, this->sizeY * this->length);

		//マップ配列データの読み込み
		for (int y = 0; y < this->sizeY; ++y) {
			for (int x = 0; x < this->sizeX; ++x) {
				fin >> this->arr[y][x];
			}
		}
		fin.close();

		return true;
	}

	//チップデータをファイルから取得
	bool Object::LoadChipData(const string& fpath_)
	{
		//ファイルを開く（読み込み）
		ifstream fin(fpath_);
		if (!fin) { return false; }//読み込み失敗
		//列の数
		fin >> this->columns;

		//画像読み込み
		string fileNameImg;
		fin >> fileNameImg;
		string imgPath = "./data/image/Map/" + fileNameImg;
		this->img = DG::Image::Create(imgPath);

		//画像の配列データ読み込み
		string fileNameArr;
		fin >> fileNameArr;
		LoadMapChipArray("./data/mapChip/" + fileNameArr);

		//画像サイズ縦
		fin >> this->imgSizeH;

		//画像サイズ横
		fin >> this->imgSizeW;

		//チップ数
		fin >> this->tiles;

		//一辺の長さ
		fin >> this->length;

		//描画用の矩形を設定
		this->drawBase = ML::Box2D(0, 0, this->length, this->length);

		//タイルごとに情報を読み込む
		for (int i = 0; i < tiles; ++i) {
			int x = (i % columns) * 32;
			int y = (i / columns) * 32;
			this->chipData[i].srcBox = ML::Box2D(x, y, this->length, this->length);

			string type;
			//タイプ情報読み込み
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
	//チップの結合用配列ロード
	bool  Object::LoadMapChipArray(const string& fpath_)
	{
		ifstream fin(fpath_);
		if (!fin) { return false; }//読み込み失敗
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

	//敵の情報を読み込む
	bool Object::LoadEnemy(const string& efpath_)
	{
		ifstream fin(efpath_);
		if (!fin) { return false; }
		int enemyType;		//敵のタイプ
		int enemyCnt;		//敵の数
		fin >> enemyType;
		fin >> enemyCnt;
		for (int e = 0; e < enemyCnt; ++e) {
			ML::Vec2 enemyPos;	//敵の初期位置
			fin >> enemyPos.x;
			fin >> enemyPos.y;
			//敵出現処理
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

	//マップサイズ変更
	void Object::ChangeMapSize(int sizeX_, int sizeY_)
	{
		this->sizeX = sizeX_;
		this->sizeY = sizeY_;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * this->length, this->sizeY * this->length);
	}


	//チップごとのエフェクト
	void Object::chipEffect() {
		
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