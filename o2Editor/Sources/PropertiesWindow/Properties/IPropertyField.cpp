#include "stdafx.h"
#include "IPropertyField.h"

#include "SceneWindow/SceneEditScreen.h"
#include "UI/Widget.h"

namespace Editor
{
	void IPropertyField::SetValuePtr(const Vector<void*>& targets, bool isProperty)
	{
		SetValueAndPrototypePtr(targets.Select<Pair<void*, void*>>([](void* x) { return Pair<void*, void*>(x, nullptr); }), isProperty);
	}

	void IPropertyField::SetValuePath(const String& path)
	{
		mValuesPath = path;
	}

	const String& IPropertyField::GetValuePath() const
	{
		return mValuesPath;
	}

	void IPropertyField::OnChanged()
	{
		CheckRevertableState();
		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}
}

DECLARE_CLASS(Editor::IPropertyField);
