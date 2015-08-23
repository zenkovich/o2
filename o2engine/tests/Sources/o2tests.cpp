#include "MathTests.h"

#include <stdio.h>
#include <conio.h>
#include "O2.h"

#include "Assets/Asset.h"

INITIALIZE_O2;

struct A;
struct B;
struct C;

struct A
{
	A()
	{
		INITIALIZE_PROPERTY(A, _vv, SetVector, GetVector);
	}

	A(int x): A()
	{}

	Ptr<B> pb;
	Vec2F mVector;

	Setter<Vec2F> _vector = Setter<Vec2F>(this, &A::SetVector);
	Property<Vec2F> _vv;

	void fun() const
	{
		Debug::Log("pb valid: %b", pb.IsValid());
	}

	void SetVector(Vec2F& vec)
	{
		Math::Swap(vec.x, vec.y);
		mVector = vec;
	}

	Vec2F GetVector()
	{
		Math::Swap(mVector.x, mVector.y);
		return mVector;
	}

	virtual void fun1(int a)
	{
		Debug::Log("A::fun1(%i)", a);
	}
};

struct B
{
	B(float f) {}
	B() {}
	Ptr<C> pc;

	virtual void fun1(int a, int b)
	{
		Debug::Log("B::fun1(%i, %i)", a, b);
	}
};

struct C: public A, public B
{
	using A::fun1;
	using B::fun1;

	Ptr<A> pa;
};

int main(char** lpCmdLine, int nCmdShow)
{
	WideTime t;
	DataNode data = t;
	DataNode res;
	AssetInfo info;
	Ptr<AssetInfo> infop = mnew AssetInfo();
	DataNode xx = 5;

	Ptr<A> pa = mnew A();
	pa->pb = mnew B();
	pa->pb->pc = mnew C();
	pa->pb->pc->pa = pa;

	Vec2F vec(3, 4);
	pa->_vv = Vec2F(1, 2);

	Debug::Log("%vf", (Vec2F)pa->_vv);

	pa.Release();
	MemoryManager::CollectGarbage();

	TestMath();

	Application app;
	app.Launch();

	MemoryManager::CollectGarbage();

	_getch();

	return 0;
}