#pragma once

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Image.h"
#include "o2/Core/SmartPtr.h"

using namespace o2;

namespace o2
{
	class WidgetDragHandle;
}

namespace Editor
{
	class BorderIProperty;

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
		void Setup(const Ref<ImageAsset>& image, BorderIProperty* borderProperty);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ImageSlicesEditorWidget);

	public:
		class PreviewImage: public Image
		{
		public:
			// Sets texture filter to nearest and draws it
			void Draw() override;

			// Returns create menu category in editor
			static String GetCreateMenuCategory();

			SERIALIZABLE(PreviewImage);
		};

	private:
		Ref<PreviewImage> mPreviewImage;
		Ref<Image>        mPreviewImageBack;

		Ref<WidgetDragHandle> mBorderLeftHandle;
		Ref<WidgetDragHandle> mBorderRightHandle;
		Ref<WidgetDragHandle> mBorderTopHandle;
		Ref<WidgetDragHandle> mBorderBottomHandle;
		BorderF              mBordersSmoothValue;

		WeakRef<BorderIProperty> mBorderProperty;

	protected:
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
		Ref<Sprite> CreateGridSprite();
	};
}
// --- META ---

CLASS_BASES_META(Editor::ImageSlicesEditorWidget)
{
    BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::ImageSlicesEditorWidget)
{
    FIELD().PRIVATE().NAME(mPreviewImage);
    FIELD().PRIVATE().NAME(mPreviewImageBack);
    FIELD().PRIVATE().NAME(mBorderLeftHandle);
    FIELD().PRIVATE().NAME(mBorderRightHandle);
    FIELD().PRIVATE().NAME(mBorderTopHandle);
    FIELD().PRIVATE().NAME(mBorderBottomHandle);
    FIELD().PRIVATE().NAME(mBordersSmoothValue);
    FIELD().PRIVATE().NAME(mBorderProperty);
}
END_META;
CLASS_METHODS_META(Editor::ImageSlicesEditorWidget)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const ImageSlicesEditorWidget&);
    FUNCTION().PUBLIC().SIGNATURE(void, Setup, const Ref<ImageAsset>&, BorderIProperty*);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeImagePreview);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeSliceHandles);
    FUNCTION().PROTECTED().SIGNATURE(void, FitImage);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateBordersAnchors);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateBordersValue);
    FUNCTION().PROTECTED().SIGNATURE(Ref<Sprite>, CreateGridSprite);
}
END_META;

CLASS_BASES_META(Editor::ImageSlicesEditorWidget::PreviewImage)
{
    BASE_CLASS(o2::Image);
}
END_META;
CLASS_FIELDS_META(Editor::ImageSlicesEditorWidget::PreviewImage)
{
}
END_META;
CLASS_METHODS_META(Editor::ImageSlicesEditorWidget::PreviewImage)
{

    FUNCTION().PUBLIC().SIGNATURE(void, Draw);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
}
END_META;
// --- END META ---