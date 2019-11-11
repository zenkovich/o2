#include "stdafx.h"

#include "Actions.h"

namespace Editor
{
	CurveAddKeysAction::CurveAddKeysAction(const CurveEditor::CurveKeysInfosVec& infos, CurveEditor* editor) :
		mInfos(infos), mEditor(editor)
	{
		for (auto& info : mInfos)
		{
			info.keys.Sort([](const Curve::Key& a, const Curve::Key& b) { return a.position < b.position; });
		}
	}

	CurveAddKeysAction::CurveAddKeysAction()
	{ }

	String CurveAddKeysAction::GetName()
	{
		return "add keys";
	}

	void CurveAddKeysAction::Redo()
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

	void CurveAddKeysAction::Undo()
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

	CurveDeleteKeysAction::CurveDeleteKeysAction(const CurveEditor::CurveKeysInfosVec& infos, CurveEditor* editor) :
		mInfos(infos), mEditor(editor)
	{
		for (auto& info : mInfos)
		{
			info.keys.Sort([](const Curve::Key& a, const Curve::Key& b) { return a.position < b.position; });
		}
	}

	CurveDeleteKeysAction::CurveDeleteKeysAction()
	{ }

	String CurveDeleteKeysAction::GetName()
	{
		return "remove keys";
	}

	void CurveDeleteKeysAction::Redo()
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

	void CurveDeleteKeysAction::Undo()
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

	CurveKeysChangeAction::CurveKeysChangeAction(const KeysInfosVec& infos, CurveEditor* editor) :
		mInfos(infos), mEditor(editor)
	{ }

	CurveKeysChangeAction::CurveKeysChangeAction()
	{ }

	bool CurveKeysChangeAction::KeysInfo::operator==(const KeysInfo& other) const
	{
		return curveId == other.curveId && beforeKeys == other.beforeKeys && afterKeys == other.afterKeys;
	}

	String CurveKeysChangeAction::GetName()
	{
		return "change keys";
	}

	void CurveKeysChangeAction::Redo()
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

	void CurveKeysChangeAction::Undo()
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

DECLARE_CLASS(Editor::CurveAddKeysAction);

DECLARE_CLASS(Editor::CurveDeleteKeysAction);

DECLARE_CLASS(Editor::CurveKeysChangeAction);
