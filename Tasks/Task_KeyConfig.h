#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//キーコンフィグ
//-------------------------------------------------------------------
#include "BObject.h"

namespace  KeyConfig
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("キーコンフィグ");	//グループ名
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
		enum GameMode { Select, SelectSave, KeyBoard, Play, Maker, Pause, ResultCheck, Tutorial };
		int gamemode;	//現在のモード
		float defaultSpeed;	//移動スピード
		float speedRate;	//現在の速さ（倍率）
		float speedAttenuationRate;//出現のスピード減衰率
		float speedAcceleration;   //消滅のスピード加速率
		float attenuations;	//スピード減衰回数（画像サイズを均等分して行われる）


		bool triggerDowned;	//トリガーが押されたか
		bool appearFlag;	//出現
	};
}