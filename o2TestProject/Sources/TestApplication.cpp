#include "TestApplication.h"

void TestApplication::OnStarted()
{
	FolderAsset folder("Images");

	ImageAsset image("Images/beam3.png");

	AtlasAsset atlas("BasicAtlas.atlas");
}

void TestApplication::OnUpdate(float dt)
{

}

void TestApplication::OnDraw()
{

}

IOBJECT_CPP(TestReflection);
IOBJECT_CPP(InhTest);
