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
	class SceneTree;
	class AssetsIconsScrollArea;

	// -----------------------------------
	// Editor actor component property box
	// -----------------------------------
	class ComponentProperty: public TPropertyField<ComponentRef>, public KeyboardEventsListener, public DragDropArea
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

		Widget* mBox = nullptr;            // Edit box 
		Text*   mNameText = nullptr;       // Component name text

	protected:
		// It is called when type specialized during setting value proxy
		void OnTypeSpecialized(const Type& type) override;

		// Checks is value can be reverted
		bool IsValueRevertable() const override;

		// Updates value view
		void UpdateValueView() override;

		// It is called when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor) override;

		// It is called when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor) override;

		// It is called when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor) override;

		// It is called when key was pressed
		void OnKeyPressed(const Input::Key& key) override;

		// It is called when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group) override;

		// It is called when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Reverts target value to source
		void RevertoToPrototype(IAbstractValueProxy* target, IAbstractValueProxy* source, IObject* targetOwner);

		// It is called when actors tree nodes was dragged and dropped to this
		void OnDroppedFromActorsTree(SceneTree* actorsTree);

		// It is called when actors tree nodes was dragged and entered to this
		void OnDragEnterFromActorsTree(SceneTree* actorsTree);

		// It is called when actors tree nodes was dragged and exited from this
		void OnDragExitFromActorsTree(SceneTree* actorsTree);

		// It is called when assets scroll icons was dragged and dropped to this
		void OnDroppedFromAssetsScroll(AssetsIconsScrollArea* assetsIconsScroll);

		// It is called when assets scroll icons was dragged and entered to this
		void OnDragEnterFromAssetsScroll(AssetsIconsScrollArea* assetsIconsScroll);

		// It is called when assets scroll icons was dragged and exited from this
		void OnDragExitFromAssetsScroll(AssetsIconsScrollArea* assetsIconsScroll);
	};
}

CLASS_BASES_META(Editor::ComponentProperty)
{
	BASE_CLASS(Editor::TPropertyField<ComponentRef>);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::DragDropArea);
}
END_META;
CLASS_FIELDS_META(Editor::ComponentProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mComponentType);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBox);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mNameText);
}
END_META;
CLASS_METHODS_META(Editor::ComponentProperty)
{

	FUNCTION().PUBLIC().SIGNATURE(void, Revert);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTypeSpecialized, const Type&);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsValueRevertable);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorEnter, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorExit, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnCursorPressed, const Input::Cursor&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnKeyPressed, const Input::Key&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDropped, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragExit, ISelectableDragableObjectsGroup*);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
	FUNCTION().PROTECTED().SIGNATURE(void, RevertoToPrototype, IAbstractValueProxy*, IAbstractValueProxy*, IObject*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDroppedFromActorsTree, SceneTree*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnterFromActorsTree, SceneTree*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragExitFromActorsTree, SceneTree*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDroppedFromAssetsScroll, AssetsIconsScrollArea*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragEnterFromAssetsScroll, AssetsIconsScrollArea*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDragExitFromAssetsScroll, AssetsIconsScrollArea*);
}
END_META;
