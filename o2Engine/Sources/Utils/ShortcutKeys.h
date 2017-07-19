#pragma once

#include "Utils/CommonTypes.h"
#include "Utils/Serializable.h"
#include "Utils/String.h"

namespace o2
{
	class ShortcutKeys: public ISerializable
	{
	public:
		bool        control; // @SERIALIZABLE
		bool        shift;	 // @SERIALIZABLE
		bool        alt;	 // @SERIALIZABLE
		KeyboardKey key;	 // @SERIALIZABLE

		ShortcutKeys();

		ShortcutKeys(KeyboardKey key, bool control = false, bool shift = false, bool alt = false);

		bool IsPressed() const;
		bool IsDown() const;
		String AsString() const;
		bool IsEmpty() const;

		bool operator==(const ShortcutKeys& other) const;

		SERIALIZABLE(ShortcutKeys);
	};
}
