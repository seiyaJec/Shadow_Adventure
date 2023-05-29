#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//チュートリアル
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "Task_Player.h"
#include "Task_Map2D.h"
#include "Task_TutorialPlayer.h"
#include "Task_TutorialKeyDisplay.h"
#include "Task_TutorialKeyStickDisplay.h"
#include "Task_Font.h"

namespace  Tutorial
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("チュートリアル");	//グループ名
	const  string  defName("NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		DG::Image::SP img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		ML::Box2D screen;				//チュートリアル用のプレイ画面
		float screenScaleRateW;			//実際の画面からの縮小率＿横幅
		float screenScaleRateH;			//実際の画面からの縮小率＿縦幅
		int categories;		//チュートリアルの項目数
		int selectingCategories;		//現在選択中の項目
		int categoryDistance;			//項目同士の間隔
		TutorialPlayer::Object::SP tutoPlayer;	//見本プレイヤーのポインタ
		Player::Object::SP player;		//プレイヤーへのポインタ
		string* categoryName;			//項目名を格納する配列へのポインタ

		int inputDataSize;				//入力の数
		InputData* inputData;			//入力情報を格納する配列へのポインタ
		KeyInput inputNow;				//現在の入力情報
		DataForPlay dataForPlay;		//ゲーム用データ

		bool retryFlagMemory;			//チュートリアル前のリトライの状態を記憶
		bool gameOverFlagMemory;		//チュートリアル前のゲームオーバーの状態を記憶
		Player::Object::SP qaPlayerMemory;	//チュートリアル前のプレイヤーへのポインタ
		Map2D::Object::SP qaMapMemory;		//チュートリアル前のマップへのポインタ
		shared_ptr<vector<BChara::SP>>qaEnemyMemory;	//チュートリアル前の敵へのポインタ

		TutorialKeyDisplay::Object::SP keyB1, keyB3, keyRT, keyA, keyD, keyM, keyL, keyShift;	//各キーのディスプレイを用意
		TutorialKeyStickDisplay::Object::SP keyStick;

		shared_ptr<vector<TutorialKeyDisplay::Object::SP>> keys;	//スティックを除くキーのディスプレイ（一括設定用）

		int waitNextInput;				//次の入力までのカウント
		int inputCnt;					//現在の入力を示すカウント
		int timeCnt;					//時間計測
		bool finishTutorial;			//チュートリアル終了フラグ

		void SetToScreen(ML::Box2D& drawBox_, ML::Box2D& srcBox_);
		void LoadInputData();														//入力情報の読み込み
		void LoadCategoryName();		//項目名の読み込み
		void CheckInput(int cnt_, const string& inp_,ifstream& fin_);				//どのボタンの入力か
		void reflectInput(KeyInput& in_, XI::VGamePad& inp_);			//入力情報の反映
		void CreateKey(TutorialKeyDisplay::Object::SP& key_, int srcOfsY_, float posX_, float posY_);				//キーの初期化
		void AppearSwitchEffect();		//項目切り替え時のエフェクト表示
		void ResetPlayer();		//見本プレイヤーリセット
		void ResetScreen();		//画面ごとリセット
		void FinishTask();		//終了処理
		string FolderPath();		//現在選択中の項目のパスを返す
	};
}