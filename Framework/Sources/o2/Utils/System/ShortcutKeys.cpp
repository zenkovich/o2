#include "o2/stdafx.h"
#include "ShortcutKeys.h"

#include "o2/Application/Input.h"

#if defined PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace o2
{
	ShortcutKeys::ShortcutKeys():
		key(-1), control(false), shift(false), alt(false)
	{}

	ShortcutKeys::ShortcutKeys(KeyboardKey key, bool control /*= false*/, bool shift /*= false*/, bool alt /*= false*/) :
		key(key), control(control), shift(shift), alt(alt)
	{}

	bool ShortcutKeys::IsPressed() const
	{
		return o2Input.IsKeyPressed(key) &&
			(shift ? o2Input.IsKeyDown(VK_SHIFT) : !o2Input.IsKeyDown(VK_SHIFT)) &&
			(control ? o2Input.IsKeyDown(VK_CONTROL) : !o2Input.IsKeyDown(VK_CONTROL)) &&
			(alt ? o2Input.IsKeyDown(VK_MENU) : !o2Input.IsKeyDown(VK_MENU));
	}

	bool ShortcutKeys::IsDown() const
	{
		return o2Input.IsKeyDown(key) &&
			(shift ? o2Input.IsKeyDown(VK_SHIFT) : !o2Input.IsKeyDown(VK_SHIFT)) &&
			(control ? o2Input.IsKeyDown(VK_CONTROL) : !o2Input.IsKeyDown(VK_CONTROL)) &&
			(alt ? o2Input.IsKeyDown(VK_MENU) : !o2Input.IsKeyDown(VK_MENU));
	}

	String ShortcutKeys::AsString() const
	{
		String res;

#if defined PLATFORM_WINDOWS
		char buf[16];
		LONG lScan = MapVirtualKey(key, 0) << 16;
		int ress = GetKeyNameText(lScan, buf, 16);
		int err = GetLastError();

		res = buf;
		if (shift) res = "Shift+" + res;
		if (control) res = "Ctrl+" + res;
		if (alt) res = " Alt+" + res;
#endif

		return res;
	}

	bool ShortcutKeys::IsEmpty() const
	{
		return key == -1 && !control && !shift && !alt;
	}

	bool ShortcutKeys::operator<(const ShortcutKeys& other) const
	{
		int w1 = (control ? 1 : 0) + (shift ? 10 : 0) + (alt ? 100 : 0) + key*1000;
		int w2 = (other.control ? 1 : 0) + (other.shift ? 10 : 0) + (other.alt ? 100 : 0) + other.key*1000;

		return w1 < w2;
	}

	bool ShortcutKeys::operator==(const ShortcutKeys& other) const
	{
		return key == other.key && control == other.control && shift == other.shift && alt == other.alt;
	}

}

DECLARE_CLASS(o2::ShortcutKeys);
