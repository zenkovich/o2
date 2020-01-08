#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2/Events/DrawableCursorEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Utils/Editor/DragAndDrop.h"

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
	class ComponentProperty: public TPropertyField<Component*>, public KeyboardEventsListener, public DragDropArea
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

		// Specializes field type
		void SpecializeType(const Type* type);

		// Returns specialized type
		const Type* GetSpecializedType() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(ComponentProperty);

	protected:
		const Type* mComponentType = nullptr;  // Component value type

		Widget* mBox = nullptr;            // Edit box 
		Text*   mNameText = nullptr;       // Component name text

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

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
	BASE_CLASS(Editor::TPropertyField<Component*>);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::DragDropArea);
}
END_META;
CLASS_FIELDS_META(Editor::ComponentProperty)
{
	PROTECTED_FIELD(mComponentType);
	PROTECTED_FIELD(mBox);
	PROTECTED_FIELD(mNameText);
}
END_META;
CLASS_METHODS_META(Editor::ComponentProperty)
{

	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PUBLIC_FUNCTION(const Type*, GetSpecializedType);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(bool, IsValueRevertable);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, RevertoToPrototype, IAbstractValueProxy*, IAbstractValueProxy*, IObject*);
	PROTECTED_FUNCTION(void, OnDroppedFromActorsTree, SceneTree*);
	PROTECTED_FUNCTION(void, OnDragEnterFromActorsTree, SceneTree*);
	PROTECTED_FUNCTION(void, OnDragExitFromActorsTree, SceneTree*);
	PROTECTED_FUNCTION(void, OnDroppedFromAssetsScroll, AssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, OnDragEnterFromAssetsScroll, AssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, OnDragExitFromAssetsScroll, AssetsIconsScrollArea*);
}
END_META;
