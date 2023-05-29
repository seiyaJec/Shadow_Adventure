#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ選択
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  MapSelect
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ選択");	//グループ名
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
		XI::GamePad::SP controller;
		enum POSITION { UP, CENTER, DOWN };
		enum SELECTMODE { TITLE = -1, LOAD, PLAY, REMOVE, SAVE };
		int selectMode;		//選択のモード
		int selectingPos;			//選択中のステージ（上・中・下）
		int page;					//現在のページ
		int pageMax;				//ページ最大
		int displayPos;				//選択中のステージ（全配列中のどれか）
		bool saving;
		bool appearLoad;				//ロード開始したか
		bool backToTitle;			//タイトルに戻るか
		string folderPath;			//選択したフォルダーへのパス
		MapFileData mapData[15];
		string stageNameInput;		//セーブ時のマップ名入力
		string makerNameInput;		//セーブ時の制作者名入力

		bool inCheck;				//確認画面を表示中か　

		void LoadMapFile();
	};
}