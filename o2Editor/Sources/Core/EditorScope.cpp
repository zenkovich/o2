#include "stdafx.h"
#include "EditorScope.h"

#include "Events/EventSystem.h"
#include "Utils/Debug/Assert.h"

namespace Editor
{
	void Scope::Enter()
	{
		if (mDepth == 0)
		{
			Actor::SetDefaultCreationMode(ActorCreateMode::NotInScene);
			EventSystem::eventsListenersEnabledByDefault = true;
		}

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

	PushScopeEnterOnStack::PushScopeEnterOnStack()
	{
		Scope::Enter();
	}

	PushScopeEnterOnStack::~PushScopeEnterOnStack()
	{
		Scope::Exit();
	}

}