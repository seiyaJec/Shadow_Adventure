//-------------------------------------------------------------------
//キーボード
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_KeyBoard.h"
#include  "Task_Key.h"
#include  "Task_Cursor.h"
#include  "Task_KeyBoardDisplay.h"
#include  "Task_KeyBoardEnter.h"
#include  "Task_KeyBoardChange.h"
#include  "Task_KeyBoardSpace.h"
#include  "Task_KeyBoardBackSpace.h"
#include  "Task_KeyBoardCancel.h"
#include  "Task_KeyBoardMessage.h"

namespace  KeyBoard
{

	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/MapMaker/KeyBoard.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		//参照用
		ge->qa_KeyBoard = ge->GetTask<KeyBoard::Object>("キーボード", "メイン");
		//画像優先度
		this->render2D_Priority[1] = 0.29f;
		//画像サイズ
		this->SetImgSize(960, 540);
		//画像矩形
		this->drawBase = setCenterPoint(this->imgSizeW,this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//出現スピード
		this->moveSpeed = 20;
		//位置情報
		this->pos = ML::Vec2(this->imgSizeW / 2.0f, this->imgSizeH / 2.0f);
		//入力情報
		this->userInput = "";
		//メッセージ
		this->messageStr = "";
		//入力上限
		this->userInputLimit = 20;
		//フラグ
		this->finishInputFlag = false;
		this->changeLangFlag = false;
		this->englishKeyFlag = false;
		this->disappearFlag = false;
		//★タスクの生成
		//カーソル
		if (nullptr == ge->qa_Cursor) {
			Cursor::Object::Create(true);
			ge->qa_Cursor = ge->GetTask<Cursor::Object>("カーソル");
		}
		ge->qa_Cursor->cursorMode = CursorMode::FREE;
		//ディスプレイ
		KeyBoardDisplay::Object::Create(true);
		//文字キー
		this->GenerateButtonJP();
		//エンターキー
		KeyBoardEnter::Object::Create(true);
		//言語変換キー
		KeyBoardChange::Object::Create(true);
		//スペースキー
		KeyBoardSpace::Object::Create(true);
		//バックスペースキー
		KeyBoardBackSpace::Object::Create(true);
		//キャンセルボタン
		KeyBoardCancel::Object::Create(true);
		//メッセージウィンドウ
		KeyBoardMessage::Object::Create(true);
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
		ge->qa_Cursor = ge->GetTask<Cursor::Object>("カーソル");
		ge->qa_KeyBoard = ge->GetTask<KeyBoard::Object>("キーボード", "メイン");
		//言語変換処理
		if(false == this->disappearFlag) {
			if (true == this->changeLangFlag) {
				ge->KillAll_GN("キーボード", "キー");
				if (false == this->englishKeyFlag) {
					this->GenerateButtonEN();
				}
				else {
					this->GenerateButtonJP();
				}
				this->englishKeyFlag = !this->englishKeyFlag;
				this->changeLangFlag = false;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase;
		this->res->img->Draw(draw, src);
	}
	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//日本語
	void Object::GenerateButtonJP()
	{
		float keyDistanceW = 16;	//キーの間隔横
		float keyDistanceH = 20;	//キーの間隔縦
		//キーを一つずつ生成
		for (int i = 0; i < 75; ++i) {
			auto key = Key::Object::Create(true);
			key->pos = ML::Vec2(90, 150);
			key->fontDrawBase = setCenterPoint(36, 36);
			key->mychar = this->res->HiraganaArr[i];
			int keyPosX = i / 5;
			int keyPosY = i % 5;
			key->pos.x += keyPosX * (key->imgSizeW + keyDistanceW);
			key->pos.y += keyPosY * (key->imgSizeH + keyDistanceH);
		}
	}
	//英語
	void Object::GenerateButtonEN()
	{
		float keyDistanceW = 16;	//キー横幅
		float keyDistanceH = 20;	//キー縦幅
		int keyImageSize = 40;		//キー画像サイズ
		ML::Vec2 keyPos = ML::Vec2(110, 150);
		//数字
		for (char c = '0'; c <= '9'; ++c) {
			this->CreateKeyHalfSize(c, keyPos);
			keyPos.x += keyImageSize + keyDistanceW;
		}

		//記号
		this->CreateKeyHalfSize(':', keyPos);
		keyPos.x += keyImageSize + keyDistanceW;
		this->CreateKeyHalfSize('_', keyPos);
		keyPos.x += keyImageSize + keyDistanceW;
		this->CreateKeyHalfSize('/', keyPos);
		keyPos.x += keyImageSize + keyDistanceW;
		this->CreateKeyHalfSize('.', keyPos);

		//小文字アルファベット
		keyPos.x = 130;
		keyPos.y += keyImageSize + keyDistanceH;
		for (char c = 'a'; c <= 'z'; ++c) {
			this->CreateKeyHalfSize(c, keyPos);
			keyPos.x += keyImageSize + keyDistanceW;
			if (c == 'm') {
				keyPos.x = 130;
				keyPos.y += keyImageSize + keyDistanceH;
			}
		}

		//大文字アルファベット
		keyPos.x = 130;
		keyPos.y += keyImageSize + keyDistanceH;
		for (char c = 'A'; c <= 'Z'; ++c) {
			this->CreateKeyHalfSize(c, keyPos);
			keyPos.x += keyImageSize + keyDistanceW;
			if (c == 'M') {
				keyPos.x = 130;
				keyPos.y += keyImageSize + keyDistanceH;
			}
		}


	}

	//半角のキーを作るときの関数
	void Object::CreateKeyHalfSize(char c_, ML::Vec2 pos_)
	{
		auto key = Key::Object::Create(true);
		key->fontDrawBase = setCenterPoint(18, 36);
		char cStr[3] = { c_, '\0', '\0' };
		key->mychar = cStr;
		key->pos = pos_;
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