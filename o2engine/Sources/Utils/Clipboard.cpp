#include "Clipboard.h"

#include <Windows.h>

namespace o2
{

	void Clipboard::SetText(const WString& text)
	{
		if (OpenClipboard(NULL))
		{
			HGLOBAL hgBuffer;
			wchar_t* chBuffer;
			EmptyClipboard();
			hgBuffer= GlobalAlloc(GMEM_DDESHARE, (text.Length() + 1)*sizeof(wchar_t));
			chBuffer= (wchar_t*)GlobalLock(hgBuffer);
			memcpy(chBuffer, text.Data(), (text.Length() + 1)*sizeof(wchar_t));
			GlobalUnlock(hgBuffer);
			SetClipboardData(CF_UNICODETEXT, hgBuffer);
			CloseClipboard();
		}
	}

	WString Clipboard::GetText()
	{
		WString res;

		if (OpenClipboard(NULL))
		{
			HANDLE hData = GetClipboardData(CF_UNICODETEXT);
			wchar_t* chBuffer= (wchar_t*)GlobalLock(hData);
			res = chBuffer;
			GlobalUnlock(hData);
			CloseClipboard();
		}

		return res;
	}

}