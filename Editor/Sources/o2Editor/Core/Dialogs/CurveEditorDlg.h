#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Math/Curve.h"
#include "o2/Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class Window;
}

namespace Editor
{
	class CurvesEditor;

	// --------------------
	// Curve editing dialog
	// --------------------
	class CurveEditorDlg: public Singleton<CurveEditorDlg>, public CursorEventsListener
	{
	public:
		// Default constructor
		CurveEditorDlg();

		// Destructor
		~CurveEditorDlg();

		// Shows editor dialog
		static void Show(const Ref<Function<void()>>& onChanged, const Ref<Function<void()>>& onCompleted = nullptr);

		// Adds editing curve with color. If color is default it will be randomized
		static void AddEditingCurve(const String& id, const Ref<Curve>& curve, const Color4& color = Color4::Green());

		// Removed curve from editing
		static void RemoveEditingCurve(const Ref<Curve>& curve);

		// Removed curve from editing
		static void RemoveEditingCurve(const String& id);

		// Removes all editing curves
		static void RemoveAllEditingCurves();

		// Adds curves range with color. It can't be edited, just a solid color between curves
		static void AddCurvesRange(const Ref<Curve>& curveA, const Ref<Curve>& curveB, const Color4& color = Color4::Green());

		// Removes curve range
		static void RemoveCurvesRange(const Ref<Curve>& curveA, const Ref<Curve>& curveB);

	protected:
		Ref<Function<void()>> mOnChangedCallback;
		Ref<Function<void()>> mOnChangeCompletedCallback;

		o2::Window*   mWindow = nullptr;
		CurvesEditor* mEditorWidget = nullptr;

	protected:
		// Called when hiding, calls completed callback
		void OnHide();

		// Initialize controls
		void InitializeControls();

		// Called when cursor was pressed outside, closed dialog
		void OnCursorPressedOutside();
	};
	 
}