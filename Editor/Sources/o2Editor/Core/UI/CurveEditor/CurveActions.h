#pragma once

#include "o2Editor/Core/Actions/IAction.h"
#include "o2Editor/Core/UI/CurveEditor/CurvesEditor.h"

namespace Editor
{
    class CurveAddKeysAction : public IAction
    {
    public:
        CurveAddKeysAction();
        CurveAddKeysAction(const Vector<CurvesEditor::CurveKeysInfo>& infos, const Ref<CurvesEditor>& editor);

        String GetName() const;
        void Redo() const;
        void Undo() const;

        SERIALIZABLE(CurveAddKeysAction);

    protected:
        Vector<CurvesEditor::CurveKeysInfo> mInfos;
        Ref<CurvesEditor> mEditor;
    };

    class CurveDeleteKeysAction : public IAction
    {
    public:
        CurveDeleteKeysAction();
        CurveDeleteKeysAction(const Vector<CurvesEditor::CurveKeysInfo>& infos, const Ref<CurvesEditor>& editor);

        String GetName() const;
        void Redo() const;
        void Undo() const;

        SERIALIZABLE(CurveDeleteKeysAction);

    protected:
        Vector<CurvesEditor::CurveKeysInfo> mInfos;
        Ref<CurvesEditor> mEditor;
    };

    class CurveKeysChangeAction : public IAction
    {
    public:
        struct KeysInfo
        {
            String curveId;

            Vector<Curve::Key> beforeKeys;
            Vector<Curve::Key> afterKeys;

            Vector<CurvesEditor::SelectedHandlesInfo> selectedHandles;

            bool operator==(const KeysInfo& other) const;
        };

    public:
        CurveKeysChangeAction();
        CurveKeysChangeAction(const Vector<KeysInfo>& infos, const Ref<CurvesEditor>& editor);

        String GetName() const;
        void Redo() const;
        void Undo() const;

        SERIALIZABLE(CurveKeysChangeAction);

    protected:
        Vector<KeysInfo> mInfos;
        Ref<CurvesEditor> mEditor;
    };
}