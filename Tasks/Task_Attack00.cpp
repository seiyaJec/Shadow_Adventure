//-------------------------------------------------------------------
//プレイヤーの通常攻撃
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Play.h"
#include  "Task_Player.h"
#include  "Task_Attack00.h"

namespace  Attack00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		LoadAnim(this->animAttack, "./data/animation/player/Attack.txt");
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
		//画像優先度
		this->render2D_Priority[1] = 0.4f;
		//矩形
		this->drawBase = setCenterPoint(100, 74);
		this->srcBase = ML::Box2D(0, 0, 100, 74);
		this->hitBase = setCenterPoint(30, 48);
		this->MovePosToAngle(this->hitBase, 0, 14);
		this->attackBase = setCenterPoint(54, 64);
		//色
		this->color = ML::Color(1, 1, 1, 1);
		//ステータス
		this->statAttack = 10;
		//移動
		this->moveTime = 20;
		//デバッグ
		this->debugFlag = false;
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
		//攻撃部分の当たり判定
		if (this->moveCnt >= 6) {
			float vecRate = this->CheckDamageToEnemy(this->attackBase);
			if (vecRate >= 0.5f) {
				if (nullptr != ge->qa_Player) {
					ge->qa_Player->StartCameraShake(6, 2, 2, 0);
				}
			}
		}

		//体部分の当たり判定
		{
			float vecRate = this->CheckDamageToEnemy(this->hitBase);
			if (vecRate != 0) {
				if (vecRate >= 0.5f) {
					if (nullptr != ge->qa_Player) {
						ge->qa_Player->StartCameraShake(6, 2, 2, 0);
					}
				}
				this->Kill();
			}
		}
		//攻撃時間が過ぎたら消滅
		if (this->moveCnt >= this->moveTime) {
			this->Kill();
		}
		++this->moveCnt;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		AnimSet(this->res->animAttack, moveCnt / 6);

		auto draw = this->drawBase;
		auto src = this->srcBase;
		auto color = this->color;
		draw.Offset(this->pos);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//チュートリアル時は描画を変更
		DrawSetIfTuto(draw,src);


		if (nullptr != ge->qa_Player) {
			if (this->angle_LR == Angle_LR::Left) {
				ge->qa_Player->res->imgShadowLeft->Draw(draw, src, color);
			}
			else {
				ge->qa_Player->res->imgShadowRight->Draw(draw, src, color);
			}
		}
		else if (nullptr != ge->qa_TPlayer) {
			if (this->angle_LR == Angle_LR::Left) {
				ge->qa_TPlayer->res->imgShadowLeft->Draw(draw, src, color);
			}
			else {
				ge->qa_TPlayer->res->imgShadowRight->Draw(draw, src, color);
			}
		}

		//デバッグ用矩形表示
		this->DebugDrawBox(this->hitBase);
		if (this->moveCnt >= 8) {
			this->DebugDrawBox(this->attackBase);
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