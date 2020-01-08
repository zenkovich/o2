#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/KeyboardEventsListener.h"

using namespace o2;

namespace Editor
{
	class FloatProperty;

	// ----------------------------
	// Editor float vector property
	// ----------------------------
	class Vec2FProperty: public IPropertyField
	{
	public:
		// Default constructor
		Vec2FProperty();

		// Copy constructor
		Vec2FProperty(const Vec2FProperty& other);

		// Copy operator
		Vec2FProperty& operator=(const Vec2FProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Reverts value to prototype value
		void Revert() override;

		// Sets value
		void SetValue(const Vec2F& value);

		// Sets value X
		void SetValueX(float value);

		// Sets value Y
		void SetValueY(float value);

		// Sets value as unknown
		void SetUnknownValue(const Vec2F& defaultValue = Vec2F());

		// Sets value X as unknown
		void SetXUnknownValue(float defaultValue = 0.0f);

		// Sets value Y as unknown
		void SetYUnknownValue(float defaultValue = 0.0f);

		// Returns value
		Vec2F GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		IOBJECT(Vec2FProperty);

	protected:
		FloatProperty* mXProperty = nullptr; // X value property
		FloatProperty* mYProperty = nullptr; // Y value property

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

	protected:
		class XValueProxy : public IValueProxy<float>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			XValueProxy();
			XValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const float& value) override;
			float GetValue() const override;
		};

		class YValueProxy : public IValueProxy<float>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			YValueProxy();
			YValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const float& value) override;
			float GetValue() const override;
		};
	};
}

CLASS_BASES_META(Editor::Vec2FProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::Vec2FProperty)
{
	PROTECTED_FIELD(mXProperty);
	PROTECTED_FIELD(mYProperty);
}
END_META;
CLASS_METHODS_META(Editor::Vec2FProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(void, SetValue, const Vec2F&);
	PUBLIC_FUNCTION(void, SetValueX, float);
	PUBLIC_FUNCTION(void, SetValueY, float);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Vec2F&);
	PUBLIC_FUNCTION(void, SetXUnknownValue, float);
	PUBLIC_FUNCTION(void, SetYUnknownValue, float);
	PUBLIC_FUNCTION(Vec2F, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetValueType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetValueTypeStatic);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
}
END_META;
