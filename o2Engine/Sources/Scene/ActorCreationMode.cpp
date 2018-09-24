#include "stdafx.h"
#include "ActorCreationMode.h"

ENUM_META_(o2::ActorCreateMode, ActorCreateMode)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(InScene);
	ENUM_ENTRY(NotInScene);
}
END_ENUM_META;
