#include "stdafx.h"

#include "Actions.h"

namespace Editor
{
	AddKeysAction::AddKeysAction(const CurveEditor::CurveKeysInfosVec& infos, CurveEditor* editor) :
		mInfos(infos), mEditor(editor)
	{
		for (auto& info : mInfos)
		{
			info.keys.Sort([](const Curve::Key& a, const Curve::Key& b) { return a.position < b.position; });
		}
	}

	AddKeysAction::AddKeysAction()
	{ }

	String AddKeysAction::GetName()
	{
		return "add keys";
	}

	void AddKeysAction::Redo()
	{
		mEditor->DeselectAll();
		mEditor->mSelectedHandles.Clear();

		for (auto& info : mInfos)
		{
			auto curveInfo =
				mEditor->mCurves.FindMatch([&](CurveEditor::CurveInfo* x) { return x->curveId == info.curveId; });

			if (!curveInfo)
				continue;

			curveInfo->BeginCurveManualChange();

			Vector<int> indexes;

			for (auto& key : info.keys)
				indexes.Add(curveInfo->curve->InsertKey(key));

			curveInfo->UpdateApproximatedPoints();
			curveInfo->UpdateHandles();

			for (int idx : indexes)
				curveInfo->handles[idx]->mainHandle.SetSelected(true);

			mEditor->mNeedRedraw = true;

			curveInfo->CompleteCurveManualChange();
		}

		mEditor->UpdateTransformFrame();
		mEditor->CheckHandlesVisible();
	}

	void AddKeysAction::Undo()
	{
		for (auto& info : mInfos)
		{
			auto curveInfo =
				mEditor->mCurves.FindMatch([&](CurveEditor::CurveInfo* x) { return x->curveId == info.curveId; });

			if (!curveInfo)
				continue;

			curveInfo->BeginCurveManualChange();

			for (auto& key : info.keys)
				curveInfo->curve->RemoveKey(key.position);

			curveInfo->UpdateApproximatedPoints();
			curveInfo->UpdateHandles();
			mEditor->mNeedRedraw = true;

			curveInfo->CompleteCurveManualChange();
		}

		mEditor->DeselectAll();
		mEditor->mSelectedHandles.Clear();
		mEditor->UpdateTransformFrame();
		mEditor->CheckHandlesVisible();
	}

	DeleteKeysAction::DeleteKeysAction(const CurveEditor::CurveKeysInfosVec& infos, CurveEditor* editor) :
		mInfos(infos), mEditor(editor)
	{
		for (auto& info : mInfos)
		{
			info.keys.Sort([](const Curve::Key& a, const Curve::Key& b) { return a.position < b.position; });
		}
	}

	DeleteKeysAction::DeleteKeysAction()
	{ }

	String DeleteKeysAction::GetName()
	{
		return "remove keys";
	}

	void DeleteKeysAction::Redo()
	{
		for (auto& info : mInfos)
		{
			auto curveInfo =
				mEditor->mCurves.FindMatch([&](CurveEditor::CurveInfo* x) { return x->curveId == info.curveId; });

			if (!curveInfo)
				continue;

			curveInfo->BeginCurveManualChange();

			for (auto& key : info.keys)
				curveInfo->curve->RemoveKey(key.position);

			curveInfo->UpdateApproximatedPoints();
			curveInfo->UpdateHandles();
			mEditor->mNeedRedraw = true;

			curveInfo->CompleteCurveManualChange();
		}

		mEditor->DeselectAll();
		mEditor->mSelectedHandles.Clear();
		mEditor->UpdateTransformFrame();
		mEditor->CheckHandlesVisible();
	}

	void DeleteKeysAction::Undo()
	{
		mEditor->DeselectAll();
		mEditor->mSelectedHandles.Clear();

		for (auto& info : mInfos)
		{
			auto curveInfo =
				mEditor->mCurves.FindMatch([&](CurveEditor::CurveInfo* x) { return x->curveId == info.curveId; });

			if (!curveInfo)
				continue;

			curveInfo->BeginCurveManualChange();

			Vector<int> indexes;

			for (auto& key : info.keys)
				indexes.Add(curveInfo->curve->InsertKey(key));

			curveInfo->UpdateApproximatedPoints();
			curveInfo->UpdateHandles();

			for (int idx : indexes)
				curveInfo->handles[idx]->mainHandle.SetSelected(true);

			mEditor->mNeedRedraw = true;

			curveInfo->CompleteCurveManualChange();
		}

		mEditor->UpdateTransformFrame();
		mEditor->CheckHandlesVisible();
	}

	KeysChangeAction::KeysChangeAction(const KeysInfosVec& infos, CurveEditor* editor) :
		mInfos(infos), mEditor(editor)
	{ }

	KeysChangeAction::KeysChangeAction()
	{ }

	bool KeysChangeAction::KeysInfo::operator==(const KeysInfo& other) const
	{
		return curveId == other.curveId && beforeKeys == other.beforeKeys && afterKeys == other.afterKeys;
	}

	String KeysChangeAction::GetName()
	{
		return "change keys";
	}

	void KeysChangeAction::Redo()
	{
		mEditor->DeselectAll();
		mEditor->mSelectedHandles.Clear();

		for (auto& info : mInfos)
		{
			auto curveInfo =
				mEditor->mCurves.FindMatch([&](CurveEditor::CurveInfo* x) { return x->curveId == info.curveId; });

			if (!curveInfo)
				continue;

			curveInfo->BeginCurveManualChange();

			for (auto& key : info.beforeKeys)
				curveInfo->curve->RemoveKey(key.position);

			for (auto& key : info.afterKeys)
				curveInfo->curve->InsertKey(key);

			for (auto& selectedHandles : info.selectedHandles)
			{
				auto handles = curveInfo->handles[selectedHandles.index];

				if (selectedHandles.mainHandle)
					handles->mainHandle.Select();

				if (selectedHandles.leftSupportHandle)
					handles->leftSupportHandle.Select();

				if (selectedHandles.rightSupportHandle)
					handles->rightSupportHandle.Select();
			}

			curveInfo->UpdateApproximatedPoints();
			curveInfo->UpdateHandles();
			mEditor->mNeedRedraw = true;

			curveInfo->CompleteCurveManualChange();
		}

		mEditor->UpdateTransformFrame();
		mEditor->CheckHandlesVisible();
	}

	void KeysChangeAction::Undo()
	{
		mEditor->DeselectAll();
		mEditor->mSelectedHandles.Clear();

		for (auto& info : mInfos)
		{
			auto curveInfo =
				mEditor->mCurves.FindMatch([&](CurveEditor::CurveInfo* x) { return x->curveId == info.curveId; });

			if (!curveInfo)
				continue;

			curveInfo->BeginCurveManualChange();

			for (auto& key : info.afterKeys)
				curveInfo->curve->RemoveKey(key.position);

			for (auto& key : info.beforeKeys)
				curveInfo->curve->InsertKey(key);

			for (auto& selectedHandles : info.selectedHandles)
			{
				auto handles = curveInfo->handles[selectedHandles.index];

				if (selectedHandles.mainHandle)
					handles->mainHandle.Select();

				if (selectedHandles.leftSupportHandle)
					handles->leftSupportHandle.Select();

				if (selectedHandles.rightSupportHandle)
					handles->rightSupportHandle.Select();
			}

			curveInfo->UpdateApproximatedPoints();
			curveInfo->UpdateHandles();
			mEditor->mNeedRedraw = true;

			curveInfo->CompleteCurveManualChange();
		}

		mEditor->UpdateTransformFrame();
		mEditor->CheckHandlesVisible();
	}
}

DECLARE_CLASS(Editor::AddKeysAction);

DECLARE_CLASS(Editor::DeleteKeysAction);

DECLARE_CLASS(Editor::KeysChangeAction);
