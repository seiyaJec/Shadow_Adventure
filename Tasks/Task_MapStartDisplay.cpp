//-------------------------------------------------------------------
//マップのスタート地点を表示
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapStartDisplay.h"

namespace  MapStartDisplay
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

		//画像の用意
		for (int i = 0; i < 15; ++i) {
			//名前があれば、登録処理をする
			if (ge->qa_MapSelect->mapData[i].imageFileName != "") {

				bool foundSameFile = false;

				//画像名配列への登録処理
				for (int j = 0; j < 15; ++j) {
					//画像名配列内に同じ名前があったら処理中止
					if (ge->qa_MapSelect->mapData[i].imageFileName == this->imgFileArr[j]) {
						foundSameFile = true;
						break;
					}
					//未登録の添字まで来たら登録処理・処理終了
					if (this->imgFileArr[j] == "") {
						this->imgFileArr[j] = ge->qa_MapSelect->mapData[i].imageFileName;
						this->img[j] = DG::Image::Create("./data/image/Map/" + imgFileArr[j]);
						break;
					}
				}
			}
		}
		
		//画像優先度
		this->render2D_Priority[1] = 0.49f;
		//サイズ
		this->SetImgSize(800, 800);
		this->displaySizeH = 11;
		this->displaySizeW = 14;
		this->chipLength = 32;
		//位置情報
		this->pos = ML::Vec2(440, 32);			//このタスクでは左上座標
		//★タスクの生成
		
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		for (int i = 0; i < 100; ++i) {
			this->img[i].reset();
		}

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
		int displayPos = (ge->qa_MapSelect->page * 3) + ge->qa_MapSelect->selectingPos;//現在見ているファイル番号

		//ファイル内に画像名があるか確認
		int imgFileNum = this->FindImageFile(displayPos);

		//画像が見つかったら描画
		if (imgFileNum != -1) {
			//1マスずつ描画
			for (int y = 0; y < this->displaySizeH; ++y) {
				for (int x = 0; x < this->displaySizeW; ++x) {
					int chipNum = ge->qa_MapSelect->mapData[displayPos].startPointMapData[y][x];
					//描画矩形
					ML::Box2D draw(
						x * this->chipLength,
						y * this->chipLength,
						this->chipLength,
						this->chipLength);

					draw.Offset(this->pos);

					//読み込み元矩形
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
	//関数定義
	//-------------------------------------------------------------------
	//記憶中の画像名に同じ画像があるか探す
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