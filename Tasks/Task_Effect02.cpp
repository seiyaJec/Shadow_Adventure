//-------------------------------------------------------------------
//GtFNg02 i^Cgpj
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect02.h"

namespace  Effect02
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Effect/TitleSmoke.png");
		LoadAnim(this->anim, "./data/animation/Effect/Effect02.txt");
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
		//ζDζx
		this->render2D_Priority[1] = 0.3f;
		//Κuξρ
		this->pos = ML::Vec2(0, 0);
		//ι`
		this->drawBase = setCenterPoint(this->res->anim.imgSizeW, this->res->anim.imgSizeH);
		this->srcBase = ML::Box2D(0, 0, this->res->anim.imgSizeW, this->res->anim.imgSizeH);
		//Aj[V
		this->animDistance = 10;
		this->animCnt = 0;
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
		if (this->animCnt >= this->res->anim.Max * this->animDistance) {
			this->Kill();
		}
		++this->animCnt;
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		int animNum = this->animCnt / this->animDistance;

		ML::Box2D draw = this->drawBase.OffsetCopy(this->pos);
		ML::Box2D src = this->srcBase.OffsetCopy(this->res->anim.Pos[animNum]);
		this->res->img->Draw(draw, src);
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