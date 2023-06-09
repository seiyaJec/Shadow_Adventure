//-------------------------------------------------------------------
//^CgpQ[ZN^[
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_GameSelector.h"
#include  "Task_Button.h"
#include  "Task_Effect.h"

namespace  GameSelector
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Title/GameSelector.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->render2D_Priority[1] = 0.29f;
		this->SetImgSize(208, 130);
		this->drawBase = setCenterPoint(this->imgSizeW, this->imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->imgSizeW, this->imgSizeH);
		this->animDistance = 40;
		this->pressStartFlag = false;
		this->pos = ge->qa_Title->qa_ButtonPlay->pos;
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
		if (false == this->pressStartFlag) {
			switch (ge->qa_Title->gameMode) {
			case 0:
			{
				this->pos = ge->qa_Title->qa_ButtonPlay->pos;
				break;
			}
			case 1:
			{
				this->pos = ge->qa_Title->qa_ButtonMake->pos;
				break;
			}
			case 2:
			{
				this->pos = ge->qa_Title->qa_ButtonEndless->pos;
				break;
			}
			}


			//Aj[Viζgkj
			if (this->animCnt < this->animDistance) {
				this->addSize = 0;
			}
			else if (this->animCnt < this->animDistance * 2) {
				this->addSize = 1;
			}
			else {
				this->animCnt = 0;
			}

			if (ge->qa_Title->startingGame == true) {
				this->pressStartFlag = true;
				auto efct = Effect03::Object::Create(true);
				efct->Circle(this->pos, 50, 8);
			}
		}
		else {
		}

		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		if (false == this->pressStartFlag) {
			ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
			this->ApplySizeMove(draw);
			ML::Box2D src = this->srcBase;
			this->res->img->Draw(draw, src);
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