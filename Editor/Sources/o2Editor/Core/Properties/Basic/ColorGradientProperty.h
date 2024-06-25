#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Utils/Editor/DragHandle.h"
#include "o2/Utils/Math/ColorGradient.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

using namespace o2;

namespace o2
{
	class Widget;
}

namespace Editor
{
	FORWARD_CLASS_REF(CurvePreview);

	// ------------------------------
	// Editor curve property edit box
	// ------------------------------
	class ColorGradientProperty: public TPropertyField<Ref<ColorGradient>>
	{
	public:
		// Default constructor
		ColorGradientProperty(RefCounter* refCounter);

		// Copy constructor
		ColorGradientProperty(RefCounter* refCounter, const ColorGradientProperty& other);

		// Copy operator
		ColorGradientProperty& operator=(const ColorGradientProperty& other);

		SERIALIZABLE(ColorGradientProperty);
		CLONEABLE_REF(ColorGradientProperty);

	protected:
		// ---------------------------------------------------------------
		// Gradient preview drawable, used as layer of property box widget
		// ---------------------------------------------------------------
		class GradientPreviewDrawable : public IRectDrawable
		{
		public:
			// Sets gradient and updates mesh
			void SetGradient(const Ref<ColorGradient>& gradient);

			// Draws gradient
			void Draw() override;

		protected:
			Ref<ColorGradient> mGradient;
			Mesh               mMesh;

		protected:
			// Called when rectangle changed, updates gradient mesh
			void BasisChanged() override;

			// Updates gradient mesh
			void UpdateMesh();
		};

	protected:
		Ref<Widget>                  mBox;     // Gradient edit box 
		Ref<GradientPreviewDrawable> mPreview; // Gradient preview drawable

		Vector<Ref<WidgetDragHandle>> mHandles; // List of handles, each for keys

	protected:
		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Called when common curve changed in curve edit dialog, tells to drawing content
		void OnValueChanged();

		// Updates value view
		void UpdateValueView() override;

		// Creates new handle
		Ref<WidgetDragHandle> CreateHandle();

		// Creates handles for all keys
		void InitializeHandles();
	};
}
// --- META ---

CLASS_BASES_META(Editor::ColorGradientProperty)
{
    BASE_CLASS(Editor::TPropertyField<Ref<ColorGradient>>);
}
END_META;
CLASS_FIELDS_META(Editor::ColorGradientProperty)
{
    FIELD().PROTECTED().NAME(mBox);
    FIELD().PROTECTED().NAME(mPreview);
    FIELD().PROTECTED().NAME(mHandles);
}
END_META;
CLASS_METHODS_META(Editor::ColorGradientProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*);
    FUNCTION().PUBLIC().CONSTRUCTOR(RefCounter*, const ColorGradientProperty&);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, OnValueChanged);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(Ref<WidgetDragHandle>, CreateHandle);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeHandles);
}
END_META;
// --- END META ---
