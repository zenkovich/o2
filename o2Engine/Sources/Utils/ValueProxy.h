#pragma once

namespace o2
{
	class IAbstractValueProxy
	{
	public:
		virtual void SetValuePtr(void* value) = 0;
		virtual void GetValuePtr(void* value) const = 0;
	};

	template<typename _type>
	class IValueProxy: public IAbstractValueProxy
	{
	public:
		virtual void SetValue(const _type& value) = 0;
		virtual _type GetValue() const = 0;

		void SetValuePtr(void* value) override { SetValue(*(_type*)value); }
		void GetValuePtr(void* value) const { *(_type*)value = GetValue(); }
	};

	template<typename _type>
	class PointerValueProxy: public IValueProxy<_type>
	{
		_type* mValuePtr = nullptr;

	public:
		PointerValueProxy() {}
		PointerValueProxy(_type* valuePtr):mValuePtr(valuePtr) {}

		void SetValue(const _type& value) override { *mValuePtr = value; }
		_type GetValue() const override { return *mValuePtr; }
	};
}
