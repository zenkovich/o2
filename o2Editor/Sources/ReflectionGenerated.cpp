#include "ReflectionGenerated.h"

#include "O2.h"
using namespace o2;

#include "ApplicationConfig.h"
#include "Core/WindowsSystem/IEditorWindow.h"
#include "Core/WindowsSystem/UIDockWindowPlace.h"
#include "Core/WindowsSystem/UIDockableWindow.h"
#include "SceneWindow/SceneWindow.h"


IOBJECT_CPP(ApplicationConfig);
IOBJECT_CPP(UIDockWindowPlace);
IOBJECT_CPP(IEditorWindow);
IOBJECT_CPP(SceneWindow);
IOBJECT_CPP(UIDockableWindow);

void ReflectionRegistTypes()
{
	INIT_TYPE(IEditorWindow);
	INIT_TYPE(UIDockableWindow);
	INIT_TYPE(ApplicationConfig);
	INIT_TYPE(SceneWindow);
	INIT_TYPE(UIDockWindowPlace);
}
