#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace Editor
{
	class IntegerProperty;

	// ---------------------------------
	// Editor integer rectangle property
	// ---------------------------------
	class RectIProperty : public IPropertyField
	{
	public:
		// Default constructor
		RectIProperty();

		// Copy constructor
		RectIProperty(const RectIProperty& other);

		// Copy operator
		RectIProperty& operator=(const RectIProperty& other);

		// Sets fields
		void SetValueAndPrototypeProxy(const TargetsVec& targets) override;

		// Updates and checks value
		void Refresh() override;

		// Sets value
		void SetValue(const RectI& value);

		// Sets value left
		void SetValueLeft(int value);

		// Sets value right
		void SetValueRight(int value);

		// Sets value top
		void SetValueTop(int value);

		// Sets value bottom
		void SetValueBottom(int value);

		// Sets value as unknown
		void SetUnknownValue(const RectI& defaultValue = RectI());

		// Sets value left as unknown
		void SetLeftUnknownValue(int defaultValue = 0);

		// Sets value right as unknown
		void SetRightUnknownValue(int defaultValue = 0);

		// Sets value top as unknown
		void SetTopUnknownValue(int defaultValue = 0);

		// Sets value bottom as unknown
		void SetBottomUnknownValue(int defaultValue = 0);

		// Returns value
		RectI GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
		const Type* GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
		static const Type* GetValueTypeStatic();

		IOBJECT(RectIProperty);

	protected:
		IntegerProperty* mLeftProperty = nullptr;   // Left value property
		IntegerProperty* mRightProperty = nullptr;  // Right value property
		IntegerProperty* mTopProperty = nullptr;    // Top value property
		IntegerProperty* mBottomProperty = nullptr; // Bottom value property

	protected:
		// Searches controls widgets and layers and initializes them
		void InitializeControls();

	protected:

		class LeftValueProxy : public IValueProxy<int>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			LeftValueProxy();
			LeftValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};

		class RightValueProxy : public IValueProxy<int>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			RightValueProxy();
			RightValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};

		class TopValueProxy : public IValueProxy<int>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			TopValueProxy();
			TopValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};

		class BottomValueProxy : public IValueProxy<int>
		{
			IAbstractValueProxy* mProxy = nullptr;

		public:
			BottomValueProxy();
			BottomValueProxy(IAbstractValueProxy* proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};
	};
}

CLASS_BASES_META(Editor::RectIProperty)
{
	BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::RectIProperty)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLeftProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mRightProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mTopProperty);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mBottomProperty);
}
END_META;
CLASS_METHODS_META(Editor::RectIProperty)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const RectIProperty&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueAndPrototypeProxy, const TargetsVec&);
	FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValue, const RectI&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueLeft, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueRight, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueTop, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetValueBottom, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetUnknownValue, const RectI&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLeftUnknownValue, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetRightUnknownValue, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTopUnknownValue, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetBottomUnknownValue, int);
	FUNCTION().PUBLIC().SIGNATURE(RectI, GetCommonValue);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsValuesDifferent);
	FUNCTION().PUBLIC().SIGNATURE(const Type*, GetValueType);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(const Type*, GetValueTypeStatic);
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
}
END_META;
