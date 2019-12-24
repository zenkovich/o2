#pragma once

#include "Core/Properties/IPropertyField.h"

using namespace o2;

namespace Editor
{
	class FloatProperty;

	// ----------------------------
	// Editor float border property
	// ----------------------------
	class BorderFProperty: public IPropertyField
	{
	public:
		// Default constructor
		BorderFProperty();

		// Copy constructor
		BorderFProperty(const BorderFProperty& other);

		// Copy operator
		BorderFProperty& operator=(const BorderFProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Sets value
		void SetValue(const BorderF& value);

		// Sets value left
		void SetValueLeft(float value);

		// Sets value right
		void SetValueRight(float value);

		// Sets value top
		void SetValueTop(float value);

		// Sets value bottom
		void SetValueBottom(float value);

		// Sets value as unknown
		void SetUnknownValue(const BorderF& defaultValue = BorderF());

		// Sets value left as unknown
		void SetLeftUnknownValue(float defaultValue = 0.0f);

		// Sets value right as unknown
		void SetRightUnknownValue(float defaultValue = 0.0f);

		// Sets value top as unknown
		void SetTopUnknownValue(float defaultValue = 0.0f);

		// Sets value bottom as unknown
		void SetBottomUnknownValue(float defaultValue = 0.0f);

		// Returns value
		BorderF GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		IOBJECT(BorderFProperty);

	protected:
		FloatProperty* mLeftProperty = nullptr;   // Left value property
		FloatProperty* mRightProperty = nullptr;  // Right value property
		FloatProperty* mTopProperty = nullptr;    // Top value property
		FloatProperty* mBottomProperty = nullptr; // Bottom value property

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

	protected:

		class LeftValueProxy : public IValueProxy<float>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			LeftValueProxy();
			LeftValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const float& value) override;
			float GetValue() const override;
		};

		class RightValueProxy : public IValueProxy<float>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			RightValueProxy();
			RightValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const float& value) override;
			float GetValue() const override;
		};

		class TopValueProxy : public IValueProxy<float>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			TopValueProxy();
			TopValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const float& value) override;
			float GetValue() const override;
		};

		class BottomValueProxy : public IValueProxy<float>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			BottomValueProxy();
			BottomValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const float& value) override;
			float GetValue() const override;
		};
	};
}

CLASS_BASES_META(Editor::BorderFProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::BorderFProperty)
{
	PROTECTED_FIELD(mLeftProperty);
	PROTECTED_FIELD(mRightProperty);
	PROTECTED_FIELD(mTopProperty);
	PROTECTED_FIELD(mBottomProperty);
}
END_META;
CLASS_METHODS_META(Editor::BorderFProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, SetValue, const BorderF&);
	PUBLIC_FUNCTION(void, SetValueLeft, float);
	PUBLIC_FUNCTION(void, SetValueRight, float);
	PUBLIC_FUNCTION(void, SetValueTop, float);
	PUBLIC_FUNCTION(void, SetValueBottom, float);
	PUBLIC_FUNCTION(void, SetUnknownValue, const BorderF&);
	PUBLIC_FUNCTION(void, SetLeftUnknownValue, float);
	PUBLIC_FUNCTION(void, SetRightUnknownValue, float);
	PUBLIC_FUNCTION(void, SetTopUnknownValue, float);
	PUBLIC_FUNCTION(void, SetBottomUnknownValue, float);
	PUBLIC_FUNCTION(BorderF, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetValueType);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
}
END_META;
