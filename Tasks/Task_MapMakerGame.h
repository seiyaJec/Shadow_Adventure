#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ生成のゲームモード
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  MapMakerGame
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ゲームモード");	//グループ名
	const  string  defName("マップ制作");	//タスク名
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
		void  UpDate()			override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();	//「終了」タスク消滅時に１回だけ行う処理
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		vector<BResource::SP> residentResorce;
		int timeCnt;

		bool timeReseted;		//時間リセット済みか
		bool stoppedTask;		//タスクを止めたか

		int timeCntGame;			//ゲーム内時間計測
		int timeCntGameMax;			//ゲーム内時間最大
		int timeCntGameDistance;	//ゲーム内時間計測の間隔

		int translateTimeDisplay();	//設定した制限時間を実装する処理
	};
}