#pragma once

#include "Core/Properties/IPropertyField.h"
#include "Events/CursorEventsArea.h"
#include "Events/KeyboardEventsListener.h"

using namespace o2;

namespace Editor
{
	class IntegerProperty;

	// ----------------------------
	// Editor int vector property
	// ----------------------------
	class Vec2IProperty : public IPropertyField
	{
	public:
		// Default constructor
		Vec2IProperty();

		// Copy constructor
		Vec2IProperty(const Vec2IProperty& other);

		// Copy operator
		Vec2IProperty& operator=(const Vec2IProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Sets value
		void SetValue(const Vec2I& value);

		// Sets value X
		void SetValueX(int value);

		// Sets value Y
		void SetValueY(int value);

		// Sets value as unknown
		void SetUnknownValue(const Vec2I& defaultValue = Vec2I());

		// Sets value X as unknown
		void SetXUnknownValue(int defaultValue = 0.0f);

		// Sets value Y as unknown
		void SetYUnknownValue(int defaultValue = 0.0f);

		// Returns value
		Vec2I GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		IOBJECT(Vec2IProperty);

	protected:
		IntegerProperty* mXProperty = nullptr; // X value property
		IntegerProperty* mYProperty = nullptr; // Y value property

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

	protected:
		class XValueProxy : public IValueProxy<int>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			XValueProxy();
			XValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};

		class YValueProxy : public IValueProxy<int>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			YValueProxy();
			YValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};
	};
}

CLASS_BASES_META(Editor::Vec2IProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::Vec2IProperty)
{
	PROTECTED_FIELD(mXProperty);
	PROTECTED_FIELD(mYProperty);
}
END_META;
CLASS_METHODS_META(Editor::Vec2IProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(void, SetValue, const Vec2I&);
	PUBLIC_FUNCTION(void, SetValueX, int);
	PUBLIC_FUNCTION(void, SetValueY, int);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Vec2I&);
	PUBLIC_FUNCTION(void, SetXUnknownValue, int);
	PUBLIC_FUNCTION(void, SetYUnknownValue, int);
	PUBLIC_FUNCTION(Vec2I, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetValueType);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
}
END_META;
