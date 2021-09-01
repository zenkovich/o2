#include "o2/stdafx.h"
#include "ActorCreationMode.h"

#include "o2/Utils/Reflection/Type.h"

ENUM_META(o2::ActorCreateMode)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(InScene);
	ENUM_ENTRY(NotInScene);
}
END_ENUM_META;
