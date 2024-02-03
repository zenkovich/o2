#pragma once
#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Types/SmartPointers.h"

using namespace o2;

namespace Editor
{
    // Show open file dialog and returns name
    Ref<const String> GetOpenFileNameDialog(const Ref<const String>& title, const Ref<const Map<Ref<const String>, Ref<const String>>>& extensions, const Ref<const String>& defaultPath = nullptr);

    // Show save file dialog and returns name
    Ref<const String> GetSaveFileNameDialog(const Ref<const String>& title, const Ref<const Map<Ref<const String>, Ref<const String>>>& extensions, const Ref<const String>& defaultPath = nullptr);
}