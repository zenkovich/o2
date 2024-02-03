#pragma once

#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2Editor/Core/SmartPointers/Ref.h"
#include "o2Editor/Core/SmartPointers/WeakRef.h"

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
        Ref<RectI> GetCommonValue() const;

		// Returns is values different
		bool IsValuesDifferent() const;

		// Returns editing by this field type
        const Ref<Type> GetValueType() const override;

		// Returns editing by this field type by static function, can't be changed during runtime
        static const Ref<Type> GetValueTypeStatic();

		IOBJECT(RectIProperty);

	protected:
		Ref<IntegerProperty> mLeftProperty = nullptr;   // Left value property
		Ref<IntegerProperty> mRightProperty = nullptr;  // Right value property
		Ref<IntegerProperty> mTopProperty = nullptr;    // Top value property
		Ref<IntegerProperty> mBottomProperty = nullptr; // Bottom value property

	protected:
		// Searches controls widgets and layers and initializes them
		void InitializeControls();

	protected:

		class LeftValueProxy : public IValueProxy<int>
		{
			WeakRef<IAbstractValueProxy> mProxy = nullptr;

		public:
			LeftValueProxy();
			LeftValueProxy(WeakRef<IAbstractValueProxy> proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};

		class RightValueProxy : public IValueProxy<int>
		{
			WeakRef<IAbstractValueProxy> mProxy = nullptr;

		public:
			RightValueProxy();
			RightValueProxy(WeakRef<IAbstractValueProxy> proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};

		class TopValueProxy : public IValueProxy<int>
		{
			WeakRef<IAbstractValueProxy> mProxy = nullptr;

		public:
			TopValueProxy();
			TopValueProxy(WeakRef<IAbstractValueProxy> proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};

		class BottomValueProxy : public IValueProxy<int>
		{
			WeakRef<IAbstractValueProxy> mProxy = nullptr;

		public:
			BottomValueProxy();
			BottomValueProxy(WeakRef<IAbstractValueProxy> proxy);

			void SetValue(const int& value) override;
			int GetValue() const override;
		};
	};
}

// --- META ---

CLASS_BASES_META(Editor::RectIProperty)
{
    BASE_CLASS(Editor::IPropertyField);
}
END_META;
CLASS_FIELDS_META(Editor::RectIProperty)
{
    FIELD().PROTECTED().NAME(mLeftProperty);
    FIELD().PROTECTED().NAME(mRightProperty);
    FIELD().PROTECTED().NAME(mTopProperty);
    FIELD().PROTECTED().NAME(mBottomProperty);
}
END_META;
CLASS_METHODS_META(Editor::RectIProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const RectIProperty&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetValueAndPrototypeProxy, const TargetsVec&);
    FUNCTION().PUBLIC().SIGNATURE(void, Refresh);
    FUNCTION().PUBLIC().SIGNATURE(void, SetValue, const Ref<RectI>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetValueLeft, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SetValueRight, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SetValueTop, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SetValueBottom, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SetUnknownValue, const Ref<RectI>&);
    FUNCTION().PUBLIC().SIGNATURE(void, SetLeftUnknownValue, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SetRightUnknownValue, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SetTopUnknownValue, int);
    FUNCTION().PUBLIC().SIGNATURE(void, SetBottomUnknownValue, int);
    FUNCTION().PUBLIC().SIGNATURE(Ref<RectI>, GetCommonValue);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsValuesDifferent);
    FUNCTION().PUBLIC().SIGNATURE(const Ref<Type>, GetValueType);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(const Ref<Type>, GetValueTypeStatic);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
}
END_META;
// --- END META ---