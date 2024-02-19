#pragma once

#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Utils/Editor/DragAndDrop.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

namespace o2
{
	class Component;
	class Text;
	class Widget;
}

namespace Editor
{
	class SceneHierarchyTree;
	class AssetsIconsScrollArea;

	// -----------------------------------
	// Editor actor component property box
	// -----------------------------------
	class ComponentProperty: public TPropertyField<Ref<Component>>, public KeyboardEventsListener, public DragDropArea
	{
	public:
		// Default constructor
		ComponentProperty();

		// Copy constructor
		ComponentProperty(const ComponentProperty& other);

		// Copy operator
		ComponentProperty& operator=(const ComponentProperty& other);

		// Reverts value to prototype value
		void Revert() override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(ComponentProperty);

	protected:
		const Type* mComponentType = nullptr;  // Component value type

		Ref<Widget> mBox;      // Edit box 
		Ref<Text>   mNameText; // Component name text

	protected:
		// Called when type specialized during setting value proxy
		void OnTypeSpecialized(const Type& type) override;

		// Checks is value can be reverted
		bool IsValueRevertable() const override;

		// Updates value view
		void UpdateValueView() override;

		// Called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// Called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// Called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// Called when key was pressed
		void OnKeyPressed(const Input::Key& key) override;

		// Called when some selectable listeners was dropped to this
		void OnDropped(const Ref<ISelectableDragableObjectsGroup>& group) override;

		// Called when some drag listeners was entered to this area
		void OnDragEnter(const Ref<ISelectableDragableObjectsGroup>& group) override;

		// Called when some drag listeners was exited from this area
		void OnDragExit(const Ref<ISelectableDragableObjectsGroup>& group) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Reverts target value to source
		void RevertoToPrototype(const Ref<IAbstractValueProxy>& target, const Ref<IAbstractValueProxy>& source, IObject* targetOwner);

		// Called when actors tree nodes was dragged and dropped to this
		void OnDroppedFromActorsTree(const Ref<SceneHierarchyTree>& actorsTree);

		// Called when actors tree nodes was dragged and entered to this
		void OnDragEnterFromActorsTree(const Ref<SceneHierarchyTree>& actorsTree);

		// Called when actors tree nodes was dragged and exited from this
		void OnDragExitFromActorsTree(const Ref<SceneHierarchyTree>& actorsTree);

		// Called when assets scroll icons was dragged and dropped to this
		void OnDroppedFromAssetsScroll(const Ref<AssetsIconsScrollArea>& assetsIconsScroll);

		// Called when assets scroll icons was dragged and entered to this
		void OnDragEnterFromAssetsScroll(const Ref<AssetsIconsScrollArea>& assetsIconsScroll);

		// Called when assets scroll icons was dragged and exited from this
		void OnDragExitFromAssetsScroll(const Ref<AssetsIconsScrollArea>& assetsIconsScroll);
	};
}
// --- META ---

CLASS_BASES_META(Editor::ComponentProperty)
{
    BASE_CLASS(Editor::TPropertyField<Ref<Component>>);
    BASE_CLASS(o2::KeyboardEventsListener);
    BASE_CLASS(o2::DragDropArea);
}
END_META;
CLASS_FIELDS_META(Editor::ComponentProperty)
{
    FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mComponentType);
    FIELD().PROTECTED().NAME(mBox);
    FIELD().PROTECTED().NAME(mNameText);
}
END_META;
CLASS_METHODS_META(Editor::ComponentProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const ComponentProperty&);
    FUNCTION().PUBLIC().SIGNATURE(void, Revert);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnTypeSpecialized, const Type&);
    FUNCTION().PROTECTED().SIGNATURE(bool, IsValueRevertable);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnKeyPressed, const Input::Key&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDropped, const Ref<ISelectableDragableObjectsGroup>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnter, const Ref<ISelectableDragableObjectsGroup>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragExit, const Ref<ISelectableDragableObjectsGroup>&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, RevertoToPrototype, const Ref<IAbstractValueProxy>&, const Ref<IAbstractValueProxy>&, IObject*);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDroppedFromActorsTree, const Ref<SceneHierarchyTree>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnterFromActorsTree, const Ref<SceneHierarchyTree>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragExitFromActorsTree, const Ref<SceneHierarchyTree>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDroppedFromAssetsScroll, const Ref<AssetsIconsScrollArea>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnterFromAssetsScroll, const Ref<AssetsIconsScrollArea>&);
    FUNCTION().PROTECTED().SIGNATURE(void, OnDragExitFromAssetsScroll, const Ref<AssetsIconsScrollArea>&);
}
END_META;
// --- END META ---
