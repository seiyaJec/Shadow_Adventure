#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タイトル
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"
#include "Task_TitlePlayer.h"
#include "Task_Button.h"
#include "Task_Font.h"

namespace  Title
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("タイトル");	//グループ名
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
		void  UpDate()			override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();	//「終了」タスク消滅時に１回だけ行う処理
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		vector<BResource::SP> residentResorce;		//常時読み込むリソース
		TitlePlayer::Object::SP qa_Shadow;			//影のポインタ
		XI::GamePad::SP controller;					//コントローラー
		ML::Box2D logoData[15];						//タイトルロゴ
		int timeCnt;								//時間計測
		int logoPosX;								//ロゴの位置
		bool startingGame;							//開始中か

		void LoadLogoData();
	};
}