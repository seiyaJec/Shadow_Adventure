#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ生成
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "Task_DataEnemy.h"

namespace  MapMaker
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ生成");	//グループ名
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
		DG::Font::SP fontDebug;
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
		GameMode		 gamemode;			//ゲーム、メーカー、セーブモードの切り替え用
		GameMode		 gamemodeBeforeLoad;	//ロード前のゲームモード
		int				 timeCnt;			//カウント
		DataForPlay dataForPlay;			//ゲーム用データ

		string		folderPath;				//読み込むフォルダのパス
		bool startSave;						//セーブ開始したか

		ML::Point playMapSize;				//メインゲーム時のマップサイズ

		bool SaveToFile(const string& stageName_,const string& makerName_);
		int SaveEnemyToFile(const string& fpath_, string& mFileName_);		//作った敵ファイルの数を返り値とする
		void LoadDataForPlay();
		void SetPlayMapSize();				//プレイ時マップサイズの設定
	};
}