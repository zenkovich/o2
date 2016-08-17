#include "Reflection.h"

#include "Utils/Data/DataNode.h"
#include "Utils/IObject.h"
#include "Utils/Math/Basis.h"
#include "Utils/Math/Color.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/String.h"

namespace o2
{
	Reflection::Reflection():
		mLastGivenTypeId(1)
	{
		mInstance = this;
		InitializeFundamentalTypes();
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

	const Vector<Type*>& Reflection::GetTypes()
	{
		return mInstance->mTypes;
	}

	void* Reflection::CreateTypeSample(const String& typeName)
	{
		for (auto type : mInstance->mTypes)
		{
			if (type->Name() == typeName)
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
			if (type->Name() == name)
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
		FundamentalType<void>::type.mName = typeid(void).name();
		FundamentalType<void>::type.mId = mInstance->mLastGivenTypeId++;
		FundamentalType<void>::type.mSampleCreator = new Type::SampleCreator<int>();

		Type::Dummy::type.mName = "Unknown";
		Type::Dummy::type.mId = mInstance->mLastGivenTypeId++;
		Type::Dummy::type.mSampleCreator = new Type::SampleCreator<int>();

		mInstance->mTypes.Add(&FundamentalType<void>::type);
	}

	const Type* Reflection::InitializePointerType(const Type* type)
	{
		if (type->mPtrType)
			return type->mPtrType;

		Type* newType = new Type(type->mName + "*");
		newType->mId = mInstance->mLastGivenTypeId++;
		newType->mSampleCreator = new Type::SampleCreator<void*>();
		newType->mSize = sizeof(void*);
		newType->mPointer = type->mPointer + 1;

		type->mPtrType = newType;
		newType->mUnptrType = const_cast<Type*>(type);

		mInstance->mTypes.Add(newType);

		return newType;
	}

	Type IObject::type("o2::IObject");
	Type FundamentalType<int>::type("int");
	Type FundamentalType<bool>::type("bool");
	Type FundamentalType<char>::type("char");
	Type FundamentalType<wchar_t>::type("wchar_t");
	Type FundamentalType<short int>::type("short int");
	Type FundamentalType<long int>::type("long int");
	Type FundamentalType<long long int>::type("long long int");
	Type FundamentalType<unsigned char>::type("unsigned char");
	Type FundamentalType<unsigned short int>::type("unsigned short int");
	Type FundamentalType<unsigned int>::type("unsigned int");
	Type FundamentalType<unsigned long int>::type("unsigned long int");
	Type FundamentalType<unsigned long long int>::type("unsigned long long int");
	Type FundamentalType<float>::type("float");
	Type FundamentalType<double>::type("double");
	Type FundamentalType<long double>::type("long double");
	Type FundamentalType<void>::type("void");
	Type FundamentalType<float const>::type("float const");
	Type FundamentalType<int const>::type("int const");
	Type FundamentalType<unsigned int const>::type("unsigned int const");
	Type FundamentalType<Basis>::type("o2::Basis");
	Type FundamentalType<Color4>::type("o2::Color4");
	Type FundamentalType<RectF>::type("o2::RectF");
	Type FundamentalType<RectI>::type("o2::RectI");
	Type FundamentalType<Vec2F>::type("o2::Vec2F");
	Type FundamentalType<Vec2I>::type("o2::Vec2I");
	Type FundamentalType<Vertex2>::type("o2::Vertex2");
	Type FundamentalType<String>::type("o2::String");
	Type FundamentalType<WString>::type("o2::WString");
	Type FundamentalType<DataNode>::type("o2::DataNode");
}