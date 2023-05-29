//-------------------------------------------------------------------
//マウスカーソル
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Cursor.h"
#include  "Task_IconEnemy.h"
#include  "Task_HideMakeMapUIButton.h"
#include  "Task_Effect.h"

namespace  Cursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		img = DG::Image::Create("./data/image/MapMaker/cursor.png");
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
		//画像優先度
		this->render2D_Priority[1] = 0.2f;
		//コントローラー
		this->controller = ge->in1;
		//画像サイズ
		this->SetImgSize(32, 32);
		//矩形
		this->drawBase = ML::Box2D(0, 0, imgSizeW, imgSizeH);
		//位置情報
		this->pos = ML::Vec2(0, 0);
		this->screenPos = ML::Vec2(0, 0);
		this->preMouseCursorPos = ML::Vec2(0, 0);
		//色
		this->color = ML::Color(1, 1, 1, 1);
		//ボタン状態
		this->leftButton = BuState::OFF;
		this->rightButton = BuState::OFF;
		this->cursorMode = CursorMode::ENEMY;
		this->inGameMap = false;
		//移動情報
		this->moveCount = 0;
		this->movingObject = false;
		//カーソル非表示
		ShowCursor(false);
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ShowCursor(true);
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto ms = ge->mouse->GetState();
		auto inp = this->controller->GetState();
		this->color = ML::Color(1, 1, 1, 1);

		//マウスボタンとカーソルの状態をリンクする
		this->LinkToInput(ms, inp);
		//マップ内にカーソルがあるか確認
		this->IsInGameMap();

		


		//左ボタンが押されたときの処理
		if (ge->qa_Cursor->CheckLeftState(BuState::DOWN)) {
			auto effect = Effect03::Object::Create(true);
			effect->Circle(this->pos, 20, 5);
			if (true == this->inGameMap) {
				if (false == this->movingObject) {
					//マップ上でクリックしたときの処理
					switch (this->cursorMode) {
					case CursorMode::ENEMY:
						ge->qa_IconEnemy->Generate();
						break;
					}
				}
			}
		}

		//LRでモード切替
		if (inp.R1.down) {
			if (false == this->movingObject) {
				if (this->cursorMode == CursorMode::ENEMY) {
					this->cursorMode = CursorMode::MAP;
				}
			}
		}
		else if(inp.L1.down) {
			if (false == this->movingObject) {
				if (this->cursorMode == CursorMode::MAP) {
					this->cursorMode = CursorMode::ENEMY;
				}
			}
		}

		//移動モードのon・off
		if (this->moveCount > 0) {
			this->movingObject = true;
		}
		else {
			this->movingObject = false;
		}

		//ボタンクリック中に色を濃くする
		if (this->CheckLeftState(BuState::ON)) {
			this->color = ML::Color(1.0f, 1.0f, 1.0f, 0.3f);
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		//ゲーム中ならカメラの位置分引く
		if (this->cursorMode != CursorMode::FREE) {
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		}
		this->ApplySize(draw);
		ML::Box2D src(0, 0, imgSizeW, imgSizeH);
		this->res->img->Draw(draw, src,this->color);
	}

	//-------------------------------------------------------------------
	//関数定義
	//-------------------------------------------------------------------
	//矩形とカーソルの当たり判定
	bool Object::Hit(ML::Box2D hitBox_)
	{
		if (this->pos.x < hitBox_.x) {
			return false;
		}
		if (this->pos.x >= hitBox_.x + hitBox_.w) {
			return false;
		}
		if (this->pos.y < hitBox_.y) {
			return false;
		}
		if (this->pos.y >= hitBox_.y + hitBox_.h) {
			return false;
		}
		return true;
	}

	//マウスカーソルがゲームマップ内にいるか
	void Object::IsInGameMap()
	{
		//UIバーと当たっていたらゲームマップ内にいない
		auto hideButton = ge->GetTask<HideMakeMapUIButton::Object>("マップ制作UI", "UI非表示ボタン");
		if(this->Hit(ge->qa_MakeMapUI->hitBase.OffsetCopy(ge->qa_MakeMapUI->pos))){
			this->inGameMap = false;
		}
		else if (this->Hit(hideButton->hitBase.OffsetCopy(hideButton->pos))) {
			this->inGameMap = false;
		}
		else {
			this->inGameMap = true;
		}
	}


	//マウスカーソルの移動
	void Object::MoveMouseCursor(XI::VMouse& ms_)
	{
		//値を加算
		this->screenPos.x += ms_.pos_Screen.x - preMouseCursorPos.x;
		this->screenPos.y += ms_.pos_Screen.y - preMouseCursorPos.y;
		SetCursorPos(100, 100);
		//今の値を記憶
		this->preMouseCursorPos.x = 100;
		this->preMouseCursorPos.y = 100;


		//移動の制限
		if (this->screenPos.x < 0) {
			this->screenPos.x = 0;
		}
		if (this->screenPos.x >= ge->screen2DWidth) {
			this->screenPos.x = ge->screen2DWidth - 1.0f;
		}
		if (this->screenPos.y < 0) {
			this->screenPos.y = 0;
		}
		if (this->screenPos.y >= ge->screen2DHeight) {
			this->screenPos.y = ge->screen2DHeight - 1.0f;
		}

	}

	//マウスのボタン情報を受け取る
	void Object::LinkToInput(XI::VMouse& ms_, XI::VGamePad& inp_)
	{
		//Lスティックで移動
		float angle = inp_.LStick.angleDYP;
		float volume = inp_.LStick.volume;
		if (volume > 0) {
			ML::Vec2 movePos;
			this->screenPos.x += cos(angle)*(volume * 14);
			this->screenPos.y += sin(angle)*(volume * 14);
		}
		//マウスカーソル移動情報を座標に変換
		this->MoveMouseCursor(ms_);
		//カメラがあるならカメラの位置を原点にする
		if (this->cursorMode != CursorMode::FREE) {
			this->pos.x = this->screenPos.x + ge->camera2D.x;
			this->pos.y = this->screenPos.y + ge->camera2D.y;
		}
		//カメラが無ければスクリーンで計算
		else {
			this->pos.x = this->screenPos.x;
			this->pos.y = this->screenPos.y;
		}


		//左ボタン
		if (leftButton != BuState::ON && (ms_.LB.down || inp_.B1.down)) {
			leftButton = BuState::DOWN;
		}
		else if (ms_.LB.on || inp_.B1.on) {
			leftButton = BuState::ON;
		}
		else if (ms_.LB.up || inp_.B1.up) {
			leftButton = BuState::UP;
		}
		else{
			leftButton = BuState::OFF;
		}

		//左ボタン
		if (rightButton != BuState::ON && (ms_.RB.down || inp_.B2.down)) {
			rightButton = BuState::DOWN;
		}
		else if (ms_.RB.on || inp_.B2.on) {
			rightButton = BuState::ON;
		}
		else if (ms_.RB.up || inp_.B2.up) {
			rightButton = BuState::UP;
		}
		else {
			rightButton = BuState::OFF;
		}

	}
	//左ボタンが押されたときの反応
	bool Object::CheckLeftState(BuState state_)
	{
		if (this->leftButton == state_) {
			return true;
		}
		else if (state_ == BuState::ON){
			if (this->leftButton == BuState::DOWN) {
				return true;
			}
		}
		else if (state_ == BuState::OFF) {
			if (this->leftButton == BuState::UP) {
				return true;
			}
		}
		return false;
	}

	//右ボタンが押されたときの反応
	bool Object::CheckRightState(BuState state_)
	{
		if (this->rightButton == state_) {
			return true;
		}
		else if (state_ == BuState::ON) {
			if (this->rightButton == BuState::DOWN) {
				return true;
			}
		}
		else if (state_ == BuState::OFF) {
			if (this->rightButton == BuState::UP) {
				return true;
			}
		}
	return false;
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