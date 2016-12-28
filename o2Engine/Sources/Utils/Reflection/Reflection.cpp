#include "Reflection.h"

#include "Utils/Data/DataNode.h"
#include "Utils/IObject.h"
#include "Utils/Math/Basis.h"
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

		auto types = mInstance->mTypes;
		for (auto type : types)
		{
			if (type->mInitializeFunc)
				type->mInitializeFunc(type);
		}
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

	const Type* Reflection::GetType(Type::Id id)
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

	Type* FundamentalTypeContainer<void>::type = new Type("void", nullptr, 0);
	Type* IObject::type = new Type("IObject", nullptr, 0);
	Type* Type::Dummy::type = new Type("Unknown", nullptr, 0);

	REG_FUNDAMENTAL_TYPE(int);
	REG_FUNDAMENTAL_TYPE(bool);
	REG_FUNDAMENTAL_TYPE(char);
	REG_FUNDAMENTAL_TYPE(wchar_t);
	REG_FUNDAMENTAL_TYPE(short int);
	REG_FUNDAMENTAL_TYPE(long int);
	REG_FUNDAMENTAL_TYPE(long long int);
	REG_FUNDAMENTAL_TYPE(unsigned char);
	REG_FUNDAMENTAL_TYPE(unsigned short int);
	REG_FUNDAMENTAL_TYPE(unsigned int);
	REG_FUNDAMENTAL_TYPE(unsigned long int);
	REG_FUNDAMENTAL_TYPE(unsigned long long int);
	REG_FUNDAMENTAL_TYPE(float);
	REG_FUNDAMENTAL_TYPE(double);
	REG_FUNDAMENTAL_TYPE(long double);
	REG_FUNDAMENTAL_TYPE(Basis);
	REG_FUNDAMENTAL_TYPE(Color4);
	REG_FUNDAMENTAL_TYPE(RectF);
	REG_FUNDAMENTAL_TYPE(RectI);
	REG_FUNDAMENTAL_TYPE(Vec2F);
	REG_FUNDAMENTAL_TYPE(Vec2I);
	REG_FUNDAMENTAL_TYPE(Vertex2);
	REG_FUNDAMENTAL_TYPE(String);
	REG_FUNDAMENTAL_TYPE(WString);
	REG_FUNDAMENTAL_TYPE(DataNode);
	REG_FUNDAMENTAL_TYPE(UID);
}