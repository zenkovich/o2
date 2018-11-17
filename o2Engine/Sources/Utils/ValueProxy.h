#pragma once

namespace o2
{
	class IAbstractValueProxy
	{
	public:
		virtual ~IAbstractValueProxy() {}

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
		void GetValuePtr(void* value) const override { *(_type*)value = GetValue(); }
	};

	class IVariableValueProxy
	{
	public:
		virtual void* GetValueVoidPointer() const = 0;
	};

	template<typename _type>
	class VariableValueProxy: public IValueProxy<_type>, public IVariableValueProxy
	{
	protected:
		_type* mValuePtr = nullptr;

	public:
		VariableValueProxy() {}
		VariableValueProxy(_type* valuePtr):mValuePtr(valuePtr) {}

		void SetValue(const _type& value) override { *mValuePtr = value; }
		_type GetValue() const override { return *mValuePtr; }

		_type* GetValuePointer() const { return mValuePtr; }
		void* GetValueVoidPointer() const override { return (void*)mValuePtr; }
	};

	class IVariablePointerValueProxy
	{
	public:
		virtual IAbstractValueProxy* GetUnptrValueProxy() const = 0;
		virtual void** GetValuePointerVoidPointer() const = 0;
	};

	template<typename _type>
	class VariablePointerValueProxy : public IValueProxy<_type*>, public IVariablePointerValueProxy
	{
	protected:
		_type** mValuePointerPtr = nullptr;
		IValueProxy<_type>* mUnptrValueProxy = nullptr;

	public:
		typedef _type* _typePtr;

		VariablePointerValueProxy() {}
		VariablePointerValueProxy(_type** valuePtr, IValueProxy<_type>* unptrValueProxy):
			mValuePointerPtr(valuePtr), mUnptrValueProxy(unptrValueProxy) {}

		void SetValue(const _typePtr& value) override { *mValuePointerPtr = value; }
		_type* GetValue() const override { return *mValuePointerPtr; }

		IAbstractValueProxy* GetUnptrValueProxy() const override { return mUnptrValueProxy; }

		IValueProxy<_type>* GetSpecializedUnptrValueProxy() const { return mUnptrValueProxy; }

		_type** GetValuePointerPointer() const { return mValuePointerPtr; }
		void** GetValuePointerVoidPointer() const override { return (void**)mValuePointerPtr; }
	};

	class IIObjectPointerValueProxy
	{
	public:
		virtual IObject* GetObjectPtr() const = 0;
		virtual void SetObjectPtr(IObject* object) = 0;
	};

	template<typename _type>
	class IObjectPointerValueProxy : public VariableValueProxy<_type>, public IIObjectPointerValueProxy
	{
	public:
		IObjectPointerValueProxy() :VariableValueProxy<_type>() {}
		IObjectPointerValueProxy(_type* valuePtr) :VariableValueProxy<_type>(valuePtr) {}

		IObject* GetObjectPtr() const override
		{
			return dynamic_cast<IObject*>(VariableValueProxy<_type>::mValuePtr);
		}

		void SetObjectPtr(IObject* object) override
		{
			VariableValueProxy<_type>::mValuePtr = dynamic_cast<_type*>(object);
		}
	};
}
