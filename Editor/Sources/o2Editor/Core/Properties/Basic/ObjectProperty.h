#pragma once

#include "o2/Utils/Property.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/Properties/PropertiesContext.h"

using namespace o2;

namespace o2
{
	class Spoiler;
}

namespace Editor
{
	class IObjectPropertiesViewer;

	// ----------------------
	// Editor object property
	// ----------------------
	class ObjectProperty: public IPropertyField
	{
	public:
		// Default constructor
		ObjectProperty();

		// Copy constructor
		ObjectProperty(const ObjectProperty& other);

		// Copy operator
		ObjectProperty& operator=(const ObjectProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		// Specializes field info, processing attributes
		void SetFieldInfo(const FieldInfo* fieldInfo) override;

		// Sets property caption
		void SetCaption(const WString& text) override;

		// Returns property caption
		WString GetCaption() const override;

		// Adds remove button
		Button* GetRemoveButton() override;

		// Expands property fields
		void Expand();

		// Collapses property field
		void Collapse();

		// Sets properties expanding
		void SetExpanded(bool expanded);

		// Returns is properties expanded
		bool IsExpanded() const;

		IOBJECT(ObjectProperty);

	protected:
		struct TargetObjectData
		{
			IAbstractValueProxy* proxy = nullptr;
			IObject* data = nullptr;
			bool isCreated = false;

			bool operator==(const TargetObjectData& other) const { return proxy == other.proxy; }

			void Refresh();
			void SetValue();
		};

	protected:							     								    
		Vector<Pair<TargetObjectData, TargetObjectData>> mTargetObjects; // Target objects

		IObjectPropertiesViewer* mObjectViewer = nullptr; // Object properties viewer

		bool mNoHeader = false;          // Is no header attribute exists
		bool mExpanded = false;          // True when must be expanded after creating object viewer

		WString mCaption; // Property caption

	protected:
		// It is called when property puts in buffer. Here you can release your shared resources
		void OnFreeProperty() override;

		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Checks viewer type, creates new if needed
		void CheckViewer();

		// Returns object target data from proxy. Creates copy of object when it is property proxy, or gets pointer from pointer proxy
		TargetObjectData GetObjectFromProxy(IAbstractValueProxy* proxy);

		// It is called when some property changed, sets value via proxy
		void OnPropertyChanged(const String& path, const Vector<DataDocument>& before, 
							   const Vector<DataDocument>& after);
	};
}

CLASS_BASES_META(Editor::ObjectProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::ObjectProperty)
{
	PROTECTED_FIELD(mTargetObjects);
	PROTECTED_FIELD(mObjectViewer);
	PROTECTED_FIELD(mNoHeader);
	PROTECTED_FIELD(mExpanded);
	PROTECTED_FIELD(mCaption);
}
END_META;
CLASS_METHODS_META(Editor::ObjectProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(const Type*, GetValueType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetValueTypeStatic);
	PUBLIC_FUNCTION(void, SetFieldInfo, const FieldInfo*);
	PUBLIC_FUNCTION(void, SetCaption, const WString&);
	PUBLIC_FUNCTION(WString, GetCaption);
	PUBLIC_FUNCTION(Button*, GetRemoveButton);
	PUBLIC_FUNCTION(void, Expand);
	PUBLIC_FUNCTION(void, Collapse);
	PUBLIC_FUNCTION(void, SetExpanded, bool);
	PUBLIC_FUNCTION(bool, IsExpanded);
	PROTECTED_FUNCTION(void, OnFreeProperty);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
	PROTECTED_FUNCTION(void, CheckViewer);
	PROTECTED_FUNCTION(TargetObjectData, GetObjectFromProxy, IAbstractValueProxy*);
	PROTECTED_FUNCTION(void, OnPropertyChanged, const String&, const Vector<DataDocument>&, const Vector<DataDocument>&);
}
END_META;
