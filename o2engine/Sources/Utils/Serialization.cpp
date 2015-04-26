#include "Serialization.h"

#include "Utils/Debug.h"

namespace o2
{
	Serializer::Serializer(Type type /*= Type::Serialize*/)
	{
		mCurrentNode = mRootNode;
		mLog = Debug::GetLog();
		mType = type;
	}

	Serializer::Serializer(const String& fileName, Type type /*= Type::Serialize*/)
	{
		mCurrentNode = mRootNode;
		mLog = Debug::GetLog();
		Load(fileName);
	}

	Serializer::Serializer(Xml::Node& xmlNode, Type type /*= Type::Serialize*/)
	{
		mCurrentNode = mRootNode.append_copy(xmlNode);
		mType = type;
	}

	Serializer::~Serializer()
	{
	}

	bool Serializer::Load(const String& file)
	{
		if (!Xml::LoadFromFile(file, mRootNode))
			return false;

		mCurrentNode = mRootNode;
		mType = Type::Deserialize;

		return true;
	}

	bool Serializer::Save(const String& file)
	{
		return Xml::SaveToFile(file, mRootNode);
	}

	bool Serializer::LoadFromString(const String& data)
	{
		return Xml::LoadFromString(data, mRootNode);
	}

	String Serializer::SaveToString()
	{
		String data;
		Xml::SaveToString(data, mRootNode);
		return data;
	}

	void Serializer::SetLog(LogStream* logStream)
	{
		mLog = logStream;
	}

	void Serializer::CreateNode(const String& id)
	{
		mCurrentNode = mCurrentNode.append_child(id);
	}

	bool Serializer::GetNode(const String& id, bool errors /*= false*/)
	{
		Xml::Node node = mCurrentNode.child(id);
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
		mCurrentNode = mCurrentNode.parent();
	}

	Serializer::Type Serializer::GetType() const
	{
		return mType;
	}

	bool Serializer::Serialize(Serializable* object, const String& id, bool errors /*= true*/)
	{
		if (mType == Type::Serialize)
		{
			CreateNode(id);
			object->OnBeginSerialize();
			object->onBeginSerializeEvent.Invoke();
			mCurrentNode.append_attribute("type") = object->GetTypeName();
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

	bool Serializer::Serialize(int& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(unsigned int& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(float& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(bool& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(String& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Vec2F& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(RectF& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Vec2I& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(RectI& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Color4& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(WideTime& object, const String& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

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