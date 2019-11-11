#pragma once

#include "AnimationWindow/AnimationWindow.h"
#include "Core/Actions/IAction.h"

namespace Editor
{
	struct AnimationKeysData
	{
		String path;
		DataNode keys;
		Vector<UInt64> selectedKeys;
	};

	class AnimationAddKeysAction : public IAction
	{
	public:
		AnimationAddKeysAction();
		AnimationAddKeysAction(const Vector<AnimationKeysData>& keysData, KeyHandlesSheet* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(AnimationAddKeysAction);

	protected:
		Vector<AnimationKeysData> mKeysData;
		KeyHandlesSheet*          mEditor;
	};

	class AnimationDeleteKeysAction : public IAction
	{
	public:
		AnimationDeleteKeysAction();
		AnimationDeleteKeysAction(const Vector<AnimationKeysData>& keysData, KeyHandlesSheet* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(AnimationDeleteKeysAction);

	protected:
		Vector<AnimationKeysData> mKeysData;
		KeyHandlesSheet*          mEditor;
	};

	class AnimationKeysChangeAction : public IAction
	{
	public:
		AnimationKeysChangeAction();
		AnimationKeysChangeAction(const Vector<AnimationKeysData>& beforeKeysData, 
								  const Vector<AnimationKeysData>& afterKeysData, KeyHandlesSheet* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(AnimationKeysChangeAction);

	protected:
		Vector<AnimationKeysData> mBeforeKeysData;
		Vector<AnimationKeysData> mAfterKeysData;
		KeyHandlesSheet*          mEditor;
	};
}
