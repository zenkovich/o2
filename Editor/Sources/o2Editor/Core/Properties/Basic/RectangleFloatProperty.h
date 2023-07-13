#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace Editor
{
	class FloatProperty;

	// -------------------------------
	// Editor float rectangle property
	// -------------------------------
	class RectFProperty : public IPropertyField
	{
	public:
		// Default constructor
		RectFProperty();

		// Copy constructor
		RectFProperty(const RectFProperty& other);

		// Copy operator
		RectFProperty& operator=(const RectFProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Sets value
		void SetValue(const RectF& value);

		// Sets value left
		void SetValueLeft(float value);

		// Sets value right
		void SetValueRight(float value);

		// Sets value top
		void SetValueTop(float value);

		// Sets value bottom
		void SetValueBottom(float value);

		// Sets value as unknown
		void SetUnknownValue(const RectF& defaultValue = RectF());

		// Sets value left as unknown
		void SetLeftUnknownValue(float defaultValue = 0.0f);

		// Sets value right as unknown
		void SetRightUnknownValue(float defaultValue = 0.0f);

		// Sets value top as unknown
		void SetTopUnknownValue(float defaultValue = 0.0f);

		// Sets value bottom as unknown
		void SetBottomUnknownValue(float defaultValue = 0.0f);

		// Returns value
		RectF GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		IOBJECT(RectFProperty);

	protected:
		FloatProperty* mLeftProperty = nullptr;   // Left value property
		FloatProperty* mRightProperty = nullptr;  // Right value property
		FloatProperty* mTopProperty = nullptr;    // Top value property
		FloatProperty* mBottomProperty = nullptr; // Bottom value property

	protected:
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
// --- META ---

CLASS_BASES_META(Editor::RectFProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::RectFProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLeftProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRightProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTopProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBottomProperty);
}
END_META;
CLASS_METHODS_META(Editor::RectFProperty)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const RectFProperty&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueAndPrototypeProxy, const TargetsVec&);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValue, const RectF&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueLeft, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueRight, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueTop, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueBottom, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetUnknownValue, const RectF&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLeftUnknownValue, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetRightUnknownValue, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTopUnknownValue, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetBottomUnknownValue, float);
	FUNCTION().PUBLIC().SIGNATURE(RectF, GetCommonValue);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsValuesDifferent);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetValueType);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetValueTypeStatic);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
}
END_META;
// --- END META ---
