#include "o2/stdafx.h"
#include "Reflection.h"

#include "o2/Utils/Serialization/DataNode.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Math/Basis.h"
#include "o2/Utils/Math/Border.h"
#include "o2/Utils/Math/Color.h"
#include "o2/Utils/Math/Rect.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Math/Vertex2.h"
#include "o2/Utils/Types/String.h"
#include "o2/Utils/Types/UID.h"

namespace o2
{
	Reflection* Reflection::mInstance;

	template<>
	Type* FundamentalTypeContainer<void>::type = mnew TType<int>("void", 0);

	Type* IObject::type = mnew TType<IObject>("IObject", 0);
	Type* Type::Dummy::type = mnew TType<int>("Unknown", 0);

	DECLARE_FUNDAMENTAL_TYPE(int);
	DECLARE_FUNDAMENTAL_TYPE(bool);
	DECLARE_FUNDAMENTAL_TYPE(char);
	DECLARE_FUNDAMENTAL_TYPE(wchar_t);
	DECLARE_FUNDAMENTAL_TYPE(short int);
	DECLARE_FUNDAMENTAL_TYPE(long int);
	DECLARE_FUNDAMENTAL_TYPE(long long int);
	DECLARE_FUNDAMENTAL_TYPE(unsigned char);
	DECLARE_FUNDAMENTAL_TYPE(unsigned short int);
	DECLARE_FUNDAMENTAL_TYPE(unsigned int);
	DECLARE_FUNDAMENTAL_TYPE(unsigned long int);
	DECLARE_FUNDAMENTAL_TYPE(unsigned long long int);
	DECLARE_FUNDAMENTAL_TYPE(float);
	DECLARE_FUNDAMENTAL_TYPE(double);
	DECLARE_FUNDAMENTAL_TYPE(long double);
	DECLARE_FUNDAMENTAL_TYPE(o2::Basis);
	DECLARE_FUNDAMENTAL_TYPE(o2::Color4);
	DECLARE_FUNDAMENTAL_TYPE(o2::RectF);
	DECLARE_FUNDAMENTAL_TYPE(o2::RectI);
	DECLARE_FUNDAMENTAL_TYPE(o2::BorderF);
	DECLARE_FUNDAMENTAL_TYPE(o2::BorderI);
	DECLARE_FUNDAMENTAL_TYPE(o2::Vec2F);
	DECLARE_FUNDAMENTAL_TYPE(o2::Vec2I);
	DECLARE_FUNDAMENTAL_TYPE(o2::Vertex2);
	DECLARE_FUNDAMENTAL_TYPE(o2::String);
	DECLARE_FUNDAMENTAL_TYPE(o2::WString);
	DECLARE_FUNDAMENTAL_TYPE(o2::DataNode);
	DECLARE_FUNDAMENTAL_TYPE(o2::UID);

	Reflection::Reflection():
		mLastGivenTypeId(1)
	{
		mInstance = this;
	}

	Reflection::~Reflection()
	{
		for (auto kv : mTypes)
			delete kv.second;
	}

	Reflection& Reflection::Instance()
	{
		if (!mInstance)
			mInstance = new Reflection();

		return *mInstance;
	}

	void Reflection::InitializeTypes()
	{
		InitializeFundamentalTypes();

		ReflectionInitializationTypeProcessor processor;
		for (auto func : mInstance->mInitializingFunctions)
		{
			func(0, processor);
		}

		mInstance->mInitializingFunctions.Clear();
		mInstance->mTypesInitialized = true;
	}

	const Map<String, Type*>& Reflection::GetTypes()
	{
		return mInstance->mTypes;
	}

	void* Reflection::CreateTypeSample(const String& typeName)
	{
		if (auto type = GetType(typeName))
			return type->CreateSample();

		return nullptr;
	}

	const Type* Reflection::GetType(const String& name)
	{
		auto fnd = mInstance->mTypes.find(name);
		if (fnd != mInstance->mTypes.End())
			return fnd->second;

		if (name[name.Length() - 1] == '*')
		{
			const Type* unptrType = GetType(name.SubStr(0, name.Length() - 1));
			return unptrType->GetPointerType();
		}

		return nullptr;
	}

	bool Reflection::IsTypesInitialized()
	{
		return mInstance->mTypesInitialized;
	}

	void Reflection::InitializeFundamentalTypes()
	{
		IObject::type->mId = mInstance->mLastGivenTypeId++;
		FundamentalTypeContainer<void>::type->mId = mInstance->mLastGivenTypeId++;
		Type::Dummy::type->mId = mInstance->mLastGivenTypeId++;

		mInstance->mTypes[FundamentalTypeContainer<void>::type->GetName()] = FundamentalTypeContainer<void>::type;
		mInstance->mTypes[Type::Dummy::type->GetName()] = Type::Dummy::type;
	}
}
