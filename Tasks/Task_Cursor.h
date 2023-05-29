#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マウスカーソル
//-------------------------------------------------------------------
#include "BObject.h"

namespace  Cursor
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("カーソル");	//グループ名
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
		XI::GamePad::SP  controller;		//コントローラー
		BuState leftButton;					//左クリック
		BuState rightButton;				//右クリック
		CursorMode cursorMode;				//カーソルのモード
		ML::Color color;					//色
		ML::Vec2 screenPos;					//スクリーン上の座標
		ML::Vec2 preMouseCursorPos;			//1フレーム前のマウスの座標
		bool inGameMap;						//マップ内にカーソルがあるか
		int  moveCount;						//1以上なら移動モード
		bool movingObject;					//移動モード中か


		void MoveMouseCursor(XI::VMouse& ms_);
		void LinkToInput(XI::VMouse& ms_,XI::VGamePad& inp_);
		bool CheckLeftState(BuState state_);
		bool CheckRightState(BuState state_);
		bool Hit(ML::Box2D hitBox_);
		void IsInGameMap();

	};
}