#pragma once
#include "GameEngine_Ver3_83.h"
#include "BChara.h"
#include "MyStructure.h"
#include "Task_KeyConfig.h"
#include "Task_Player.h"
#include "Task_Play.h"
#include "Task_Title.h"
#include "Task_TitlePlayer.h"
#include "Task_Tutorial.h"
#include "Task_Result.h"
#include "Task_KeyBoard.h"
#include "Task_MapSelect.h"
#include "Task_Map2D.h"
#include "Task_MakeMapUI.h"
#include "Task_MapMaker.h"
#include "Task_IconEnemy.h"
#include "Task_IconMapchip.h"
#include "Task_Cursor.h"
#include "Task_GameDebug.h"
#include "Task_GamePause.h"
#include <fstream>
#include <sstream>

//ゲーム内全域で使用する構造体などを定義する
//-----------------------------------------------------------------------------------------------

#include "fpscounter.h"

#include "task_Effect00.h"

//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//カメラ基本形
	class Camera
	{
		Camera() { }
		Camera(
			const ML::Vec3& tg_,	//	被写体の位置
			const ML::Vec3& pos_,	//	カメラの位置
			const ML::Vec3& up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
			float				fov_,	//	視野角
			float				np_,	//	前クリップ平面（これより前は映らない）
			float				fp_,	//	後クリップ平面（これより後ろは映らない）
			float				asp_);	//	アスペクト比（画面の比率に合わせる　横÷縦）			
	public:
		//	ビュー情報（方向関連）
		ML::Vec3 target;			//	被写体の位置
		ML::Vec3 pos;			//	カメラの位置
		ML::Vec3 up;				//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
	//	射影情報（視野範囲関連）
		float fov;					//	視野角
		float nearPlane;			//	前クリップ平面（これより前は映らない）
		float forePlane;			//	後クリップ平面（これより後ろは映らない）
		float aspect;				//	アスペクト比（画面の比率に合わせる　横÷縦）
	//	行列情報
		ML::Mat4x4  matView, matProj;
		~Camera();
		using SP = shared_ptr<Camera>;
		//	カメラを生成する
		static SP Create(
			const ML::Vec3& tg_,	//	被写体の位置
			const ML::Vec3& pos_,	//	カメラの位置
			const ML::Vec3& up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
			float				fov_,	//	視野角
			float				np_,	//	前クリップ平面（これより前は映らない）
			float				fp_,	//	後クリップ平面（これより後ろは映らない）
			float				asp_);	//	アスペクト比（画面の比率に合わせる　横÷縦）	
//	カメラの設定
		void UpDate();
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine();
		//ゲームエンジンに追加したものの初期化と開放
		bool Initialize(HWND wnd_);
		~MyGameEngine();
		//ゲームエンジンに追加したもののステップ処理
		void UpDate();

		//3DPG1対応により追加
			//2D描画環境のデフォルトパラメータ設定
		void Set2DRenderState(DWORD l_);
		//3D描画環境のデフォルトパラメータ設定
		void Set3DRenderState(DWORD l_);

		//ゲームエンジンに追加したいものは下に加える
		//----------------------------------------------
		MyPG::Camera::SP		camera[4];		//	カメラ

		XI::Mouse::SP  mouse;
		XI::GamePad::SP  in1, in2, in3, in4;	//取り合えず４本
		//２Ｄカメラ矩形
		ML::Box2D				camera2D;	//  ２Ｄスクロール制御用
		map<string, float> evFlags;

		ML::Point				camera2DMovePos;	//カメラを移動させる値

		//キーコンフィグの検出を少なくするために
		KeyConfig::Object::SP				qa_KeyConfig;

		//敵の検出処理を少なくするために
		shared_ptr<vector<BChara::SP>>		qa_Enemys;
		//マップの検出処理を少なくするために
		Map2D::Object::SP					qa_Map;
		//プレイヤの検出処理を少なくするために
		Player::Object::SP					qa_Player;


		//オブジェクトの検出処理を減らす
		shared_ptr<vector<BObject::SP>>		qa_DataObjects;
		//マップUIの検出処理を減らす
		MakeMapUI::Object::SP				qa_MakeMapUI;
		//カーソルの検出処理を減らす
		Cursor::Object::SP					qa_Cursor;
		//マップ制作メインタスクの検出処理を減らす
		MapMaker::Object::SP				qa_MapMaker;
		//アイコンの検出処理を減らす
		IconEnemy::Object::SP				qa_IconEnemy;
		IconMapchip::Object::SP				qa_IconMapchip;


		//タイトル
		Title::Object::SP					qa_Title;
		TitlePlayer::Object::SP				qa_TPlayer;

		//本ゲーム
		Play::Object::SP					qa_Play;

		//チュートリアル
		Tutorial::Object::SP				qa_Tutorial;

		//リザルト
		Result::Object::SP					qa_Result;

		//マップ選択
		MapSelect::Object::SP				qa_MapSelect;

		//キーボード
		KeyBoard::Object::SP				qa_KeyBoard;

		//ロードタスクが出現、消滅完了したか（初期値はtrueに）
		bool finishedLoadAppear = true;
		bool finishedLoadDisappear = true;

		//ゴール処理（複数シーンで使用するためここで定義）
		bool goalFlag = false;
		//死亡処理
		bool gameOverflag = false;
		//リトライフラグ
		bool retryFlag = false;
		//中断フラグ
		bool pauseFlag = false;

		//チュートリアルフラグ
		bool inTutorial = false;

		//ポーズ画面で選択中のボタン
		int pauseSelectingButton = 0;

		//ステージファイル名
		string stageFilePath = "";


		//デバッグ用
		DG::Image::SP	imgDebug;
		const bool debugFlagAll = false;
		const float gravity = ML::Gravity(32) * 5;


		//----------------------------------------------
		FPSCounter* c;

		int stage;

		void CreateEffect(int no, ML::Vec2 pos);
		void Dbg_ToConsole(const char* str, ...);
		void Dbg_ToDisplay(int x, int y, const char* str, ...);
		void Dbg_ToTitle(const char* str, ...);
		void Dbg_ToBox(const char* str, ...);
		void Dbg_ToFileOut(const char* str, ...);

		//デバッグ矩形
		//ここから
		DG::Image::SP rectImage;

		enum DEBUGRECTMODE {
			RED, BLUE, GREEN, ORANGE,
			REDFRAME, BLUEFRAME, GREENFRAME, ORANGEFRAME,
		};
		struct DebugRectInfo {
			ML::Box2D	r;
			int			flag;
		};

		std::vector<DebugRectInfo> debugRectArray;
		//内部利用
		void debugRectLoad();
		void debugRectReset();
		void debugRectDraw();
		//表示したい矩形と表示モードを渡す
		//RED,BLUE,GREEN,ORANGEの矩形とFRAMEは枠のみ
		void debugRect(ML::Box2D r_, int flag = 4, int offsetx = 0, int offsety = 0);

		enum COUNTER_FLAGS {
			NONE,
			ACTIVE,//稼働中
			LIMIT,//終了直後
			END,//終了
		};

		struct COUNTER {
			COUNTER_FLAGS flag;
			int count;
		};
		std::map < std::string, COUNTER > counterMap;


		void InitCounter();
		void ActCounter();
		void AllEndCounter();
		COUNTER_FLAGS getCounterFlag(string str);
		void StartCounter(string str, int count);


	};
}
extern MyPG::MyGameEngine* ge;


