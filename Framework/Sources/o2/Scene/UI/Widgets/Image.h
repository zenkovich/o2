#pragma once

#include "o2/Assets/Types/ImageAsset.h"
#include "o2/Scene/UI/Widget.h"

namespace o2
{
    class Sprite;

    // ------------
    // Image widget
    // ------------
    class Image: public Widget
    {
    public:
        PROPERTIES(Image);
        PROPERTY(Ref<Sprite>, image, SetImage, GetImage);                  // Image sprite @SCRIPTABLE
        PROPERTY(Ref<ImageAsset>, imageAsset, SetImageAsset, GetImageAsset); // Image asset @SCRIPTABLE
        PROPERTY(String, imageName, SetImageName, GetImageName);           // Image asset name @SCRIPTABLE

    public:
        // Default constructor @SCRIPTABLE
        explicit Image(RefCounter* refCounter);

        // Copy-constructor
        Image(RefCounter* refCounter, const Image& other);

        // Assign operator
        Image& operator=(const Image& other);

        // Sets image @SCRIPTABLE
        void SetImage(const Ref<Sprite>& sprite);

        // Returns image sprite @SCRIPTABLE
        Ref<Sprite> GetImage();

        // Sets image asset @SCRIPTABLE
        void SetImageAsset(const Ref<ImageAsset>& asset);

        // Returns image asset @SCRIPTABLE
        Ref<ImageAsset> GetImageAsset() const;

        // Sets image asset name @SCRIPTABLE
        void SetImageName(const String& name);

        // Returns asset image name @SCRIPTABLE
        String GetImageName() const;

        // Returns create menu group in editor
        static String GetCreateMenuGroup();

        SERIALIZABLE(Image);
        CLONEABLE_REF(Image);

    protected:
        WeakRef<Sprite> mImage; // Image layer drawable

        friend class UIButtonGroup;
    };
}
// --- META ---

CLASS_BASES_META(o2::Image)
{
    BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(o2::Image)
{
    FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(image);
    FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(imageAsset);
    FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(imageName);
    FIELD().PROTECTED().NAME(mImage);
}
END_META;
CLASS_METHODS_META(o2::Image)
{

    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(RefCounter*);
    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*, const Image&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetImage, const Ref<Sprite>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<Sprite>, GetImage);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetImageAsset, const Ref<ImageAsset>&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Ref<ImageAsset>, GetImageAsset);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetImageName, const String&);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(String, GetImageName);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuGroup);
}
END_META;
// --- END META ---
