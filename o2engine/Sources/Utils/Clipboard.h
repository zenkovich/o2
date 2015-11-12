#pragma once

#include "Utils/Singleton.h"
#include "Utils/String.h"

namespace o2
{
	class Clipboard
	{
	public:
		// Sets text into clipboard
		static void SetText(const WString& text);

		// Gets text from clipboard
		static WString GetText();
	};
}