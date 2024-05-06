#pragma once

#include "o2/Render/Font.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    // --------------
    // Font reference
    // --------------
    template<>
    class Ref<Font> : public IObject, public BaseRef<Font>
    {
        IOBJECT(Ref<Font>);

    public:
        // Base reference implementation
        BASE_REF_IMPLEMETATION(Font);

        // Constructor @SCRIPTABLE
        explicit Ref<Font>(const String& fileName);

    protected:
        friend class Render;
        friend class VectorFontAsset;
    };
}
// --- META ---

CLASS_BASES_META(o2::Ref<o2::Font>)
{
    BASE_CLASS(o2::IObject);
    BASE_CLASS(BaseRef<Font>);
}
END_META;
CLASS_FIELDS_META(o2::Ref<o2::Font>)
{
}
END_META;
CLASS_METHODS_META(o2::Ref<o2::Font>)
{

    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(const String&);
}
END_META;
// --- END META ---
