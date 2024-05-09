#pragma once

#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/Ref.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
    class Actor;

    // ---------
    // Scene tag
    // ---------
    class Tag: public ISerializable, public RefCounterable
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
        void AddActor(const Ref<Actor>& actor);

        // Removes actor
        void RemoveActor(const Ref<Actor>& actor);

        // Removes all actors
        void Clear();

        // Adds actor
        Tag& operator+=(const Ref<Actor>& actor);

        // Removes actor
        Tag& operator-=(const Ref<Actor>& actor);

        SERIALIZABLE(Tag);

    protected:
        String mName; // Tag name @SERIALIZABLE

        Vector<WeakRef<Actor>> mActors; // Actors in layer

        friend class Actor;
    };

    // ----------
    // Tags group
    // ----------
    class TagGroup: public ISerializable
    {
    public:
        Function<void(const Ref<Tag>&)> onTagAdded;
        Function<void(const Ref<Tag>&)> onTagRemoved;

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
        void AddTag(const Ref<Tag>& tag);

        // Removes tag
        void RemoveTag(const String& name);

        // Removes tag
        void RemoveTag(const Ref<Tag>& tag);

        // Removes all tags
        void Clear();

        // Returns is have tag with name
        bool IsHaveTag(const String& name) const;

        // Returns is have tag
        bool IsHaveTag(const Ref<Tag>& tag) const;

        // Returns tags array
        const Vector<WeakRef<Tag>>& GetTags() const;

        // Returns tags names array
        Vector<String> GetTagsNames() const;

        // Adds tag
        TagGroup& operator+=(const String& name);

        // Adds tag
        TagGroup& operator+=(const Ref<Tag>& tag);

        // Removes tag
        TagGroup& operator-=(const String& name);

        // Removes tag
        TagGroup& operator-=(const Ref<Tag>& tag);

        SERIALIZABLE(TagGroup);

    private:
        Vector<WeakRef<Tag>> mTags; // @SERIALIZABLE

        friend class Tag;
    };

    // -----------------------
    // Tag data node converter
    // -----------------------
//     class TagDataValueConverter: public IDataValueTypeConverter
//     {
//     public:
//         // Converts tag pointer to data 
//         void ToData(void* object, DataValue& data);
// 
//         // Gets tag pointer from data
//         void FromData(void* object, const DataValue& data);
// 
//         // Checks that type is tag's type
//         bool IsConvertsType(const Type* type) const;
//     };
}
// --- META ---

CLASS_BASES_META(o2::Tag)
{
    BASE_CLASS(o2::ISerializable);
    BASE_CLASS(o2::RefCounterable);
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

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const String&);
    FUNCTION().PUBLIC().SIGNATURE(const String&, GetName);
    FUNCTION().PUBLIC().SIGNATURE(void, SetName, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, AddActor, const Ref<Actor>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveActor, const Ref<Actor>&);
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

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const TagGroup&);
    FUNCTION().PUBLIC().SIGNATURE(void, AddTag, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, AddTag, const Ref<Tag>&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveTag, const String&);
    FUNCTION().PUBLIC().SIGNATURE(void, RemoveTag, const Ref<Tag>&);
    FUNCTION().PUBLIC().SIGNATURE(void, Clear);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsHaveTag, const String&);
    FUNCTION().PUBLIC().SIGNATURE(bool, IsHaveTag, const Ref<Tag>&);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<WeakRef<Tag>>&, GetTags);
    FUNCTION().PUBLIC().SIGNATURE(Vector<String>, GetTagsNames);
}
END_META;
// --- END META ---
