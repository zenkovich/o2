#include "Reflection.h"

#include "Utils/Data/DataNode.h"
#include "Utils/IObject.h"
#include "Utils/Math/Basis.h"
#include "Utils/Math/Border.h"
#include "Utils/Math/Color.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/String.h"
#include "Utils/UID.h"

namespace o2
{
	Reflection::Reflection():
		mLastGivenTypeId(1)
	{
		mInstance = this;
	}

	Reflection::~Reflection()
	{
		for (auto type : mTypes)
			delete type;
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

	const Vector<Type*>& Reflection::GetTypes()
	{
		return mInstance->mTypes;
	}

	void* Reflection::CreateTypeSample(const String& typeName)
	{
		for (auto type : mInstance->mTypes)
		{
			if (type->GetName() == typeName)
				return type->CreateSample();
		}

		return nullptr;
	}

	const Type* Reflection::GetType(TypeId id)
	{
		for (auto type : mInstance->mTypes)
		{
			if (type->ID() == id)
				return type;
		}

		return nullptr;
	}

	const Type* Reflection::GetType(const String& name)
	{
		for (auto type : mInstance->mTypes)
		{
			if (type->GetName() == name)
				return type;
		}

		if (name[name.Length() - 1] == '*')
		{
			const Type* unptrType = GetType(name.SubStr(0, name.Length() - 1));
			return InitializePointerType(unptrType);
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

		mInstance->mTypes.Add(FundamentalTypeContainer<void>::type);
		mInstance->mTypes.Add(Type::Dummy::type);
	}

	const Type* Reflection::InitializePointerType(const Type* type)
	{
		if (type->mPtrType)
			return type->mPtrType;

		PointerType* newType = new PointerType(type);
		newType->mId = mInstance->mLastGivenTypeId++;

		type->mPtrType = newType;

		mInstance->mTypes.Add(newType);

		return newType;
	}
	
	Reflection* Reflection::mInstance;

	Type* FundamentalTypeContainer<void>::type = new Type("void", nullptr, 0, &Reflection::NoCastFunc);
	Type* IObject::type = new Type("IObject", nullptr, 0, &Reflection::NoCastFunc);
	Type* Type::Dummy::type = new Type("Unknown", nullptr, 0, &Reflection::NoCastFunc);

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
	DECLARE_FUNDAMENTAL_TYPE(Basis);
	DECLARE_FUNDAMENTAL_TYPE(Color4);
	DECLARE_FUNDAMENTAL_TYPE(RectF);
	DECLARE_FUNDAMENTAL_TYPE(RectI);
	DECLARE_FUNDAMENTAL_TYPE(BorderF);
	DECLARE_FUNDAMENTAL_TYPE(BorderI);
	DECLARE_FUNDAMENTAL_TYPE(Vec2F);
	DECLARE_FUNDAMENTAL_TYPE(Vec2I);
	DECLARE_FUNDAMENTAL_TYPE(Vertex2);
	DECLARE_FUNDAMENTAL_TYPE(String);
	DECLARE_FUNDAMENTAL_TYPE(WString);
	DECLARE_FUNDAMENTAL_TYPE(DataNode);
	DECLARE_FUNDAMENTAL_TYPE(UID);
}