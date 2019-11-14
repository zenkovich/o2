#pragma once

#include "Utils/Types/CommonTypes.h"
#include "Utils/Serialization/Serializable.h"
#include "Utils/Types/String.h"

namespace o2
{
	class ShortcutKeys: public ISerializable
	{
	public:
		bool        control; // @SERIALIZABLE
		bool        shift;	 // @SERIALIZABLE
		bool        alt;	 // @SERIALIZABLE
		KeyboardKey key;	 // @SERIALIZABLE

	public:
		ShortcutKeys();

		ShortcutKeys(KeyboardKey key, bool control = false, bool shift = false, bool alt = false);

		bool IsPressed() const;
		bool IsDown() const;
		String AsString() const;
		bool IsEmpty() const;

		bool operator==(const ShortcutKeys& other) const;
		bool operator<(const ShortcutKeys& other) const;

		SERIALIZABLE(ShortcutKeys);
	};
}

CLASS_BASES_META(o2::ShortcutKeys)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ShortcutKeys)
{
	PUBLIC_FIELD(control).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(shift).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(alt).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(key).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::ShortcutKeys)
{

	PUBLIC_FUNCTION(bool, IsPressed);
	PUBLIC_FUNCTION(bool, IsDown);
	PUBLIC_FUNCTION(String, AsString);
	PUBLIC_FUNCTION(bool, IsEmpty);
}
END_META;
