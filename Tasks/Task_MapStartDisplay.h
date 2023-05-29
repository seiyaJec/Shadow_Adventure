#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//マップのスタート地点を表示
//-------------------------------------------------------------------
#include "BObject.h"

namespace  MapStartDisplay
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("マップ選択スタート地点表示用");	//グループ名
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
		DG::Image::SP		img[100];			//マップチップ画像
		string				imgFileArr[100];	//見つかった画像
		int					arr[11][14];		//マップ配列
		int					displaySizeW;		//マップ横幅
		int					displaySizeH;		//マップ縦幅
		int					chipLength;			//チップサイズ

		int FindImageFile(int displayPos_);		//画像ファイル名を探す　見つからなかったら-1
	};
}