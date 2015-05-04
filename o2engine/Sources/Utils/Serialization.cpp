#include "Serialization.h"

#include "Utils/Debug.h"
#include "Dependencies/PugiXml/pugixml.hpp"

namespace o2
{
	Serializer::Serializer(Type type /*= Type::Serialize*/)
	{
		mCurrentNode = &mDataDoc;
		mLog = Debug::GetLog();
		mType = type;
	}

	Serializer::Serializer(const String& fileName, Type type /*= Type::Serialize*/)
	{
		mCurrentNode = &mDataDoc;
		mLog = Debug::GetLog();
		Load(fileName);
	}

	Serializer::Serializer(DataNode* dataNode, Type type /*= Type::Serialize*/)
	{
		mCurrentNode = mDataDoc.AddNode(new DataNode(*dataNode));
		mType = type;
	}

	Serializer::~Serializer()
	{
	}

	bool Serializer::Load(const String& file)
	{
		if (!mDataDoc.LoadFromFile(file))
			return false;

		mCurrentNode = &mDataDoc;
		mType = Type::Deserialize;

		return true;
	}

	bool Serializer::Save(const String& file)
	{
		return mDataDoc.SaveToFile(file);
	}

	bool Serializer::LoadFromString(const WString& data)
	{
		return mDataDoc.LoadFromData(data);
	}

	o2::WString Serializer::SaveToString()
{
		return mDataDoc.SaveAsWString();
	}

	void Serializer::SetLog(LogStream* logStream)
	{
		mLog = logStream;
	}

	void Serializer::CreateNode(const WString& id)
	{
		mCurrentNode = mCurrentNode->AddNode(id);
	}

	bool Serializer::GetNode(const WString& id, bool errors /*= false*/)
	{
		auto node = mCurrentNode->GetNode(id);
		if (!node)
		{
			if (errors)
				mLog->Error("Failed to get serialize node: %s", id);

			return false;
		}

		mCurrentNode = node;
		return true;
	}

	void Serializer::PopNode()
	{
		mCurrentNode = mCurrentNode->GetParent();
	}

	Serializer::Type Serializer::GetType() const
	{
		return mType;
	}

	bool Serializer::Serialize(Serializable* object, const WString& id, bool errors /*= true*/)
	{
		if (mType == Type::Serialize)
		{
			CreateNode(id);
			object->OnBeginSerialize();
			object->onBeginSerializeEvent.Invoke();
			*mCurrentNode->AddNode("type") = (WString)object->GetTypeName();
			object->Serialize(this);
			PopNode();
			return true;
		}
		else
		{
			if (!GetNode(id, errors))
				return false;

			object->Serialize(this);
			object->OnDeserialized();
			object->onDeserializedEvent.Invoke();
			PopNode();
		}

		return true;
	}

	bool Serializer::Serialize(int& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(unsigned int& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(float& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(bool& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(WString& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(String& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Vec2F& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(RectF& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Vec2I& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(RectI& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Color4& object, const WString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

// 	bool Serializer::Serialize(WideTime& object, const String& id, bool errors /*= true*/)
// 	{
// 		return SerializeTemp(object, id, errors);
// 	}

	Serializable* Serializer::CreateSerializableSample(const String& type)
	{
		return gSerializeTypesContainer::GetSample(type);
	}

	void gSerializeTypesContainer::RegType(Serializable* type)
	{
		mSamples.Add(type->GetTypeName(), type);
	}

	Serializable* gSerializeTypesContainer::GetSample(const String& typeName)
	{
		return mSamples[typeName]->CreateSample();
	}

	void gSerializeTypesContainer::OutputRegisteredSamples()
	{
		for (auto sam : mSamples)
			Debug::Log("Sample: %s", sam.Key());
	}
}