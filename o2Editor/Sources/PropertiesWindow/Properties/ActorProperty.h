#pragma once

#include "Events/DrawableCursorEventsListener.h"
#include "Events/KeyboardEventsListener.h"
#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/DragAndDrop.h"

namespace o2
{
	class Actor;
	class Text;
	class UIButton;
	class UIWidget;
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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty);

		// Updates and checks value
		void Refresh();

		// Reverts value to prototype value
		void Revert();

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

		TargetsVec mValuesPointers;           // Fields' pointers
		Actor*     mCommonValue = nullptr;    // Common field value (if not different)
		bool       mValuesDifferent = true;   // Are values different

		UIWidget*  mPropertyWidget = nullptr; // Property widget
		UIButton*  mRevertBtn = nullptr;      // Revert to source prototype button
		UIWidget*  mBox = nullptr;            // Property edit box
		Text*      mNameText = nullptr;       // Asset name text

	protected:
		// Sets common value actor
		void SetCommonValue(Actor* value);

		// Checks value for reverting to prototype
		void CheckRevertableState();

		// Reverts target value to source
		void RevertoToPrototype(void* target, void* source, IObject* targetOwner);

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

		// Sets value actor, checks value changed, calls onChangeCompleted
		void SetValueByUser(Actor* value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}
