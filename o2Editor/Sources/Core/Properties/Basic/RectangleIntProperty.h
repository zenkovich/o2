#pragma once

#include "Core/Properties/IPropertyField.h"

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
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

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
	PROTECTED_FIELD(mLeftProperty);
	PROTECTED_FIELD(mRightProperty);
	PROTECTED_FIELD(mTopProperty);
	PROTECTED_FIELD(mBottomProperty);
}
END_META;
CLASS_METHODS_META(Editor::RectIProperty)
{

	PUBLIC_FUNCTION(void, SetValueAndPrototypeProxy, const TargetsVec&);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, SetValue, const RectI&);
	PUBLIC_FUNCTION(void, SetValueLeft, int);
	PUBLIC_FUNCTION(void, SetValueRight, int);
	PUBLIC_FUNCTION(void, SetValueTop, int);
	PUBLIC_FUNCTION(void, SetValueBottom, int);
	PUBLIC_FUNCTION(void, SetUnknownValue, const RectI&);
	PUBLIC_FUNCTION(void, SetLeftUnknownValue, int);
	PUBLIC_FUNCTION(void, SetRightUnknownValue, int);
	PUBLIC_FUNCTION(void, SetTopUnknownValue, int);
	PUBLIC_FUNCTION(void, SetBottomUnknownValue, int);
	PUBLIC_FUNCTION(RectI, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetValueType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetValueTypeStatic);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, InitializeControls);
}
END_META;
