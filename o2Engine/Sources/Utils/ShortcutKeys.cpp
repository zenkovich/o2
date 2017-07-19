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

	bool ShortcutKeys::IsEmpty() const
	{
		return key == -1 && !control && !shift && !alt;
	}

	bool ShortcutKeys::operator==(const ShortcutKeys& other) const
	{
		return key == other.key && control == other.control && shift == other.shift && alt == other.alt;
	}

}

CLASS_META(o2::ShortcutKeys)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(control).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(shift).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(alt).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(key).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(bool, IsPressed);
	PUBLIC_FUNCTION(bool, IsDown);
	PUBLIC_FUNCTION(String, AsString);
	PUBLIC_FUNCTION(bool, IsEmpty);
}
END_META;
