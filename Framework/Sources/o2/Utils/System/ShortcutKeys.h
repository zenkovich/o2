#pragma once

#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
    // -------------------------
    // Shortcut keys description
    // -------------------------
    class ShortcutKeys: public ISerializable
    {
    public:
        bool        control = false; // Is control (or command) should be pressed // @SERIALIZABLE
        bool        shift = false;   // Is shift should be pressed // @SERIALIZABLE
        bool        alt = false;     // Is alt should be pressed // @SERIALIZABLE
        KeyboardKey key;             // What key should be pressed // @SERIALIZABLE

        String custromString; // Custom string representation

    public:
        // Default constructor
        ShortcutKeys();

        // Constructor by key and modifiers
        ShortcutKeys(KeyboardKey key, bool control = false, bool shift = false, bool alt = false);

        // Constructor by custom string
        ShortcutKeys(const String& customString);

        // Returns true if all keys are pressed
        bool IsPressed() const;

        // Returns true if all keys are down
        bool IsDown() const;

        // Returns string representation
        String AsString() const;

        // Returns true if all keys are empty
        bool IsEmpty() const;

        // Check equals operator
        bool operator==(const ShortcutKeys& other) const;

        // Less operator
        bool operator<(const ShortcutKeys& other) const;

        SERIALIZABLE(ShortcutKeys);
    };
}
// --- META ---

CLASS_BASES_META(o2::ShortcutKeys)
{
    BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::ShortcutKeys)
{
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(control);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(shift);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(alt);
    FIELD().PUBLIC().SERIALIZABLE_ATTRIBUTE().NAME(key);
    FIELD().PUBLIC().NAME(custromString);
}
END_META;
CLASS_METHODS_META(o2::ShortcutKeys)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(KeyboardKey, bool, bool, bool);
    FUNCTION().PUBLIC().CONSTRUCTOR(const String&);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsPressed);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsDown);
    FUNCTION().PUBLIC().SIGNATURE(String, AsString);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsEmpty);
}
END_META;
// --- END META ---
