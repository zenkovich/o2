#include "o2Editor/stdafx.h"
#include "EditorScope.h"

#include "o2/Events/EventSystem.h"
#include "o2/Utils/Debug/Assert.h"

namespace Editor
{
	void Scope::Enter()
	{
		Actor::SetDefaultCreationMode(ActorCreateMode::NotInScene);
		EventSystem::eventsListenersEnabledByDefault = true;

		mDepth++;
	}

	void Scope::Exit()
	{
		mDepth--;

		if (mDepth == 0)
		{
			Actor::SetDefaultCreationMode(ActorCreateMode::InScene);
			EventSystem::eventsListenersEnabledByDefault = false;
		}
		else 
		{
			Actor::SetDefaultCreationMode(ActorCreateMode::NotInScene);
			EventSystem::eventsListenersEnabledByDefault = true;
		}

		Assert(mDepth >= 0, "Editor scope Enter/Exit mismatch");
	}

	int Scope::GetDepth()
	{
		return mDepth;
	}

	bool Scope::IsInScope()
	{
		return mDepth > 0;
	}

	int Scope::mDepth = 0;

	PushEditorScopeOnStack::PushEditorScopeOnStack()
	{
		Scope::Enter();
	}

	PushEditorScopeOnStack::~PushEditorScopeOnStack()
	{
		Scope::Exit();
	}

}
