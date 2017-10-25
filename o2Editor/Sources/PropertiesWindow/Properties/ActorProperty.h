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
		void SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Returns root widget
		UIWidget* GetWidget() const override;

		// Returns value
		Actor* GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetFieldType() const override;

		// Sets value actor
		void SetValue(Actor* value);

		// Sets value as unknown
		void SetUnknownValue();

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

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

		// Sets value actor, checks value changed, calls onChangeCompleted
		void SetValueByUser(Actor* value);

		// Checks that value was changed and calls onChangeCompleted
		void CheckValueChangeCompleted();

		// Stores values to data
		void StoreValues(Vector<DataNode>& data) const;
	};
}
