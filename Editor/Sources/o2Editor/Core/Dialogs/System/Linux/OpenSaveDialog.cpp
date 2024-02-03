#include "o2Editor/stdafx.h"
#include "o2Editor/Core/Dialogs/System/OpenSaveDialog.h"
#include "o2Editor/Memory/Ref.h"
#include "o2Editor/Memory/WeakRef.h"
#include "o2Editor/Utils/DynamicCast.h"

#ifdef PLATFORM_LINUX

namespace Editor
{
	String GetOpenFileNameDialog(const String& title, const Map<String, String>& extensions, const String& defaultPath /*= ""*/)
	{
		return "";
	}

	String GetSaveFileNameDialog(const String& title, const Map<String, String>& extensions, const String& defaultPath /*= ""*/)
	{
		return "";
	}
}

#endif