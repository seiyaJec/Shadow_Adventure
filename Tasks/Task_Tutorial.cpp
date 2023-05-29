//-------------------------------------------------------------------
//チュートリアル
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorial.h"
#include  "Task_TutorialPlayer.h"
#include  "Task_TutorialCategory.h"
#include  "Task_Key.h"
#include  "Task_Effect.h"

namespace  Tutorial
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Tutorial/BackGround.png");
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
		ge->qa_Tutorial = ge->GetTask<Tutorial::Object>("チュートリアル");
		ge->inTutorial = true;
		//リトライフラグ記憶・リセット
		this->retryFlagMemory = ge->retryFlag;
		ge->retryFlag = false;
		//ゲームオーバーフラグ記憶・リセット
		this->gameOverFlagMemory = ge->gameOverflag;
		ge->gameOverflag = false;


		//画像優先度
		this->render2D_Priority[1] = 0.99f;
		//時間計測
		this->timeCnt = 0;
		//項目
		this->categories = 8;
		this->selectingCategories = 0;
		this->categoryDistance = 10;
		//入力
		this->inputDataSize = 0;

		//入力情報の読み込み
		this->LoadInputData();
		this->inputNow = this->inputData[0].input;
		this->waitNextInput = this->inputData[0].distanceOfNextInput;

		//項目名の読み込み
		this->LoadCategoryName();

		//ゲーム情報の読み込み
		this->dataForPlay.Load(this->FolderPath() + "tutorialMap.txt");

		//カメラの設定
		ge->camera2D = ML::Box2D(
			0,
			0,
			ge->screen2DWidth,
			ge->screen2DHeight
		);

		this->screen = ML::Box2D(450, 100, 480, 270);
		this->screenScaleRateW = (float)this->screen.w / ge->camera2D.w;
		this->screenScaleRateH = (float)this->screen.h / ge->camera2D.h;
		
		//★タスクの生成
		//マップ・敵の記憶・生成
		this->qaMapMemory = ge->qa_Map;
		this->qaEnemyMemory = ge->qa_Enemys;
		auto map = Map2D::Object::Create(true);
		ge->qa_Map = map;
		map->Load(this->FolderPath() + "tutorialMap.txt");
		//プレイヤの生成
		this->qaPlayerMemory = ge->qa_Player;
		this->player = Player::Object::Create(true);
		this->player->pos = this->dataForPlay.playerStartPos;
		//チュートリアル用見本の生成
		this->tutoPlayer = TutorialPlayer::Object::Create(true);
		this->tutoPlayer->pos = this->dataForPlay.playerStartPos;
		//キーの生成
		this->CreateKey(this->keyB1, 0, 570,440);
		this->CreateKey(this->keyB3, 2, 545, 410);
		this->CreateKey(this->keyA, 5, 720, 420);
		this->CreateKey(this->keyD, 7, 790, 420);
		this->CreateKey(this->keyM, 8, 870, 440);
		this->CreateKey(this->keyL, 9, 905, 420);

		//シフトキー
		this->keyShift = TutorialKeyDisplay::Object::Create(true);
		this->keyShift->useShift = true;
		this->keyShift->SetImgSize(64, 32);
		this->keyShift->pos = ML::Vec2(736, 454);
		this->keyShift->drawBase = setCenterPoint(this->keyShift->imgSizeW, this->keyShift->imgSizeH);
		this->keyShift->srcBase = ML::Box2D(0, 0, this->keyShift->imgSizeW, this->keyShift->imgSizeH);
		//RTキー
		this->keyRT = TutorialKeyDisplay::Object::Create(true);
		this->keyRT->useRT = true;
		this->keyRT->pos = ML::Vec2(610, 415);
		this->keyRT->SetImgSize(32, 64);
		this->keyRT->drawBase = setCenterPoint(this->keyRT->imgSizeW, this->keyRT->imgSizeH);
		this->keyRT->srcBase = ML::Box2D(0, 0, this->keyRT->imgSizeW, this->keyRT->imgSizeH);
		//一括設定用を設定
		this->keys = ge->GetTasks<TutorialKeyDisplay::Object>("チュートリアル", "入力表示");
		
		
		//スティック
		this->keyStick = TutorialKeyStickDisplay::Object::Create(true);
		this->keyStick->pos = ML::Vec2(480, 430);

		//項目の表示
		for (int i = 0; i < this->categories; ++i) {
			auto category = TutorialCategory::Object::Create(true);
			category->pos = ML::Vec2(225, 50);
			category->pos.y += (float)(this->categoryDistance + category->imgSizeH) * i;
			category->fontObj->pos = category->pos;
			category->fontObj->SetString(this->categoryName[i]);
			category->orderNum = i;
		}


		//切り替えエフェクト表示
		this->AppearSwitchEffect();

		//ここでポーズ画面の画像優先度をダウン！元のゲーム画面は非表示！



		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete[] this->inputData;
		delete[] this->categoryName;

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		ge->qa_Enemys = ge->GetTasks<BChara>("敵");

		//リトライ処理
		if (true == ge->retryFlag) {
			this->player->Kill();
			auto shadow = ge->GetTask<BTask>("ゲームオーバーシャドウ");
			if (nullptr != shadow) {
				shadow->Kill();
			}
			ge->gameOverflag = false;
			this->player = Player::Object::Create(true);
			this->player->SetRetryPos(this->dataForPlay.playerStartPos);
			this->player->retryAppear = true;
			ge->retryFlag = false;
		}

		//B4で見本リセット
		if (inp.B4.down) {
			this->ResetPlayer();
		}

		//L1,R1で項目切り替え
		if (inp.L1.down) {
			if (this->selectingCategories > 0) {
				--this->selectingCategories;
				this->ResetScreen();
			}
		}
		if (inp.R1.down) {
			if (this->selectingCategories < this->categories - 1) {
				++this->selectingCategories;
				this->ResetScreen();
			}
		}

		//B2でチュートリアル終了
		if (inp.B2.down) {
			this->finishTutorial = true;
			AppearLoad(10, 3, 20,ML::Color(1.0f,0.5f,0.5f,0.5f));
			return;
		}


		//入力情報を各タスクに反映
		this->reflectInput(this->inputNow,inp);
		
		//指定時間に達したら次の入力へ
		if (this->waitNextInput <= 0 && this->inputCnt < this->inputDataSize - 1) {
			++this->inputCnt;
			this->inputNow += inputData[inputCnt].input;
			this->waitNextInput = inputData[inputCnt].distanceOfNextInput;
		}

		--this->waitNextInput;
		++this->timeCnt;

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 960, 540);
		ML::Box2D src(0, 0, 960, 540);
		this->res->img->Draw(draw, src);
	}

	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//スクリーン用の座標・サイズに設定
	void Object::SetToScreen(ML::Box2D& drawBox_, ML::Box2D& srcBox_)
	{
		//スクリーンの位置へ移動
		drawBox_.x = (int)(drawBox_.x * this->screenScaleRateW);
		drawBox_.y = (int)(drawBox_.y * this->screenScaleRateH);
		drawBox_.Offset(this->screen.x, this->screen.y);
		//スクリーンのサイズに合わせる
		drawBox_.w = (int)(drawBox_.w * this->screenScaleRateW);
		drawBox_.h = (int)(drawBox_.h * this->screenScaleRateH);
		//スクリーン
		ML::Rect s = {
			this->screen.x,
			this->screen.y,
			this->screen.x + this->screen.w,
			this->screen.y + this->screen.h
		};
		//描画
		ML::Rect d = {
			drawBox_.x,
			drawBox_.y,
			drawBox_.x + drawBox_.w,
			drawBox_.y + drawBox_.h
		};

		if (this->screen.Hit(drawBox_)) {
			//画像がスクリーンからはみ出ている場合、
			//出ている部分は描画させない処理
			//左
			if (s.left > d.left) {
				drawBox_.x += s.left - d.left;
				drawBox_.w -= s.left - d.left;
				srcBox_.x += (int)((s.left - d.left) / this->screenScaleRateW);	//はみ出している部分を元のデータの範囲に合わせてから計算
				srcBox_.w -= (int)((s.left - d.left) / this->screenScaleRateW);	
			}
			//上
			if (s.top > d.top) {
				drawBox_.y += s.top - d.top;
				drawBox_.h -= s.top - d.top;
				srcBox_.y += (int)((s.top - d.top) / this->screenScaleRateH);
				srcBox_.h -= (int)((s.top - d.top) / this->screenScaleRateH);
			}
			//右
			if (s.right < d.right) {
				drawBox_.w -= d.right - s.right;
				srcBox_.w -= (int)((d.right - s.right) / this->screenScaleRateW);
			}
			//下
			if (s.bottom < d.bottom) {
				drawBox_.h -= d.bottom - s.bottom;
				srcBox_.h -= (int)((d.bottom - s.bottom) / this->screenScaleRateH);
			}
		}
		//完全にスクリーン外に出ていたらサイズを0に
		else {
			drawBox_.w = 0;
			drawBox_.h = 0;
		}
	}

	//入力情報の読み込み
	void Object::LoadInputData()
	{
		ifstream fin(this->FolderPath() + "InputData.txt");
		if (!fin) { return; }

		//配列を再設定
		delete[] this->inputData;
		fin >> this->inputDataSize;
		this->inputData = new InputData[this->inputDataSize];
		
		//入力情報読み込み
		for (int i = 0; i < this->inputDataSize; ++i) {
			string inp;
			fin >> inp;
			//「DIS」が見つかるまで入力読みこみ
			while (inp != "DIS")
			{
				if (!fin) { 
					this->inputDataSize = i;
					return; 
				}
				this->CheckInput(i, inp, fin);
				fin >> inp;
			}

			fin >> this->inputData[i].distanceOfNextInput;
		}
		fin.close();
	}

	//項目名の読み込み
	void Object::LoadCategoryName()
	{
		this->categoryName = new string[this->categories];
		//現在用意しているすべての項目名を読み込む
		for (int i = 0; i < this->categories; ++i) {
			string fPath = "./data/Tutorial/Category" + to_string(i) + "/";
			ifstream fin(fPath + "Name.txt");
			fin >> categoryName[i];
			fin.close();
		}
	}

	//どのボタンの入力か
	void Object::CheckInput(int cnt_, const string& inp_,ifstream& fin_)
	{
		if (inp_ == "LBR") {
			fin_ >> this->inputData[cnt_].input.LStickBR;
		}
		else if (inp_ == "LBL") {
			fin_ >> this->inputData[cnt_].input.LStickBL;
		}
		else if (inp_ == "B1") {
			fin_ >> this->inputData[cnt_].input.B1;
		}
		else if (inp_ == "B3") {
			fin_ >> this->inputData[cnt_].input.B3;
		}
		else if (inp_ == "RT") {
			fin_ >> this->inputData[cnt_].input.RT;
		}
	}

	//入力情報の反映
	void Object::reflectInput(KeyInput& in_,XI::VGamePad& inp_)
	{
		//フラグリセット
		//プレイヤー
		this->tutoPlayer->moveLRFlag = false;
		this->tutoPlayer->dashFlag = false;
		this->tutoPlayer->attackFlag = false;
		this->tutoPlayer->jumpFlag = false;
		//キー入力表示
		for (auto& key : *this->keys) {
			key->pushing = false;
			key->playerInput = false;
		}
		//スティック入力表示
		this->keyStick->movingStick = this->keyStick->Angle::NON;
		this->keyStick->playerInput = this->keyStick->Angle::NON;


		//データ上にある入力情報ーーーーーーーーー
		//右移動
		if (in_.LStickBL >= 1) {
			this->tutoPlayer->ctrlAng = BChara::Angle_LR::Left;
			this->tutoPlayer->moveLRFlag = true;
			this->keyA->pushing = true;
			this->keyStick->movingStick = this->keyStick->Angle::LEFT;
		}
		//左移動
		if (in_.LStickBR >= 1) {
			this->tutoPlayer->ctrlAng = BChara::Angle_LR::Right;
			this->tutoPlayer->moveLRFlag = true;
			this->keyD->pushing = true;
			this->keyStick->movingStick = this->keyStick->Angle::RIGHT;
		}
		//ダッシュ
		if (in_.B3 >= 1) {
			this->tutoPlayer->dashFlag = true;
			this->keyB3->pushing = true;
			this->keyShift->pushing = true;
		}
		//ジャンプ
		if (in_.B1 >= 1) {
			this->tutoPlayer->jumpFlag = true;
			this->keyB1->pushing = true;
			this->keyM->pushing = true;
		}
		//アタック
		if (in_.RT >= 1) {
			this->tutoPlayer->attackFlag = true;
			this->keyRT->pushing = true;
			this->keyL->pushing = true;
		}

		//プレイヤーの入力情報ーーーーーーーーー
		//右移動
		if (inp_.LStick.BR.on) {
			this->keyD->playerInput = true;
			this->keyStick->playerInput = this->keyStick->Angle::RIGHT;
		}
		if (inp_.LStick.BL.on) {
			this->keyA->playerInput = true;
			this->keyStick->playerInput = this->keyStick->Angle::LEFT;
		}
		//ジャンプ
		if (inp_.B1.on) {
			this->keyB1->playerInput = true;
			this->keyM->playerInput = true;
		}
		//ダッシュ
		if (inp_.B3.on) {
			this->keyB3->playerInput = true;
			this->keyShift->playerInput = true;
		}
		//アタック
		if (inp_.Trigger.volumeR != 0) {
			this->keyRT->playerInput = true;
			this->keyL->playerInput = true;
		}
		//中身の減算処理
		in_.DecrementAll();

	}

	//キーの生成
	void Object::CreateKey(TutorialKeyDisplay::Object::SP& key_, int srcOfsY_, float posX_, float posY_)
	{
		//生成
		key_ = TutorialKeyDisplay::Object::Create(true);
		//描画する画像の変更
		key_->srcBase.y = srcOfsY_ * key_->imgSizeH;
		//位置の設定
		key_->pos.x = posX_;
		key_->pos.y = posY_;
	}

	//項目切り替え時のエフェクト表示
	void Object::AppearSwitchEffect()
	{
		auto effect = Effect05::Object::Create(true);
		effect->Set(480, 270, 100);
		effect->pos = ML::Vec2(690, 235);
	}

	//見本プレイヤーリセット
	void Object::ResetPlayer()
	{
		this->LoadInputData();
		this->inputNow = this->inputData[0].input;
		this->waitNextInput = this->inputData[0].distanceOfNextInput;
		this->inputCnt = 0;

		this->tutoPlayer->Kill();
		this->tutoPlayer = TutorialPlayer::Object::Create(true);
		this->tutoPlayer->pos = this->dataForPlay.playerStartPos;
	}

	//スクリーンごとリセット
	void Object::ResetScreen()
	{
		//エフェクトが残っていたらいったんキル
		ge->KillAll_GN("エフェクト", "05");
		//敵がいる場合もキル
		ge->KillAll_G("敵");
		//入力情報
		this->dataForPlay.Load(this->FolderPath() + "tutorialMap.txt");
		//マップ
		ge->qa_Map->Load(this->FolderPath() + "tutorialMap.txt");
		//見本
		this->ResetPlayer();
		//プレイヤー位置
		this->player->Kill();
		this->player = Player::Object::Create(true);
		this->player->pos = this->dataForPlay.playerStartPos;
		//エフェクト
		this->AppearSwitchEffect();
	}

	//現在選択中の項目へのフォルダパスを返す
	string Object::FolderPath()
	{
		return "./data/Tutorial/Category" + to_string(this->selectingCategories) + "/";
	}

	//チュートリアル終了処理
	void Object::FinishTask()
	{
		//チュートリアルに使用したタスクのキル
		//プレイヤー
		this->player->Kill();
		//敵		現在無効状態ではないもの（チュートリアルで使用していたもの）のみ消す											
		auto enemys = ge->GetTasks<BChara>("敵");				
		if (nullptr != enemys) {
			for (auto& it : *enemys) {
				if (it->TaskStateCnt_Suspend() == 0) {
					it->Kill();
				}
			}
		}
		//マップ
		ge->qa_Map->Kill();
		//エフェクト
		auto effects = ge->GetTasks<BTask>("エフェクト");
		if (nullptr != effects) {
			for (auto& it : *effects) {
				if (it->TaskStateCnt_Suspend() == 0) {
					it->Kill();
				}
			}
		}
		//攻撃
		auto attacks = ge->GetTasks<BChara>("攻撃");
		if (nullptr != attacks) {
			for (auto& it : *attacks) {
				if (it->TaskStateCnt_Suspend() == 0) {
					it->Kill();
				}
			}
		}
		//ゲームUI
		auto uis = ge->GetTasks<BTask>("ゲームUI");
		if (nullptr != uis) {
			for (auto& it : *uis) {
				if (it->TaskStateCnt_Suspend() == 0) {
					it->Kill();
				}
			}
		}
		//ゲームオーバーシャドウ
		auto shadow = ge->GetTask<BTask>("ゲームオーバーシャドウ");
		if (nullptr != shadow) {
			if (shadow->TaskStateCnt_Suspend() == 0) {
				shadow->Kill();
			}
		}

		//参照の状態を元に戻す
		ge->qa_Player = this->qaPlayerMemory;
		ge->qa_Map = this->qaMapMemory;
		ge->qa_Enemys = this->qaEnemyMemory;
		ge->retryFlag = this->retryFlagMemory;
		ge->gameOverflag = this->gameOverFlagMemory;

		//チュートリアルフラグをオフに
		ge->inTutorial = false;

		//チュートリアルのキル
		ge->KillAll_G("チュートリアル");
		ge->KillAll_GN("プレイヤ", "チュートリアル");

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