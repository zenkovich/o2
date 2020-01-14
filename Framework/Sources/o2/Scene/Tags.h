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
		typedef Vector<Actor*> ActorsVec;

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
		String    mName;   // Tag name @SERIALIZABLE
		ActorsVec mActors; // Actors in layer

		friend class Actor;
	};
	typedef Vector<Tag*> Vector<Tag*>;

	// ----------
	// Tags group
	// ----------
	class TagGroup: public ISerializable
	{
	public:
		typedef Vector<String> StringsVec;

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
		StringsVec GetTagsNames() const;

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
// 	class TagDataNodeConverter: public IDataNodeTypeConverter
// 	{
// 	public:
// 		// Converts tag pointer to data 
// 		void ToData(void* object, DataNode& data);
// 
// 		// Gets tag pointer from data
// 		void FromData(void* object, const DataNode& data);
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
	PROTECTED_FIELD(mName).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mActors);
}
END_META;
CLASS_METHODS_META(o2::Tag)
{

	PUBLIC_FUNCTION(const String&, GetName);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(void, AddActor, Actor*);
	PUBLIC_FUNCTION(void, RemoveActor, Actor*);
	PUBLIC_FUNCTION(void, Clear);
}
END_META;

CLASS_BASES_META(o2::TagGroup)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::TagGroup)
{
	PUBLIC_FIELD(onTagAdded);
	PUBLIC_FIELD(onTagRemoved);
	PRIVATE_FIELD(mTags).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::TagGroup)
{

	PUBLIC_FUNCTION(void, AddTag, const String&);
	PUBLIC_FUNCTION(void, AddTag, Tag*);
	PUBLIC_FUNCTION(void, RemoveTag, const String&);
	PUBLIC_FUNCTION(void, RemoveTag, Tag*);
	PUBLIC_FUNCTION(void, Clear);
	PUBLIC_FUNCTION(bool, IsHaveTag, const String&);
	PUBLIC_FUNCTION(bool, IsHaveTag, Tag*);
	PUBLIC_FUNCTION(const Vector<Tag*>&, GetTags);
	PUBLIC_FUNCTION(StringsVec, GetTagsNames);
}
END_META;
