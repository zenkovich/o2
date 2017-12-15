#include "Tags.h"

#include "Scene/Actor.h"

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
			o2Debug.LogWarning("Can't rename tag %s to %s: already exist tag with same name", mName, name);
			return;
		}

		mName = name;
	}

	void Tag::AddActor(Actor* actor)
	{
		if (mActors.Contains(actor))
			return;

		mActors.Add(actor);
		actor->tags.mTags.Add(this);
	}

	void Tag::RemoveActor(Actor* actor)
	{
		if (!actor)
			return;

		mActors.Remove(actor);
		actor->tags.mTags.Remove(this);
	}

	void Tag::Clear()
	{
		for (auto actor : mActors)
			actor->tags.mTags.Remove(this);

		mActors.Clear();
	}

	Tag& Tag::operator-=(Actor* actor)
	{
		RemoveActor(actor);
		return *this;
	}

	Tag& Tag::operator+=(Actor* actor)
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
			onTagAdded(tag);

		return *this;
	}

	bool TagGroup::operator==(const TagGroup& other) const
	{
		for (auto tag : mTags)
			if (!other.IsHaveTag(tag))
				return false;

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

	void TagGroup::AddTag(Tag* tag)
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
			o2Debug.LogWarning("Can't remove tag with name %s from group: isn't exist", name);
			return;
		}

		RemoveTag(tag);
	}

	void TagGroup::RemoveTag(Tag* tag)
	{
		if (!tag || !mTags.Contains(tag))
			return;

		mTags.Remove(tag);
		onTagRemoved(tag);
	}

	void TagGroup::Clear()
	{
		for (auto tag : mTags)
			onTagRemoved(tag);

		mTags.Clear();
	}

	bool TagGroup::IsHaveTag(const String& name) const
	{
		return mTags.ContainsPred([&](auto x) { return x->GetName() == name; });
	}

	bool TagGroup::IsHaveTag(Tag* tag) const
	{
		return mTags.Contains(tag);
	}

	const TagsVec& TagGroup::GetTags() const
	{
		return mTags;
	}

	TagGroup::StringsVec TagGroup::GetTagsNames() const
	{
		return mTags.Select<String>([](auto x) { return x->GetName(); });
	}

	TagGroup& TagGroup::operator-=(Tag* tag)
	{
		RemoveTag(tag);
		return *this;
	}

	TagGroup& TagGroup::operator-=(const String& name)
	{
		RemoveTag(name);
		return *this;
	}

	TagGroup& TagGroup::operator+=(Tag* tag)
	{
		AddTag(tag);
		return *this;
	}

	TagGroup& TagGroup::operator+=(const String& name)
	{
		AddTag(name);
		return *this;
	}

	void TagDataNodeConverter::ToData(void* object, DataNode& data)
	{
		if (object)
		{
			Tag* value = (Tag*)object;
			data = value->GetName();
		}
	}

	void TagDataNodeConverter::FromData(void* object, const DataNode& data)
	{
		Tag*& value = *(Tag**)object;
		value = o2Scene.GetTag(data);
	}

	bool TagDataNodeConverter::IsConvertsType(const Type* type) const
	{
		return type == &TypeOf(Tag);
	}

}

CLASS_META(o2::Tag)
{
	BASE_CLASS(o2::ISerializable);

	PROTECTED_FIELD(mName).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mActors);

	PUBLIC_FUNCTION(const String&, GetName);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(void, AddActor, Actor*);
	PUBLIC_FUNCTION(void, RemoveActor, Actor*);
	PUBLIC_FUNCTION(void, Clear);
}
END_META;

CLASS_META(o2::TagGroup)
{
	BASE_CLASS(o2::ISerializable);

	PUBLIC_FIELD(onTagAdded);
	PUBLIC_FIELD(onTagRemoved);
	PRIVATE_FIELD(mTags).SERIALIZABLE_ATTRIBUTE();

	PUBLIC_FUNCTION(void, AddTag, const String&);
	PUBLIC_FUNCTION(void, AddTag, Tag*);
	PUBLIC_FUNCTION(void, RemoveTag, const String&);
	PUBLIC_FUNCTION(void, RemoveTag, Tag*);
	PUBLIC_FUNCTION(void, Clear);
	PUBLIC_FUNCTION(bool, IsHaveTag, const String&);
	PUBLIC_FUNCTION(bool, IsHaveTag, Tag*);
	PUBLIC_FUNCTION(const TagsVec&, GetTags);
	PUBLIC_FUNCTION(StringsVec, GetTagsNames);
}
END_META;
