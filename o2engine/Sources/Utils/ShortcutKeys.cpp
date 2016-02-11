#include "ShortcutKeys.h"

#include "Application/Input.h"
#include <Windows.h>

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
		char buf[16];
		LONG lScan = MapVirtualKey(key, 0) << 16;
		int ress = GetKeyNameText(lScan, buf, 16);
		int err = GetLastError();

		String res = buf;
		if (shift) res = "Shift+" + res;
		if (control) res = "Ctrl+" + res;
		if (alt) res = " Alt+" + res;

		return res;
	}

	bool ShortcutKeys::operator==(const ShortcutKeys& other) const
	{
		return key == other.key && control == other.control && shift == other.shift && alt == other.alt;
	}

}