#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイ中断
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "Task_PauseButton.h"

namespace  GamePause
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ポーズ");	//グループ名
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
		XI::GamePad::SP  controller;
		enum ButtonType { CONTINUE, RETRY, TUTORIAL, SELECT, TITLE };

		int buttonMax;				//ボタンの数
		int timeCnt;				//時間計測
		int buttonDistanceX;		//ボタンを表示する間隔(X方向)
		int buttonDistanceY;		//ボタンを表示する間隔(Y方向）
		int buttonGenerateDistance;	//ボタンが表示される間隔（時間）
		bool loadingSelect;			//セレクトを読み込み中
		bool loadingTitle;			//タイトルを読み込み中
		bool loadingTutorial;		//チュートリアルを読み込み中
		bool finishedAppearTutorial;//チュートリアル出現終了したか
		bool appeardLastButton;		//最後のボタンが表示されたか
		PauseButton::Object::SP		lastButton;

		void GenerateButton(ML::Vec2 startPos_, float addPosX_, float addPosY_, int generateDistance_);
		void SuspendGameTask();			//ゲームタスクを停止から無効状態に
		void StopGameTask();			//ゲームタスクを無効から停止状態に
		void RestartGameTask();			//ゲームタスクを再開
		void KillGameTask();			//ゲームタスクをキル
	};
}