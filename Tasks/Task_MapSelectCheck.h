#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップ選択時の確認画面
//-------------------------------------------------------------------
#include "BObject.h"

namespace  MapSelectCheck
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ選択確認画面");	//グループ名
	const  string  defName("ディスプレイ");	//タスク名
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
		DG::Font::SP font;
		DG::Font::SP fontMin;
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
		XI::GamePad::SP controller;
		bool selectedFlag;			//選択が終了したか
		bool selectOK;				//trueなら「はい」falseなら「いいえ」が押されたと判定する

		int OKNum;					//OKとする数値を決める（主にDELETE処理用）
		int selectingNum;			//現在選択中の数値
		
		const string checkStr[4] = {	//モード別　確認時の文章
			"このファイルを読み込みますか？",						//LOAD
			"このファイルで遊びますか？",							//PLAY
			"このファイルを削除しますか？(取り消し不可)",			//REMOVE
			"このファイルに保存しますか？",							//SAVE
		};

		string mapName;				//マップ名
	};
}