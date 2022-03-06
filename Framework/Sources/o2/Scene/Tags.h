#pragma once

#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Actor;

	// ---------
	// Scene tag
	// ---------
	class Tag: public ISerializable
	{
	public:
		// Default constructor
		Tag();

		// Constructor with name
		Tag(const String& name);

		// Destructor
		~Tag();

		// Returns tag name
		const String& GetName() const;

		// Sets tag name
		void SetName(const String& name);

		// Adds actor
		void AddActor(Actor* actor);

		// Removes actor
		void RemoveActor(Actor* actor);

		// Removes all actors
		void Clear();

		// Adds actor
		Tag& operator+=(Actor* actor);

		// Removes actor
		Tag& operator-=(Actor* actor);

		SERIALIZABLE(Tag);

	protected:
		String         mName;   // Tag name @SERIALIZABLE
		Vector<Actor*> mActors; // Actors in layer

		friend class Actor;
	};

	// ----------
	// Tags group
	// ----------
	class TagGroup: public ISerializable
	{
	public:
		Function<void(Tag*)> onTagAdded;
		Function<void(Tag*)> onTagRemoved;

	public:
		// Default constructor
		TagGroup();

		// Copy-constructor
		TagGroup(const TagGroup& other);

		// Destructor
		~TagGroup();

		// Copy-operator
		TagGroup& operator=(const TagGroup& other);

		// Check equals operator
		bool operator==(const TagGroup& other) const;

		// Check not equals operator
		bool operator!=(const TagGroup& other) const;

		// Adds tag
		void AddTag(const String& name);

		// Adds tag
		void AddTag(Tag* tag);

		// Removes tag
		void RemoveTag(const String& name);

		// Removes tag
		void RemoveTag(Tag* tag);

		// Removes all tags
		void Clear();

		// Returns is have tag with name
		bool IsHaveTag(const String& name) const;

		// Returns is have tag
		bool IsHaveTag(Tag* tag) const;

		// Returns tags array
		const Vector<Tag*>& GetTags() const;

		// Returns tags names array
		Vector<String> GetTagsNames() const;

		// Adds tag
		TagGroup& operator+=(const String& name);

		// Adds tag
		TagGroup& operator+=(Tag* tag);

		// Removes tag
		TagGroup& operator-=(const String& name);

		// Removes tag
		TagGroup& operator-=(Tag* tag);

		SERIALIZABLE(TagGroup);

	private:
		Vector<Tag*> mTags; // @SERIALIZABLE

		friend class Tag;
	};

	// -----------------------
	// Tag data node converter
	// -----------------------
// 	class TagDataValueConverter: public IDataValueTypeConverter
// 	{
// 	public:
// 		// Converts tag pointer to data 
// 		void ToData(void* object, DataValue& data);
// 
// 		// Gets tag pointer from data
// 		void FromData(void* object, const DataValue& data);
// 
// 		// Checks that type is tag's type
// 		bool IsConvertsType(const Type* type) const;
// 	};
}

CLASS_BASES_META(o2::Tag)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::Tag)
{
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mName);
	FIELD().PROTECTED().NAME(mActors);
}
END_META;
CLASS_METHODS_META(o2::Tag)
{

	FUNCTION().PUBLIC().SIGNATURE(const String&, GetName);
	FUNCTION().PUBLIC().SIGNATURE(void, SetName, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, AddActor, Actor*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveActor, Actor*);
	FUNCTION().PUBLIC().SIGNATURE(void, Clear);
}
END_META;

CLASS_BASES_META(o2::TagGroup)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::TagGroup)
{
	FIELD().PUBLIC().NAME(onTagAdded);
	FIELD().PUBLIC().NAME(onTagRemoved);
	FIELD().PRIVATE().SERIALIZABLE_ATTRIBUTE().NAME(mTags);
}
END_META;
CLASS_METHODS_META(o2::TagGroup)
{

	FUNCTION().PUBLIC().SIGNATURE(void, AddTag, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, AddTag, Tag*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveTag, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveTag, Tag*);
	FUNCTION().PUBLIC().SIGNATURE(void, Clear);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsHaveTag, const String&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsHaveTag, Tag*);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<Tag*>&, GetTags);
	FUNCTION().PUBLIC().SIGNATURE(Vector<String>, GetTagsNames);
}
END_META;
