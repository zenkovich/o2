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
		return *mInstance;
	}

	void Reflection::Initialize()
	{
		mInstance = new Reflection();
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

	Type* Reflection::GetType(Type::Id id)
	{
		for (auto type : mInstance->mTypes)
		{
			if (type->ID() == id)
				return type;
		}

		return nullptr;
	}

	void Reflection::InitializeFundamentalTypes()
	{
		InitializeFundamentalType<int>();
		InitializeFundamentalType<bool>();
		InitializeFundamentalType<char>();
		InitializeFundamentalType<wchar_t>();
		InitializeFundamentalType<short int>();
		InitializeFundamentalType<long int>();
		InitializeFundamentalType<long long int>();
		InitializeFundamentalType<unsigned char>();
		InitializeFundamentalType<unsigned short int>();
		InitializeFundamentalType<unsigned int>();
		InitializeFundamentalType<unsigned long int>();
		InitializeFundamentalType<unsigned long long int>();
		InitializeFundamentalType<float>();
		InitializeFundamentalType<double>();
		InitializeFundamentalType<Basis>();
		InitializeFundamentalType<RectI>();
		InitializeFundamentalType<Color4>();
		InitializeFundamentalType<RectF>();
		InitializeFundamentalType<Vec2F>();
		InitializeFundamentalType<Vec2I>();
		//InitializeFundamentalType<Vertex2>();
		InitializeFundamentalType<String>();
		InitializeFundamentalType<WString>();
		InitializeFundamentalType<DataNode>();

		FundamentalType<void>::type.mName = typeid(void).name();
		FundamentalType<void>::type.mId = mInstance->mLastGivenTypeId++;
		FundamentalType<void>::type.mTypeAgent = new Type::TypeCreator<int>();

		Type::Dummy::type.mName = "Unknown";
		Type::Dummy::type.mId = mInstance->mLastGivenTypeId++;
		Type::Dummy::type.mTypeAgent = new Type::TypeCreator<int>();

		mInstance->mTypes.Add(&FundamentalType<void>::type);
	}

	Type IObject::type;
	Type FundamentalType<int>::type;
	Type FundamentalType<bool>::type;
	Type FundamentalType<char>::type;
	Type FundamentalType<wchar_t>::type;
	Type FundamentalType<short int>::type;
	Type FundamentalType<long int>::type;
	Type FundamentalType<long long int>::type;
	Type FundamentalType<unsigned char>::type;
	Type FundamentalType<unsigned short int>::type;
	Type FundamentalType<unsigned int>::type;
	Type FundamentalType<unsigned long int>::type;
	Type FundamentalType<unsigned long long int>::type;
	Type FundamentalType<float>::type;
	Type FundamentalType<double>::type;
	Type FundamentalType<long double>::type;
	Type FundamentalType<void>::type;
	Type FundamentalType<float const>::type;
	Type FundamentalType<int const>::type;
	Type FundamentalType<unsigned int const>::type;
	Type FundamentalType<Basis>::type;
	Type FundamentalType<Color4>::type;
	Type FundamentalType<RectF>::type;
	Type FundamentalType<RectI>::type;
	Type FundamentalType<Vec2F>::type;
	Type FundamentalType<Vec2I>::type;
	Type FundamentalType<Vertex2>::type;
	Type FundamentalType<String>::type;
	Type FundamentalType<WString>::type;
	Type FundamentalType<DataNode>::type;
}