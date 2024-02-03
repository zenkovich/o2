#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2/Memory/Ref.h"
#include "o2/Memory/WeakRef.h"

using namespace o2;

namespace o2
{
    class Toggle;
}

namespace Editor
{
    // -----------------------
    // Editor boolean property
    // -----------------------
    class BooleanProperty : public TPropertyField<bool>
    {
    public:
        // Default constructor
        BooleanProperty();

        // Copy constructor
        BooleanProperty(const BooleanProperty& other);

        // Copy operator
        BooleanProperty& operator=(const BooleanProperty& other);

        IOBJECT(BooleanProperty);

    protected:
        Ref<Toggle> mToggle;

    protected:
        // Updates value view
        void UpdateValueView() override;

        // Searches controls widgets and layers and initializes them
        void InitializeControls();
    };
}

// --- META ---

CLASS_BASES_META(Editor::BooleanProperty)
{
    BASE_CLASS(Editor::TPropertyField<bool>);
}
END_META;
CLASS_FIELDS_META(Editor::BooleanProperty)
{
    FIELD().PROTECTED().NAME(mToggle);
}
END_META;
CLASS_METHODS_META(Editor::BooleanProperty)
{
    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const Ref<BooleanProperty>&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
}
END_META;
// --- END META ---