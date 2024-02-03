The edited code with the requested changes is as follows:

```cpp
#pragma once

#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Render/Text.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2Editor/AssetsWindow/AssetsIconsScroll.h"
#include "o2Editor/AssetsWindow/AssetsWindow.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace Editor
{
    class ObjectViewer;

    // -------------------------
    // Editor asset property box
    // -------------------------
    class AssetProperty : public TPropertyField<Ref<Asset>>, public DragDropArea, public KeyboardEventsListener
    {
    public:
        // Default constructor
        AssetProperty();

        // Copy constructor
        AssetProperty(const AssetProperty& other);

        // Copy operator
        AssetProperty& operator=(const AssetProperty& other);

        // Returns editing by this field type
        const Type* GetValueType() const override;

        // Returns editing by this field type by static function, can't be changed during runtime
        static const Type* GetValueTypeStatic();

        // Sets value asset id
        void SetAssetId(const UID& id);

        // Sets asset type
        void SetAssetType(const Type* assetType);

        // Specializes field info
        void SetFieldInfo(const FieldInfo* fieldInfo) override;

        // Sets property caption
        void SetCaption(const WString& text) override;

        // Returns property caption
        WString GetCaption() const override;

        // Adds remove button
        Button* GetRemoveButton() override;

        // Returns true if point is in this object
        bool IsUnderPoint(const Vec2F& point) override;


        IOBJECT(AssetProperty);

    protected:
        Ref<Widget> mBox;                // Property edit box
        Ref<Text> mNameText;             // Asset name text
        Ref<Spoiler> mSpoiler;           // Spoiler
        Ref<Label> mCaption;             // Property name caption
        Ref<Button> mCreateInstanceBtn;  // Create instance button

        Ref<HorizontalLayout> mHeaderContainer;  // Asset controls container: create, save and remove

        WeakRef<ObjectViewer> mAssetObjectViewer;  // Asset instance viewer. Created if required

        const Type* mAssetType = nullptr;  // Type of asset

        bool mAvailableToHaveInstance = false;  // Is asset can own an instance

    protected:
        // Searches controls widgets and layers and initializes them
        void InitializeControls();

        // Sets common value asset id
        void SetCommonAssetId(const UID& id);

        // Sets asset id, checks value changed, calls onChangeCompleted
        void SetAssetIdByUser(const UID& id);

        // Called when create instance button pressed, creates asset instance
        void OnCreateInstancePressed();

        // Called when remove instance button pressed, removes asset instance
        void OnRemoveInstancePressed();

        // Called when save instance button pressed, saves asset instance as asset
        void OnSaveInstancePressed();

        // Called when type specialized during setting value proxy
        void OnTypeSpecialized(const Type& type) override;

        // Returns value from proxy
        Ref<Asset> GetProxy(IAbstractValueProxy* proxy) const override;

        // Sets value to proxy
        void SetProxy(IAbstractValueProxy* proxy, const Ref<Asset>& value) override;

        // Updates value view
        void UpdateValueView() override;

        // Is required refresh view every time
        bool IsAlwaysRefresh() const override;

        // Called when cursor enters this object
        void OnCursorEnter(const Input::Cursor& cursor) override;

        // Called when cursor exits this object
        void OnCursorExit(const Input::Cursor& cursor) override;

        // Called when cursor pressed on this
        void OnCursorPressed(const Input::Cursor& cursor) override;

        // Called when key was pressed
        void OnKeyPressed(const Input::Key& key) override;

        // Called when some selectable listeners was dropped to this
        void OnDropped(ISelectableDragableObjectsGroup* group) override;

        // Called when some drag listeners was entered to this area
        void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

        // Called when some drag listeners was exited from this area
        void OnDragExit(ISelectableDragableObjectsGroup* group) override;
    };
}

// --- META ---

META_CLASS(Editor::AssetProperty)
META_BASE_CLASSES(Editor::AssetProperty, TPropertyField<Ref<Asset>>, DragDropArea, KeyboardEventsListener)
ATTRIBUTE(Ref<Widget>, mBox)
ATTRIBUTE(Ref<Text>, mNameText)
ATTRIBUTE(Ref<Spoiler>, mSpoiler)
ATTRIBUTE(Ref<Label>, mCaption)
ATTRIBUTE(Ref<Button>, mCreateInstanceBtn)
ATTRIBUTE(Ref<HorizontalLayout>, mHeaderContainer)
ATTRIBUTE(WeakRef<ObjectViewer>, mAssetObjectViewer)
ATTRIBUTE(const Type*, mAssetType)
ATTRIBUTE(bool, mAvailableToHaveInstance)
END_META;
```#include <memory>

template<class T>
using Ref = std::shared_ptr<T>;

template<class T>
using WeakRef = std::weak_ptr<T>;

template<class T, class... Args>
Ref<T> mmake(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<class T, class U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
    return std::dynamic_pointer_cast<T>(ptr);
}

class Editor
{
    class AssetProperty
    {
    public:
        AssetProperty(){}
        AssetProperty(const AssetProperty&){}
        const Type* GetValueType(){}
        static const Type* GetValueTypeStatic(){}
        void SetAssetId(const UID&){}
        void SetAssetType(const Type*){}
        void SetFieldInfo(const FieldInfo*){}
        void SetCaption(const WString&){}
        WString GetCaption(){}
        Button* GetRemoveButton(){}
        bool IsUnderPoint(const Vec2F&){}
    protected:
        void InitializeControls(){}
        void SetCommonAssetId(const UID&){}
        void SetAssetIdByUser(const UID&){}
        void OnCreateInstancePressed(){}
        void OnRemoveInstancePressed(){}
        void OnSaveInstancePressed(){}
        void OnTypeSpecialized(const Type&){}
        Ref<Asset> GetProxy(IAbstractValueProxy*){}
        void SetProxy(IAbstractValueProxy*, const Ref<Asset>&){}
        void UpdateValueView(){}
        bool IsAlwaysRefresh(){}
        void OnCursorEnter(const Input::Cursor&){}
        void OnCursorExit(const Input::Cursor&){}
        void OnCursorPressed(const Input::Cursor&){}
        void OnKeyPressed(const Input::Key&){}
        void OnDropped(ISelectableDragableObjectsGroup*){}
        void OnDragEnter(ISelectableDragableObjectsGroup*){}
        void OnDragExit(ISelectableDragableObjectsGroup*){}
    };
};