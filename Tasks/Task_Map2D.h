#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//二次元配列マップ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Map2D
{
	struct ChipData {
		ML::Box2D srcBox;
		ChipType chipType;
		ChipData() :
			srcBox(0, 0, 0, 0),
			chipType(ChipType::AIR)
		{}
	};
	struct BackGroundData {
		DG::Image::SP img;
		int imgSizeW;
		int imgSizeH;
	};
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("フィールド");	//グループ名
	const  string  defName("マップ");	//タスク名
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
		DG::Image::SP		img;
		DG::Image::SP		bgImg[10];
		int					arr[100][100];
		int					sizeY, sizeX;
		int					imgSizeW, imgSizeH;	//画像サイズ
		int					tiles;				//タイル総数
		int					length;				//一辺の長さ
		int					backGrounds;		//背景の数
		int					columns;			//列の数
		bool				foundSpike;			//針が見つかったか
		bool				foundGoal;			//ゴールが見つかったか
		BackGroundData		backGround[10];		//背景の大きさ
		ML::Box2D			drawBase;			//描画用の矩形
		ML::Box2D			hitBase;			//ピクセル単位のマップサイズを持つ
		ChipData			chipData[1000];		//各マップチップのデータ（タイプ分け）
		int					chipArrTypeMax;		//マップチップのタイプ総数
		ChipArr* chipArr;						//マップチップ結合配列（宣言はcpp）
		


		bool Load(const string& fpath_);//マップ読み込み
		bool LoadChipData(const string& fpath_);//チップデータ読み込み
		bool LoadMapChipArray(const string& fpath_);//マップチップ同士の結合用配列読み込み
		bool LoadEnemy(const string& efpath_);//敵情報読み込み
		bool CheckHit(const ML::Box2D& hit_);//当たり判定
		
		void ChangeMapSize(int sizeX_, int sizeY_);
		void chipEffect();			//エフェクト
		void AdjustCameraPos();
	};
}