#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ポーズ画面ボタン
//-------------------------------------------------------------------
#include "BObject.h"

namespace  PauseButton
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ポーズ");	//グループ名
	const  string  defName("ボタン");	//タスク名
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
		int orderNum;				//自分の順番
		int fontSizeW;				//フォント横幅
		int fontSizeH;				//フォント縦幅
		int appearWait;				//出現を待つ処理
		int appearTime;				//出現にかける時間
		int appearTimeMax;			//出現時間最大
		int appearSpeed;			//出現スピード
		int selectingMove;			//ボタン選択時の移動量
		int selectingMoveMax;		//ボタン選択時の移動量最大
		int selectingMoveSpeed;		//ボタン選択時の移動スピード
		int selectingAlphaDec;		//透明度減算の値
		int selectingAlphaDecSpeed;	//透明度減算のスピード
		int selectingAlphaDecMax;	//透明度減算の最大
		bool isAlphaDec;			//trueなら透明度減算、falseなら加算
		bool finishedAppear;		//出現完了したか
		string displayStr;		//ボタンに表示する文字列
	};
}