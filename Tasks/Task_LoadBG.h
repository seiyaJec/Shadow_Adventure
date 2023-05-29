#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タスク切り替え時ロードの背景
//-------------------------------------------------------------------
#include "BObject.h"

namespace  LoadBG
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("タスクロード背景");	//グループ名
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
		int			  cuts;		//カット数
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
		bool appearFlag;			//表示処理開始フラグ
		bool disappearFlag;			//消滅処理開始フラグ
		int	   appearCnt;			//表示までの待機時間（カウント用）
		int	   appearCntMax;		//表示までの待機時間（設定用）
		int	   disappearCnt;		//消滅までの待機時間（カウント用）
		int	   disappearCntMax;		//消滅までの待機時間（設定用）
		int	   orderNum;			//表示・消滅する時の自分の順番
		float  moveSpeed;			//表示・消滅の速度
		int	   cuts;				//分割数
		bool   returnFinish;		//出現・消滅が完了したかを返す役か
		ML::Color color;				//色

		void Appear(int cuts_, int orderNum_, int distance_,float speed_);//出現
		void Disappear(int distance_, float speed_);//消滅
	};
}