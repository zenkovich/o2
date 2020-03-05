#pragma once

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Image.h"

using namespace o2;

namespace o2
{
	class BorderIProperty;
}

namespace Editor
{
	class WidgetDragHandle;

	// -----------------------------
	// Editor of image slices widget
	// -----------------------------
	class ImageSlicesEditorWidget: public Widget
	{
	public:
		// Default constructor
		ImageSlicesEditorWidget();

		// Default copy-constructor
		ImageSlicesEditorWidget(const ImageSlicesEditorWidget& other);

		// Copy operator
		ImageSlicesEditorWidget& operator=(const ImageSlicesEditorWidget& other);

		// Sets image and property
		void Setup(const ImageAssetRef& image, BorderIProperty* borderProperty);

		SERIALIZABLE(ImageSlicesEditorWidget);

	protected:
		class PreviewImage: public Image
		{
		public:
			// Sets texture filter to nearest and draws it
			void Draw() override;

			SERIALIZABLE(PreviewImage);
		};

	private:
		Widget*       mPreviewImageContent;
		PreviewImage* mPreviewImage = nullptr;
		Image*        mPreviewImageBack = nullptr;

		WidgetDragHandle* mBorderLeftHandle;
		WidgetDragHandle* mBorderRightHandle;
		WidgetDragHandle* mBorderTopHandle;
		WidgetDragHandle* mBorderBottomHandle;
		BorderF           mBordersSmoothValue;

		BorderIProperty* mBorderProperty = nullptr;

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// Initializes image preview widgets and border handles
		void InitializeImagePreview();

		// Initializes image slices handles
		void InitializeSliceHandles();

		// Fits image by size
		void FitImage();

		// Updates borders anchors layouts
		void UpdateBordersAnchors();

		// Updates targets assets borders values by floored mBordersSmoothValue
		void UpdateBordersValue();

		// Creates grid sprite
		Sprite* CreateGridSprite();
	};
}

CLASS_BASES_META(Editor::CurvePreview)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::CurvePreview)
{
	PROTECTED_FIELD(mCurve);
	PROTECTED_FIELD(mNeedRedraw);
	PROTECTED_FIELD(mSprite);
	PROTECTED_FIELD(mBackColor);
	PROTECTED_FIELD(mCurveColor);
}
END_META;
CLASS_METHODS_META(Editor::CurvePreview)
{

	PUBLIC_FUNCTION(void, SetCurve, Curve*);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetBackColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetBackColor);
	PUBLIC_FUNCTION(void, SetCurveColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetCurveColor);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, Redraw);
	PROTECTED_FUNCTION(void, OnCurveChanged);
}
END_META;
