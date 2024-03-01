#pragma once

#include "o2/Utils/Function/Function.h"

#if IS_SCRIPTING_SUPPORTED
#include "o2/Scripts/ScriptValueDef.h"
#endif

namespace o2
{
	class Type;

	// ----------------------------------------------------------------------------
	// Abstract value proxy. Accepts by void*, value by pointer must match the type
	// ----------------------------------------------------------------------------
	class IAbstractValueProxy : virtual public RefCounterable
	{
	public:
		virtual ~IAbstractValueProxy() {}

		virtual void SetValuePtr(void* value) = 0;
		virtual void GetValuePtr(void* value) const = 0;
		virtual const Type& GetType() const = 0;
	};

	// -----------------
	// Typed value proxy
	// -----------------
	template<typename _type>
	class IValueProxy : public IAbstractValueProxy
	{
	public:
		virtual void SetValue(const _type& value) = 0;
		virtual _type GetValue() const = 0;

		void SetValuePtr(void* value) override;
		void GetValuePtr(void* value) const override;
		const Type& GetType() const override;
	};

	// ------------------------------------------------------------
	// Pointer value proxy interface. Can returns to source pointer
	// ------------------------------------------------------------
	class IPointerValueProxy : virtual public RefCounterable
	{
	public:
		virtual void* GetValueVoidPointer() const = 0;
	};

	// ------------------------------------------------------------------------
	// Typed pointer value proxy. Stores the pointer to value and works with it
	// ------------------------------------------------------------------------
	template<typename _type>
	class PointerValueProxy : public IValueProxy<_type>, public IPointerValueProxy
	{
	protected:
		_type* mValuePtr = nullptr;

	public:
		PointerValueProxy() {}
		PointerValueProxy(_type* valuePtr) :mValuePtr(valuePtr) {}

		void SetValue(const _type& value) override { *mValuePtr = value; }
		_type GetValue() const override { return *mValuePtr; }

		_type* GetValuePointer() const { return mValuePtr; }
		void* GetValueVoidPointer() const override { return (void*)mValuePtr; }
	};

	// ----------------------------------------------------
	// Reference pointer value proxy. Works with Ref<_type>
	// ----------------------------------------------------
	class IRefPointerValueProxy
	{
	public:
		virtual void* GetRefValueVoidPtr() const = 0;
		virtual void SetRefValuePtr(void* value) = 0;
	};

	// ----------------------------------------------------
	// Reference pointer value proxy. Works with Ref<_type>
	// ----------------------------------------------------
	template<typename _type>
	class RefPointerValueProxy : public PointerValueProxy<Ref<_type>>, public IRefPointerValueProxy
	{
	public:
		using Base = PointerValueProxy<Ref<_type>>;

	public:
		RefPointerValueProxy() {}
		RefPointerValueProxy(Ref<_type>* valuePtr) :Base(valuePtr) {}

		void* GetRefValueVoidPtr() const override { return Base::GetValuePointer()->Get(); }
		void SetRefValuePtr(void* value) override { (*Base::GetValuePointer()) = Ref((_type*)value); }
	};


	// ----------------------------------------------------
	// Functional proxy, uses function to set and get value
	// ----------------------------------------------------
	template<typename _type>
	class FunctionalValueProxy : public IValueProxy<_type>
	{
	protected:
		Function<void(_type)> mSetter;
		Function<_type()>     mGetter;

	public:
		FunctionalValueProxy() {}
		FunctionalValueProxy(const Function<void(_type)> setter, const Function<_type()> getter) :
			mSetter(setter), mGetter(getter)
		{}

		void SetValue(const _type& value) override { mSetter(value); }
		_type GetValue() const override { return mGetter(); }
	};

	// -------------------------------------------------------------------------
	// Property proxy, uses setter and getter from property to set and get value
	// -------------------------------------------------------------------------
	template<typename _type, typename _property_type>
	class PropertyValueProxy : public IValueProxy<_type>
	{
		_property_type* mProperty;

	public:
		PropertyValueProxy() {}
		PropertyValueProxy(_property_type* ptr) :mProperty(ptr) {}

		bool operator==(const PropertyValueProxy<_type, _property_type>& other) const { return mProperty == other.mProperty; }

		void SetValue(const _type& value) override { mProperty->Set(value); }
		_type GetValue() const override { return mProperty->Get(); }
	};


#if IS_SCRIPTING_SUPPORTED
	// --------------------------
	// Unified script value proxy
	// --------------------------
	class ScriptValueProxy : public IAbstractValueProxy
	{
	public:
		IScriptValueProperty* scriptProperty = nullptr;

	public:
		ScriptValueProxy();
		ScriptValueProxy(IScriptValueProperty* prop);
		~ScriptValueProxy() override;

		void SetValuePtr(void* value) override;
		void GetValuePtr(void* value) const override;
		const Type& GetType() const override;
	};

	// ------------------------
	// Script value typed proxy
	// ------------------------
	template<typename _type>
	class TypeScriptValueProxy : public IValueProxy<_type>, public ScriptValueProperty
	{
	public:
		IScriptValueProperty* scriptProperty = nullptr;

	public:
		TypeScriptValueProxy();
		TypeScriptValueProxy(IScriptValueProperty* prop);
		~TypeScriptValueProxy() override;

		void SetValue(const _type& value) override;
		_type GetValue() const override;
	};

	// ----------------------------------------
	// Pointer script value proxy, using _type*
	// ----------------------------------------
	template<typename _type>
	class PtrScriptValueProxy : public IValueProxy<_type>, public ScriptValueProperty
	{
	public:
		IScriptValueProperty* scriptProperty = nullptr;

	public:
		PtrScriptValueProxy();
		PtrScriptValueProxy(IScriptValueProperty* prop);
		~PtrScriptValueProxy() override;

		void SetValue(const _type& value) override;
		_type GetValue() const override;

		const Type& GetType() const override;
	};
#endif
}

#include "o2/Utils/Reflection/Type.h"
#if IS_SCRIPTING_SUPPORTED
#include "o2/Scripts/ScriptValue.h"
#endif

namespace o2
{
	template<typename _type>
	void IValueProxy<_type>::SetValuePtr(void* value)
	{
		SetValue(*(_type*)value);
	}

	template<typename _type>
	void IValueProxy<_type>::GetValuePtr(void* value) const
	{
		*(_type*)value = GetValue();
	}

	template<typename _type>
	const Type& IValueProxy<_type>::GetType() const
	{
		return TypeOf(_type);
	}

#if IS_SCRIPTING_SUPPORTED
	template<typename _type>
	TypeScriptValueProxy<_type>::TypeScriptValueProxy()
	{}

	template<typename _type>
	TypeScriptValueProxy<_type>::TypeScriptValueProxy(IScriptValueProperty* prop)
		:scriptProperty(prop)
	{}

	template<typename _type>
	TypeScriptValueProxy<_type>::~TypeScriptValueProxy()
	{
		if (scriptProperty)
			delete scriptProperty;
	}

	template<typename _type>
	void TypeScriptValueProxy<_type>::SetValue(const _type& value)
	{
		scriptProperty->Set(ScriptValue(value));
	}

	template<typename _type>
	_type TypeScriptValueProxy<_type>::GetValue() const
	{
		return scriptProperty->Get().GetValue<_type>();
	}

	template<typename _type>
	PtrScriptValueProxy<_type>::PtrScriptValueProxy()
	{}

	template<typename _type>
	PtrScriptValueProxy<_type>::PtrScriptValueProxy(IScriptValueProperty* prop)
		:scriptProperty(prop)
	{}

	template<typename _type>
	PtrScriptValueProxy<_type>::~PtrScriptValueProxy()
	{
		if (scriptProperty)
			delete scriptProperty;
	}

	template<typename _type>
	void PtrScriptValueProxy<_type>::SetValue(const _type& value)
	{
		scriptProperty->Set(ScriptValue(mnew _type(value)));
	}

	template<typename _type>
	_type PtrScriptValueProxy<_type>::GetValue() const
	{
		return *scriptProperty->Get().GetValue<_type*>();
	}

	template<typename _type>
	const Type& PtrScriptValueProxy<_type>::GetType() const
	{
		if constexpr (std::is_base_of<IObject, _type>::value)
			return scriptProperty->Get().GetValue<_type*>()->GetType();
		else
			return TypeOf(_type);
	}
#endif
}
