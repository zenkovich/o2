#pragma once

#include "Utils/String.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Delegates.h"

namespace o2
{
	/** Reflection fields processor. Using in template function ProcessFields */
	template<typename _type, typename _processor_type>
	class IFieldProcessor
	{
	protected:
		String           mName;     /** Name of field. */
		_type&           mFieldRef; /** Field value reference. */
		_processor_type& mOwner;    /** Reference to owner fields processor. */

	public:
		/** ctor. */
		IFieldProcessor(_type& field, const String& name, _processor_type& owner);

		/** Function indicating that field is serializable. */
		virtual IFieldProcessor& Serializable();

		/** Function indicating that field could be animated. */
		virtual IFieldProcessor& Animatable(Ptr<IFunction<void()>> onUpdate = nullptr);

		/** Here You can add your own indicating functions like attributes for fields. */
	};

	/** Field info interface. */
	struct IFieldInfo
	{
		const void* mOwner; /** Pointer to owner object. */
		String      mName;  /** Field name. */

		/** ctor. */
		IFieldInfo();

		/** ctor. */
		IFieldInfo(const String& name, const void* owner);
	};

	/** Field information. */
	template<typename _type>
	struct FieldInfo: public IFieldInfo
	{
		_type& mFieldRef; /** Reference to field of object. */

		/** ctor. */
		FieldInfo();

		/** ctor. */
		FieldInfo(_type& field, const String& name, const void* owner);
	};
	typedef Vector<Ptr<IFieldInfo>> FieldsVec;

	/** Fields processor for collecting fields infos, something like simple reflection. */
	class ReflectionFieldsProcessor
	{
	public:
		/** Overridden field processor. */
		template<typename _type>
		class FieldProcessor: public IFieldProcessor<_type, ReflectionFieldsProcessor>
		{
		public:
			/** ctor. */
			FieldProcessor(_type& field, const String& name, ReflectionFieldsProcessor& owner);

			/** Function indicating that field is serializable. */
			FieldProcessor& Serializable();
		};

	private:
		const void* mOwner = nullptr; /** Fields owner object pointer. */

	public:
		FieldsVec   mFields;          /** Vector of collecting fields. */

		/** ctor. */
		ReflectionFieldsProcessor(const void* owner);

		/** Processing field function, returns FieldProcessor object, which that you can add indicating functions (aka attributes). */
		template<typename _type>
		FieldProcessor<_type> ProcessField(_type& field, const String& name);
	};

/** Basic reflection macros. */
#define FIELDS()                               \
FieldsVec GetFields()                          \
{                                              \
	ReflectionFieldsProcessor processor(this); \
	ProcessFields(processor);                  \
	return processor.mFields;                  \
}                                              \
template<typename _processor_type>             \
void ProcessFields(_processor_type& processor)

/** Smart field processing macros. */
#define FIELD(NAME) processor.ProcessField(NAME, #NAME)

/** Smart serializable field processing macros. */
#define  SERIALIZABLE_FIELD(NAME) processor.ProcessField(NAME, #NAME).Serializable()

/** Macros for processing base class fields. */
#define BASE_CLASS_FIELDS(CLASS) CLASS::ProcessFields(processor)

/** Serializable attribute function macros */
#define SERIALIZABLE .Serializable()

/** Animatable attribute function macros */
#define ANIMATABLE(FUNC) .Animatable(FUNC)

#pragma region IFieldProcessor implementation

	template<typename _type, typename _processor_type>
	IFieldProcessor<_type, _processor_type>::IFieldProcessor(_type& field, const String& name, _processor_type& owner):
		mName(name), mFieldRef(field), mOwner(owner)
	{}

	template<typename _type, typename _processor_type>
	IFieldProcessor<_type, _processor_type>& IFieldProcessor<_type, _processor_type>::Serializable()
	{
		return *this;
	}

	template<typename _type, typename _processor_type>
	IFieldProcessor<_type, _processor_type>& IFieldProcessor<_type, _processor_type>::Animatable(Ptr<IFunction<void()>> onUpdate /*= nullptr*/)
	{
		return *this;
	}

#pragma endregion IFieldProcessor implementation

#pragma region FieldInfo implementation

	template<typename _type>
	FieldInfo<_type>::FieldInfo() 
	{}

	template<typename _type>
	FieldInfo<_type>::FieldInfo(_type& field, const String& name, const void* owner):
		IFieldInfo(name, owner), mFieldRef(field)
	{}

#pragma endregion FieldInfo implementation

#pragma region ReflectionFieldsProcessor implementation
	
	template<typename _type>
	ReflectionFieldsProcessor::FieldProcessor<_type> ReflectionFieldsProcessor::ProcessField(_type& field, const String& name)
	{
		return FieldProcessor<_type>(field, name, *this);
	}

	template<typename _type>
	ReflectionFieldsProcessor::FieldProcessor<_type>& ReflectionFieldsProcessor::FieldProcessor<_type>::Serializable()
	{
		mOwner.mFields.Add(mnew FieldInfo<_type>(mFieldRef, mName, mOwner.mOwner));
		return *this;
	}

	template<typename _type>
	ReflectionFieldsProcessor::FieldProcessor<_type>::FieldProcessor(_type& field, const String& name, ReflectionFieldsProcessor& owner):
		IFieldProcessor(field, name, owner)
	{}

#pragma endregion ReflectionFieldsProcessor implementation
}