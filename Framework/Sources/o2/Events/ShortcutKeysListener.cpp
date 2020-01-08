#include "o2/stdafx.h"
#include "ShortcutKeysListener.h"

namespace o2
{
	ShortcutKeysListener::~ShortcutKeysListener()
	{
		ShortcutKeysListenersManager::Instance().UnRegister(mShortcut, this);
	}

	void ShortcutKeysListener::SetMaxPriority()
	{
		ShortcutKeysListenersManager::Instance().SetMaxPriority(mShortcut, this);
	}

	void ShortcutKeysListener::SetMinPriority()
	{
		ShortcutKeysListenersManager::Instance().SetMinPriority(mShortcut, this);
	}

	void ShortcutKeysListener::SetShortcut(const ShortcutKeys& shortcut)
	{
		ShortcutKeysListenersManager::Instance().UnRegister(mShortcut, this);
		mShortcut = shortcut;
		ShortcutKeysListenersManager::Instance().Register(mShortcut, this);
	}

	const ShortcutKeys& ShortcutKeysListener::GetShortcut() const
	{
		return mShortcut;
	}

	void ShortcutKeysListener::SetEnabled(bool enabled)
	{
		if (enabled == mEnabled)
			return;

		mEnabled = enabled;

		if (mEnabled)
			ShortcutKeysListenersManager::Instance().Register(mShortcut, this);
		else
			ShortcutKeysListenersManager::Instance().UnRegister(mShortcut, this);
	}

	bool ShortcutKeysListener::IsEnabled() const
	{
		return mEnabled;
	}


	DECLARE_SINGLETON(ShortcutKeysListenersManager);

	void ShortcutKeysListenersManager::Register(const ShortcutKeys& shortcut, ShortcutKeysListener* listener)
	{
		if (shortcut.IsEmpty())
			return;

		if (!mListeners.ContainsKey(shortcut))
			mListeners.Add(shortcut, Vector<ShortcutKeysListener*>());

		mListeners[shortcut].Add(listener);
	}

	void ShortcutKeysListenersManager::UnRegister(const ShortcutKeys& shortcut, ShortcutKeysListener* listener)
	{
		if (!mListeners.ContainsKey(shortcut))
			return;

		mListeners[shortcut].Remove(listener);

		if (mListeners[shortcut].IsEmpty())
			mListeners.Remove(shortcut);
	}

	void ShortcutKeysListenersManager::SetMinPriority(const ShortcutKeys& shortcut, ShortcutKeysListener* listener)
	{
		if (!mListeners.ContainsKey(shortcut))
			return;

		mListeners[shortcut].Remove(listener);
		mListeners[shortcut].Insert(listener, 0);
	}

	void ShortcutKeysListenersManager::SetMaxPriority(const ShortcutKeys& shortcut, ShortcutKeysListener* listener)
	{
		if (!mListeners.ContainsKey(shortcut))
			return;

		mListeners[shortcut].Remove(listener);
		mListeners[shortcut].Add(listener);
	}

	void ShortcutKeysListenersManager::OnKeyPressed(const Input::Key& key)
	{
		for (auto& kv : mListeners)
		{
			if (kv.first.IsPressed() && !kv.second.IsEmpty())
			{
				for (int i = kv.second.Count() - 1; i >= 0; i--)
				{
					if (kv.second[i]->IsListeningEvents())
					{
						kv.second[i]->OnShortcutPressed();
						break;
					}
				}
			}
		}
	}

}
