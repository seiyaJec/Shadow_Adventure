#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//フォント自動設定タスク
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Font
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("フォント自動設定");	//グループ名
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
		DG::Font::SP font;				//フォント
		ML::Vec2	 pos;				//位置
		ML::Color	 color;				//色

		void SetPos(float x_, float y);		//位置の指定
		void SetPos(const ML::Vec2& pos_);	//位置の指定
		void SetSize(int w_, int h_);	//サイズの設定
		void SetString(const string& str_);	//表示する文字列の設定
	private:
		//間違って変更できないようにする変数はここへ
		bool finishSet;					//サイズが設定済みか
		int fontSizeW;					//1文字の横幅
		int fontSizeH;					//1文字の縦幅
		ML::Box2D drawBase;				//描画矩形
		string displayStr;				//表示する文字列
	};
}