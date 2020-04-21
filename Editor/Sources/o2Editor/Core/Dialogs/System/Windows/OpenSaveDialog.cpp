#include "o2Editor/stdafx.h"
#include "o2Editor/Core/Dialogs/System/OpenSaveDialog.h"

#ifdef PLATFORM_WINDOWS

namespace Editor
{
	String GetOpenFileNameDialog(const String& title, const Map<String, String>& extensions)
	{
		char szFile[MAX_PATH];
		char oldDir[MAX_PATH];

		GetCurrentDirectory(MAX_PATH, oldDir);

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = o2Application.GetWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

		int filterStrSize = extensions.Sum<int>([](auto& k, auto& v) { return k.Length() + v.Length() + 2; }) + 1;
		char* filterStr = new char[filterStrSize];
		filterStr[filterStrSize - 1] = '\0';
		ofn.lpstrFilter = filterStr;

		int i = 0;
		for (auto x : extensions)
		{
			memcpy(filterStr + i, x.first.Data(), x.first.Length() + 1);
			i += x.first.Length() + 1;

			memcpy(filterStr + i, x.second.Data(), x.second.Length() + 1);
			i += x.second.Length() + 1;
		}

		if (GetOpenFileName(&ofn)==TRUE)
		{
			SetCurrentDirectory(oldDir);
			return ofn.lpstrFile;
		}

		return "";
	}

	String GetSaveFileNameDialog(const String& title, const Map<String, String>& extensions)
	{
		char szFile[MAX_PATH];
		char oldDir[MAX_PATH];

		GetCurrentDirectory(MAX_PATH, oldDir);

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = o2Application.GetWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

		int filterStrSize = extensions.Sum<int>([](auto& k, auto& v) { return k.Length() + v.Length() + 2; }) + 1;
		char* filterStr = new char[filterStrSize];
		filterStr[filterStrSize - 1] = '\0';
		ofn.lpstrFilter = filterStr;

		int i = 0;
		for (auto x : extensions)
		{
			memcpy(filterStr + i, x.first.Data(), x.first.Length() + 1);
			i += x.first.Length() + 1;

			memcpy(filterStr + i, x.second.Data(), x.second.Length() + 1);
			i += x.second.Length() + 1;
		}

		if (GetSaveFileName(&ofn) == TRUE)
		{
			SetCurrentDirectory(oldDir);
			return ofn.lpstrFile;
		}

		return "";
	}
}

#endif