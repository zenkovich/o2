#include "o2/stdafx.h"
#include "Tags.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"
#include "o2/Utils/Debug/Debug.h"

namespace o2
{

    Tag::Tag()
    {}

    Tag::Tag(const String& name):
        mName(name)
    {}

    Tag::~Tag()
    {
        Clear();
    }

    const String& Tag::GetName() const
    {
        return mName;
    }

    void Tag::SetName(const String& name)
    {
        if (o2Scene.GetTag(name))
        {
            o2Debug.LogWarning("Can't rename tag " + mName + " to " + name + ": already exist tag with same name");
            return;
        }

        mName = name;
    }

    void Tag::AddActor(const ActorRef<>& actor)
    {
        if (mActors.Contains(actor.GetRef()))
            return;

        mActors.Add(actor.GetRef());
        actor->tags.mTags.Add(Ref(this));
    }

    void Tag::RemoveActor(const ActorRef<>& actor)
    {
        if (!actor)
            return;

        mActors.Remove(actor.GetRef());
        actor->tags.mTags.Remove(Ref(this));
    }

    void Tag::Clear()
    {
        for (auto actor : mActors)
            actor.Lock()->tags.mTags.Remove(Ref(this));

        mActors.Clear();
    }

    Tag& Tag::operator-=(const ActorRef<>& actor)
    {
        RemoveActor(actor);
        return *this;
    }

    Tag& Tag::operator+=(const ActorRef<>& actor)
    {
        AddActor(actor);
        return *this;
    }

    TagGroup::TagGroup()
    {}

    TagGroup::TagGroup(const TagGroup& other):
        mTags(other.mTags)
    {}

    TagGroup::~TagGroup()
    {}

    TagGroup& TagGroup::operator=(const TagGroup& other)
    {
        Clear();

        mTags = other.mTags;

        for (auto tag : mTags)
            onTagAdded(tag.Lock());

        return *this;
    }

    bool TagGroup::operator==(const TagGroup& other) const
    {
        for (auto tag : mTags) {
            if (!other.IsHaveTag(tag.Lock()))
                return false;
        }

        return true;
    }

    bool TagGroup::operator!=(const TagGroup& other) const
    {
        return !(*this == other);
    }

    void TagGroup::AddTag(const String& name)
    {
        auto tag = o2Scene.GetTag(name);

        if (!tag)
            tag = o2Scene.AddTag(name);

        AddTag(tag);
    }

    void TagGroup::AddTag(const Ref<Tag>& tag)
    {
        if (!tag || mTags.Contains(tag))
            return;

        mTags.Add(tag);
        onTagAdded(tag);
    }

    void TagGroup::RemoveTag(const String& name)
    {
        auto tag = o2Scene.GetTag(name);

        if (!tag)
        {
            o2Debug.LogWarning("Can't remove tag with name " + name + " from group: isn't exist");
            return;
        }

        RemoveTag(tag);
    }

    void TagGroup::RemoveTag(const Ref<Tag>& tag)
    {
        if (!tag || !mTags.Contains(tag))
            return;

        mTags.Remove(tag);
        onTagRemoved(tag);
    }

    void TagGroup::Clear()
    {
        for (auto tag : mTags)
            onTagRemoved(tag.Lock());

        mTags.Clear();
    }

    bool TagGroup::IsHaveTag(const String& name) const
    {
        return mTags.Contains([&](auto x) { return x.Lock()->GetName() == name; });
    }

    bool TagGroup::IsHaveTag(const Ref<Tag>& tag) const
    {
        return mTags.Contains(tag);
    }

    const Vector<WeakRef<Tag>>& TagGroup::GetTags() const
    {
        return mTags;
    }

    Vector<String> TagGroup::GetTagsNames() const
    {
        return mTags.Convert<String>([](auto x) { return x.Lock()->GetName(); });
    }

    TagGroup& TagGroup::operator-=(const Ref<Tag>& tag)
    {
        RemoveTag(tag);
        return *this;
    }

    TagGroup& TagGroup::operator-=(const String& name)
    {
        RemoveTag(name);
        return *this;
    }

    TagGroup& TagGroup::operator+=(const Ref<Tag>& tag)
    {
        AddTag(tag);
        return *this;
    }

    TagGroup& TagGroup::operator+=(const String& name)
    {
        AddTag(name);
        return *this;
    }

//     void TagDataValueConverter::ToData(void* object, DataValue& data)
//     {
//         if (object)
//         {
//             Tag* value = (Tag*)object;
//             data = value->GetName();
//         }
//     }
// 
//     void TagDataValueConverter::FromData(void* object, const DataValue& data)
//     {
//         Tag*& value = *(Tag**)object;
//         value = o2Scene.GetTag(data);
//     }
// 
//     bool TagDataValueConverter::IsConvertsType(const Type* type) const
//     {
//         return type == &TypeOf(Tag);
//     }

}
// --- META ---

DECLARE_CLASS(o2::Tag, o2__Tag);

DECLARE_CLASS(o2::TagGroup, o2__TagGroup);
// --- END META ---
