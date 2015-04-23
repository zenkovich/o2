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

	Serializer::Serializer(const TString& fileName, Type type /*= Type::Serialize*/)
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

	bool Serializer::Load(const TString& file)
	{
		if (!Xml::LoadFromFile(file, mRootNode))
			return false;

		mCurrentNode = mRootNode;
		mType = Type::Deserialize;

		return true;
	}

	bool Serializer::Save(const TString& file)
	{
		return Xml::SaveToFile(file, mRootNode);
	}

	bool Serializer::LoadFromString(const TString& data)
	{
		return Xml::LoadFromString(data, mRootNode);
	}

	TString Serializer::SaveToString()
	{
		TString data;
		Xml::SaveToString(data, mRootNode);
		return data;
	}

	void Serializer::SetLog(LogStream* logStream)
	{
		mLog = logStream;
	}

	void Serializer::CreateNode(const TString& id)
	{
		mCurrentNode = mCurrentNode.append_child(id.c_str());
	}

	bool Serializer::GetNode(const TString& id, bool errors /*= false*/)
	{
		Xml::Node node = mCurrentNode.child(id.c_str());
		if (!node)
		{
			if (errors)
				mLog->Error("Failed to get serialize node: %s", id.c_str());

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

	bool Serializer::Serialize(Serializable* object, const TString& id, bool errors /*= true*/)
	{
		if (mType == Type::Serialize)
		{
			CreateNode(id);
			object->OnBeginSerialize();
			object->onBeginSerializeEvent.Invoke();
			mCurrentNode.append_attribute("type") = object->GetTypeName().c_str();
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

	bool Serializer::Serialize(int& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(unsigned int& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(float& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(bool& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(TString& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Vec2F& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(RectF& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Vec2I& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(RectI& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(Color4& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	bool Serializer::Serialize(WideTime& object, const TString& id, bool errors /*= true*/)
	{
		return SerializeTemp(object, id, errors);
	}

	Serializable* Serializer::CreateSerializableSample(const TString& type)
	{
		return gSerializeTypesContainer::GetSample(type);
	}

	void gSerializeTypesContainer::RegType(Serializable* type)
	{
		mSamples.Add(type->GetTypeName(), type);
	}

	Serializable* gSerializeTypesContainer::GetSample(const TString& typeName)
	{
		return mSamples[typeName]->CreateSample();
	}

	void gSerializeTypesContainer::OutputRegisteredSamples()
	{
		for (auto sam : mSamples)
			Debug::Log("Sample: %s", sam.Key().c_str());
	}
}