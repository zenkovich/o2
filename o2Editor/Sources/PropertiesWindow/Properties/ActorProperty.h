#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/DragAndDrop.h"

namespace o2
{
	class UIWidget;
	class Text;
	class Actor;
}

namespace Editor
{
	class UIActorsTree;
	class UIAssetsIconsScrollArea;

	// ---------------------
	// Editor actor property
	// ---------------------
	class ActorProperty: public IPropertyField, public KeyboardEventsListener, public DragDropArea
	{
	public:
		// Default constructor
		ActorProperty(UIWidget* widget = nullptr);

		// Destructor
		~ActorProperty();

		// Sets target fields
		void Setup(const Vector<void*>& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Returns root widget
		UIWidget* GetWidget() const;

		// Returns value
		Actor* GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const;

		// Sets value actor
		void SetValue(Actor* value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		IOBJECT(ActorProperty);

	protected:
		Function<void(void*, Actor*)> mAssignFunc; // Value assign function
		Function<Actor*(void*)>       mGetFunc;    // Get value function

		Vector<void*> mValuesPointers;        // Fields' pointers
		Actor*        mCommonValue = nullptr; // Common field value (if not different)
		bool          mValuesDifferent;       // Are values different

		UIWidget*     mBox;             // Edit box 
		Text*         mNameText;        // Asset name text

	protected:
		// Sets common value actor
		void SetCommonValue(Actor* value);

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when key was pressed
		void OnKeyPressed(const Input::Key& key);

		// Calls when some selectable listeners was dropped to this
		void OnDropped(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was entered to this area
		void OnDragEnter(ISelectableDragableObjectsGroup* group);

		// Calls when some drag listeners was exited from this area
		void OnDragExit(ISelectableDragableObjectsGroup* group);

		// Calls when actors tree nodes was dragged and dropped to this
		void OnDroppedFromActorsTree(UIActorsTree* actorsTree);

		// Calls when actors tree nodes was dragged and entered to this
		void OnDragEnterFromActorsTree(UIActorsTree* actorsTree);

		// Calls when actors tree nodes was dragged and exited from this
		void OnDragExitFromActorsTree(UIActorsTree* actorsTree);

		// Calls when assets scroll icons was dragged and dropped to this
		void OnDroppedFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);

		// Calls when assets scroll icons was dragged and entered to this
		void OnDragEnterFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);

		// Calls when assets scroll icons was dragged and exited from this
		void OnDragExitFromAssetsScroll(UIAssetsIconsScrollArea* assetsIconsScroll);
	};
}