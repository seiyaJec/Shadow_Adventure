//-------------------------------------------------------------------
//マップ制作で使用するUI
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MakeMapUI.h"
#include  "Task_Grid.h"
#include  "Task_IconEnemy.h"
#include  "Task_IconMapchip.h"
#include  "Task_DataPlayer.h"
#include  "Task_SelectingIcon.h"
#include  "Task_HideMakeMapUIButton.h"
#include  "Task_MakerSetTimeDisplay.h"
#include  "Task_MakerSetTimeButton.h"
#include  "Task_MakerSetPlayerHPButton.h"
#include  "Task_MakerSetPlayerHPDisplay.h"

namespace MakeMapUI
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->UIbarImg = DG::Image::Create("./data/image/MapMaker/UIbar.png");
		this->cantPutColor = DG::Image::Create("./data/image/MapMaker/cantPut.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->UIbarImg.reset();
		this->GridImg.reset();
		this->cantPutColor.reset();

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
		ge->qa_MakeMapUI = ge->GetTask<MakeMapUI::Object>("マップ制作UI", "NoName");
		//画像優先度
		this->render2D_Priority[1] = 0.3f;
		//マウス入力
		this->mouse = ge->mouse;
		//画像サイズ
		this->SetImgSize(960, 80);
		//矩形
		this->hitBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		//位置情報
		this->pos.x = (float)(this->imgSizeW / 2);
		this->pos.y = (float)(ge->screen2DHeight - this->imgSizeH / 2);
		//非表示関係
		this->hideFlag = false;
		this->hideMovePosY = 0;
		this->hideMoveSpeed = 10;
		//★タスクの生成
		//敵アイコン
		IconEnemy::Object::Create(true);
		//マップチップアイコン
		IconMapchip::Object::Create(true);
		//プレイヤースタート地点
		DataPlayer::Object::Create(true);
		//グリッド
		Grid::Object::Create(true);
		//選択中
		SelectingIcon::Object::Create(true);
		//UIバー非表示ボタン
		HideMakeMapUIButton::Object::Create(true);

		//制限時間設定タスク生成
		int numberPlace = 1;//生成するタスクの位
		for (int i = 0; i < 3; ++i) {
			//数字表示用ディスプレイ
			auto display = MakerSetTimeDisplay::Object::Create(true);
			display->addPosX = 382 - (i * 29);
			display->numberPlace = numberPlace;
			display->myNumber = ge->qa_MapMaker->dataForPlay.gameTime / numberPlace % 10;
			//数字設定ボタン
			auto buttonTop = MakerSetTimeButton::Object::Create(true);
			buttonTop->linkingDisplay = display;
			buttonTop->buttonMode = buttonTop->ButtonMode::TOP;
			auto buttonBottom = MakerSetTimeButton::Object::Create(true);
			buttonBottom->linkingDisplay = display;
			buttonBottom->buttonMode = buttonBottom->ButtonMode::BOTTOM;

			numberPlace *= 10;
		}

		//HP設定タスク生成
		numberPlace = 1;
		//数字表示用ディスプレイ
		auto display = MakerSetPlayerHPDisplay::Object::Create(true);
		display->addPosX = -275;
		display->numberPlace = numberPlace;
		display->myNumber = ge->qa_MapMaker->dataForPlay.playerHPMax / 10;
		//数字設定ボタン
		auto buttonTop = MakerSetPlayerHPButton::Object::Create(true);
		buttonTop->linkingDisplay = display;
		buttonTop->buttonMode = buttonTop->ButtonMode::TOP;
		auto buttonBottom = MakerSetPlayerHPButton::Object::Create(true);
		buttonBottom->linkingDisplay = display;
		buttonBottom->buttonMode = buttonBottom->ButtonMode::BOTTOM;

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
		//マウスの情報取得
		auto ms = this->mouse->GetState();
		//位置設定
		this->pos.x = (float)(this->imgSizeW / 2);
		this->pos.y = (float)(ge->screen2DHeight - this->imgSizeH / 2);

		//表示・非表示処理
		//ボタンが押されたら
		if (true == this->hideFlag) {
			//完全に消えるまで一定速度で移動
			if (this->hideMovePosY < this->imgSizeH) {
				this->hideMovePosY += this->hideMoveSpeed;
				//下に行き過ぎないようにする処理
				if (this->hideMovePosY >= this->imgSizeH) {
					this->hideMovePosY = (float)this->imgSizeH;
				}
			}
		}
		//非表示中にボタンが押されたら
		else {
			//元に戻るまで一定速度で移動
			if (this->hideMovePosY > 0) {
				this->hideMovePosY -= this->hideMoveSpeed;
				//ゼロを下回らないようにする処理
				if (this->hideMovePosY <= 0) {
					this->hideMovePosY = 0;
				}
			}
		}
		//非表示アニメーション
		this->pos.y += this->hideMovePosY;

		//カメラがを原点に
		this->pos.x += ge->camera2D.x;
		this->pos.y += ge->camera2D.y;

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//ゲームモード中は処理しない
		if (ge->qa_MapMaker->gamemode != GameMode::Maker) {
			return;
		}

		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);

			ML::Box2D src(0, 0, imgSizeW, imgSizeH);
			this->res->UIbarImg->Draw(draw, src);
		}


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