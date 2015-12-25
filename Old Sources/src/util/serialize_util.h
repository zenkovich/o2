#pragma once

#include <map>
#include "public.h"
#include "callback.h"
#include "time_utils.h"
#include "xml_tools.h"

OPEN_O2_NAMESPACE

class LogStream;
class Serializer;

/** Serializable object interface. */
class Serializable
{
	friend class Serializer;

public:
	CallbackChain onBeginSerializeEvent; /** Calls when serialization begins. */
	CallbackChain onDeserializedEvent;   /** Calls when serialization complete. */

	/** Serialization function. */
	virtual bool Serialize(Serializer* serializer) = 0;
	virtual Serializable* CreateSample() const = 0;
	virtual String GetTypeName() const = 0;

protected:
	virtual void OnBeginSerialize() {}
	virtual void OnDeserialized() {}
};

struct gSerializeTypesContainer
{
	typedef std::map<String, Serializable*> SamplesMap;

	static SamplesMap mSamples;

	static void          RegType(Serializable* type);
	static Serializable* GetSample(const String& typeName);
	static void          OutputRegisteredSamples();
};

template<typename T>
struct gSerializeTypesInitializer
{
	gSerializeTypesInitializer()
	{
		T* sample = new T();
		gSerializeTypesContainer::RegType(sample);
	}
};

/** Basic serialize object, need to serialize data structures. */
class Serializer
{
public:
	enum class Type { Serialize, Deserialize };

private:
	LogStream*         mLog;         /** Serialization log, where puts errors. */
	pugi::xml_document mRootNode;    /** Root serialization xml document. */
	pugi::xml_node     mCurrentNode; /** Current xml node. */
	Type      mType;        /** Serialization type. */

public:
	/** ctor. */
	Serializer(Type type = Type::Serialize);

	/** ctor. Loading file and setting deserialize type. */
	Serializer(const String& fileName, Type type = Type::Serialize);

	/** ctor. Getting xml node and setting type. */
	Serializer(pugi::xml_node& xmlNode, Type type = Type::Serialize);

	/** dtor. */
	~Serializer();

	/** Returns serialization type. */
	Type GetType() const;

	/** Loading file, sets type deserialization and returns a flag of successful execution. */
	bool Load(const String& file, bool stdConfigExt = true);

	/** Saving data to file. */
	bool Save(const String& file, bool stdConfigExt = true);

	/** Loads data from file and returns a flag of successful execution. */
	bool LoadFromString(const String& data);

	/** Saving data to string. */
	String SaveToString();

	/** Sets log. If log not setted - using global log. */
	void SetLog(LogStream* logStream);

	/** Creates inherited node. */
	void CreateNode(const String& id);

	/** Trying to get inherited node. */
	bool GetNode(const String& id, bool errors = false);

	/** Returns to parent node. */
	void PopNode();

	/** Serialize object. */
	bool Serialize(Serializable* object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(int& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(unsigned int& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(float& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(bool& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(String& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(Vec2F& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(RectF& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(Vec2I& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(RectI& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(Color4& object, const String& id, bool errors = true);

	/** Serialize object. */
	bool Serialize(WideTime& object, const String& id, bool errors = true);

	/** Serialize object. */
	template<typename T>
	bool SerializeTemp(T& object, const String& id, bool errors = true)
	{
		if (mType == Type::Serialize)
		{
			CreateNode(id);
			XmlTools::ToXmlNode(object, mCurrentNode);
			PopNode();
			return true;
		}
		else
		{
			if (!GetNode(id, errors))
				return false;

			XmlTools::FromXmlNode(object, mCurrentNode);
			PopNode();
		}

		return true;
	}

	/** Saving data from object to xml node. */
	template<typename T>
	bool Serialize(Vector<T>& array, const String& id, bool errors = true)
	{
		if (mType == Type::Serialize)
		{
			CreateNode(id);

			mCurrentNode.append_attribute("count") = array.size();
			for (int i = 0; i < (int)array.size(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				T* arrElem = &(array[i]);
				Serialize(arrElem, elemNodeName, errors);
			}

			PopNode();
			return true;
		}
		else
		{
			if (!GetNode(id, errors))
				return false;

			array.clear();
			int srCount = mCurrentNode.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				array.push_back(T());
				T* elemPtr = &array.back();
				Serialize(elemPtr, elemNodeName, errors);
			}

			PopNode();
		}

		return true;
	}

	/** Saving data from object to xml node. */
	template<typename T>
	bool Serialize(Vector<T*>& array, const String& id, bool errors = true)
	{
		if (mType == Type::Serialize)
		{
			CreateNode(id);

			mCurrentNode.append_attribute("count") = array.size();
			for (int i = 0; i < (int)array.size(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				T* elem = array[i];
				Serialize(elem, elemNodeName, errors);
			}

			PopNode();
			return true;
		}
		else
		{
			if (!GetNode(id, errors))
				return false;

			array.clear();
			int srCount = mCurrentNode.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				String type = mCurrentNode.child(elemNodeName).attribute("type").value();
				T* elem = static_cast<T*>(CreateSerializableSample(type));
				Serialize(elem, elemNodeName, errors);
				array.push_back(elem);
			}

			PopNode();
		}

		return true;
	}

	/** Saving data from object to xml node. */
	template<typename T>
	bool Serialize(Array<T>& arr, const String& id, bool errors = true)
	{
		if (mType == Type::Serialize)
		{
			CreateNode(id);

			mCurrentNode.append_attribute("count") = arr.Count();
			for (int i = 0; i < (int)arr.Count(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				T* arrElem = &(arr[i]);
				Serialize(arrElem, elemNodeName, errors);
			}

			PopNode();
			return true;
		}
		else
		{
			if (!GetNode(id, errors))
				return false;

			arr.Clear();
			int srCount = mCurrentNode.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				arr.Add(T());
				T* elemPtr = &arr.Last();
				Serialize(elemPtr, elemNodeName, errors);
			}

			PopNode();
		}

		return true;
	}

	/** Saving data from object to xml node. */
	template<typename T>
	bool Serialize(Array<T*>& arr, const String& id, bool errors = true)
	{
		if (mType == Type::Serialize)
		{
			CreateNode(id);

			mCurrentNode.append_attribute("count") = arr.Count();
			for (int i = 0; i < (int)arr.Count(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				T* elem = arr[i];
				Serialize(elem, elemNodeName, errors);
			}

			PopNode();
			return true;
		}
		else
		{
			if (!GetNode(id, errors))
				return false;

			arr.Clear();
			int srCount = mCurrentNode.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				String type = mCurrentNode.child(elemNodeName).attribute("type").value();
				T* elem = static_cast<T*>(CreateSerializableSample(type));
				Serialize(elem, elemNodeName, errors);
				arr.Add(elem);
			}

			PopNode();
		}

		return true;
	}

private:
	static Serializable* CreateSerializableSample(const String& type);
};

#define FIRST_SERIALIZATION() std::map<string, Serializable*> gSerializeTypesContainer::mSamples

/** Implementation of serialize method. You must define class. */
#define SERIALIZE_METHOD_IMPL(CLASS)                  \
	gSerializeTypesInitializer<CLASS> CLASS::RegType; \
	bool CLASS::Serialize(Serializer* serializer)

/** Implementation of serialize method for inherited class. */
#define SERIALIZE_INHERITED_METHOD_IMPL(CLASS)        \
	gSerializeTypesInitializer<CLASS> CLASS::RegType; \
	bool CLASS::SerializeInh(Serializer* serializer)

/** Declaration of serialize methods. */
#define SERIALIZBLE_METHODS(CLASS)                                       \
	static gSerializeTypesInitializer<CLASS> RegType;                    \
	virtual Serializable* CreateSample() const { return mnew CLASS(); }  \
	virtual String GetTypeName() const { return #CLASS; }                \
	bool CLASS::Serialize(Serializer* serializer)

/** Declaration of inherited serialize methods. */
#define SERIALIZBLE_INHERITED_METHODS(CLASS, BASIC_CLASS)                \
	static gSerializeTypesInitializer<CLASS> RegType;                    \
	virtual Serializable* CreateSample() const { return mnew CLASS(); }  \
	virtual String GetTypeName() const { return #CLASS; }                \
	virtual bool SerializeInh(Serializer* serializer);                   \
	virtual bool Serialize(Serializer* serializer)                       \
{                               							         \
if (serializer->GetType() == Serializer::Type::Serialize)	             \
	serializer->CreateNode(#BASIC_CLASS);				         \
		else													         \
		serializer->GetNode(#BASIC_CLASS, true);			         \
		\
if (!BASIC_CLASS::Serialize(serializer)) 				         \
{														         \
	serializer->PopNode();								         \
	return false;										         \
}														         \
	\
	serializer->PopNode();									         \
	\
	return SerializeInh(serializer);						         \
}

/** Serialize object with ID. */
#define SERIALIZE_ID(obj, id) \
	serializer->Serialize(obj, id);

/** Serialize @obj with name "obj". */
#define SERIALIZE(obj) \
	SERIALIZE_ID(obj, #obj)

CLOSE_O2_NAMESPACE
