#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ制作プレイヤーHP設定用ボタン
//-------------------------------------------------------------------
#include "BObject.h"
#include "Task_MakerSetPlayerHPDisplay.h"

namespace  MakerSetPlayerHPButton
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ制作UI");	//グループ名
	const  string  defName("プレイヤーHP設定ボタン");	//タスク名
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
	class  Object : public  BObject
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
		enum ButtonMode{BOTTOM = -1, TOP = 1};		//ボタンが上か下か
		ButtonMode buttonMode;						//ボタンが上か下か
		MakerSetPlayerHPDisplay::Object::SP linkingDisplay;		//自分と紐づいているディスプレイ
		int addPosY;								//位置の調整
	};
}