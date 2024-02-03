#include "o2Editor/stdafx.h"
#include "o2Editor/Core/Dialogs/System/OpenSaveDialog.h"
#include <memory>

#ifdef PLATFORM_MAC

namespace Editor
{
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    template<typename T, typename... Args>
    Ref<T> mmake(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename Derived, typename Base>
    Ref<Derived> DynamicCast(const Ref<Base>& ptr)
    {
        return std::dynamic_pointer_cast<Derived>(ptr);
    }

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