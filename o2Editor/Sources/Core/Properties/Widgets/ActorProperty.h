#pragma once

#include "Core/Properties/IPropertyField.h"
#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "Utils/Editor/DragAndDrop.h"

namespace o2
{
	class Actor;
	class Text;
	class UIWidget;
}

namespace Editor
{
	class UIActorsTree;
	class UIAssetsIconsScrollArea;

	// ---------------------
	// Editor actor property
	// ---------------------
	class ActorProperty: public TPropertyField<Actor*>, public KeyboardEventsListener, public DragDropArea
	{
	public:
		// Default constructor
		ActorProperty();

		// Copy constructor
		ActorProperty(const ActorProperty& other);

		// Copy operator
		ActorProperty& operator=(const ActorProperty& other);

		// Reverts value to prototype value
		void Revert() override;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		IOBJECT(ActorProperty);

	protected:
		UIWidget* mBox = nullptr;      // Property edit box
		Text*     mNameText = nullptr; // Asset name text

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Checks is value can be reverted
		bool IsValueRevertable() const override;

		// Updates value view
		void UpdateValueView() override;

		// Reverts target value to source
		void RevertoToPrototype(IAbstractValueProxy* target, IAbstractValueProxy* source, IObject* targetOwner);

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

		// It is called when actors tree nodes was dragged and dropped to this
		void OnDroppedFromActorsTree(UIActorsTree* actorsTree);

		// It is called when actors tree nodes was dragged and entered to this
		void OnDragEnterFromActorsTree(UIActorsTree* actorsTree);

		// It is called when actors tree nodes was dragged and exited from this
		void OnDragExitFromActorsTree(UIActorsTree* actorsTree);

		// It is called when assets scroll icons was dragged and dropped to this
		void OnDroppedFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);

		// It is called when assets scroll icons was dragged and entered to this
		void OnDragEnterFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);

		// It is called when assets scroll icons was dragged and exited from this
		void OnDragExitFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);
	};
}

CLASS_BASES_META(Editor::ActorProperty)
{
	BASE_CLASS(Editor::TPropertyField<Actor*>);
	BASE_CLASS(o2::KeyboardEventsListener);
	BASE_CLASS(o2::DragDropArea);
}
END_META;
CLASS_FIELDS_META(Editor::ActorProperty)
{
	PROTECTED_FIELD(mBox);
	PROTECTED_FIELD(mNameText);
}
END_META;
CLASS_METHODS_META(Editor::ActorProperty)
{

	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(bool, IsValueRevertable);
	PROTECTED_FUNCTION(void, UpdateValueView);
	PROTECTED_FUNCTION(void, RevertoToPrototype, IAbstractValueProxy*, IAbstractValueProxy*, IObject*);
	PROTECTED_FUNCTION(void, OnCursorEnter, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorExit, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnCursorPressed, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyPressed, const Input::Key&);
	PROTECTED_FUNCTION(void, OnDropped, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragEnter, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDragExit, ISelectableDragableObjectsGroup*);
	PROTECTED_FUNCTION(void, OnDroppedFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDragEnterFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDragExitFromActorsTree, UIActorsTree*);
	PROTECTED_FUNCTION(void, OnDroppedFromAssetsScroll, UIAssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, OnDragEnterFromAssetsScroll, UIAssetsIconsScrollArea*);
	PROTECTED_FUNCTION(void, OnDragExitFromAssetsScroll, UIAssetsIconsScrollArea*);
}
END_META;
