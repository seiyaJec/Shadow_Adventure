#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//エフェクト3　飛び散る光の粒
//-------------------------------------------------------------------
#include "BObject.h"

namespace  Effect03
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("エフェクト");	//グループ名
	const  string  defName("03");	//タスク名
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
		Anim anim;
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
		float angle;		//向き
		int   animCnt;		//アニメーション用カウント
		int	  animDistance;  //アニメーションの間隔

		void Set(ML::Vec2 pos_, float angle = 0, int animDistance = 10);
		void Circle(ML::Vec2 pos_, float distance_, int efctSize_);		//円形に飛び散る(アングル設定後に使用するように）

	};
}