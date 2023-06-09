//-------------------------------------------------------------------
//UgζΚpXRA{[h
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_ResultScoreBoard.h"

namespace  ResultScoreBoard
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Result/Result_ScoreBoard.png");
		this->font = DG::Font::Create("lr SVbN", 33, 66);
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->font.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		//ζDζx
		this->render2D_Priority[1] = 0.8f;
		//ζTCY
		this->SetImgSize(592, 220);
		//tHgTCY
		this->fontSizeW = 33;
		this->fontSizeH = 66;
		//ι`
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		//\¦p
		this->clearTime = "NA^C@";
		this->retryConut = "gCρ@";
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		//wi
		{
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			this->res->img->Draw(draw, this->srcBase);
		}
		//NA^C
		{
			string clearTimeDisplay = this->clearTime + to_string(ge->qa_Result->clearTime);
			ML::Box2D draw = setCenterPoint(
				this->fontSizeW * (int)clearTimeDisplay.size(),
				this->fontSizeH
			);
			ML::Vec2 clearTimePos;
			clearTimePos.x = this->pos.x;
			clearTimePos.y = this->pos.y - 50;
			draw.Offset(clearTimePos);
			this->res->font->Draw(draw, clearTimeDisplay, ML::Color(1, 0, 0, 0));
		}
		//gCρ
		{
			string retryCountDisplay = this->retryConut + to_string(ge->qa_Result->retryCount);
			ML::Box2D draw = setCenterPoint(
				this->fontSizeW * (int)retryCountDisplay.size(),
				this->fontSizeH
			);
			ML::Vec2 retryCountPos;
			retryCountPos.x = this->pos.x;
			retryCountPos.y = this->pos.y + this->fontSizeH - 10;
			draw.Offset(retryCountPos);
			this->res->font->Draw(draw, retryCountDisplay, ML::Color(1, 0, 0, 0));
		}
	}

	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//\[XNXΜΆ¬
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}