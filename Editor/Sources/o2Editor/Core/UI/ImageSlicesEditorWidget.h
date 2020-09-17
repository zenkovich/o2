#pragma once

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Image.h"

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
		void Setup(const ImageAssetRef& image, BorderIProperty* borderProperty);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ImageSlicesEditorWidget);

	protected:
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
		PreviewImage* mPreviewImage = nullptr;
		Image*        mPreviewImageBack = nullptr;

		WidgetDragHandle* mBorderLeftHandle;
		WidgetDragHandle* mBorderRightHandle;
		WidgetDragHandle* mBorderTopHandle;
		WidgetDragHandle* mBorderBottomHandle;
		BorderF           mBordersSmoothValue;

		BorderIProperty* mBorderProperty = nullptr;

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
		Sprite* CreateGridSprite();
	};
}

CLASS_BASES_META(Editor::ImageSlicesEditorWidget)
{
	BASE_CLASS(o2::Widget);
}
END_META;
CLASS_FIELDS_META(Editor::ImageSlicesEditorWidget)
{
	PRIVATE_FIELD(mPreviewImage).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mPreviewImageBack).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mBorderLeftHandle);
	PRIVATE_FIELD(mBorderRightHandle);
	PRIVATE_FIELD(mBorderTopHandle);
	PRIVATE_FIELD(mBorderBottomHandle);
	PRIVATE_FIELD(mBordersSmoothValue);
	PRIVATE_FIELD(mBorderProperty).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::ImageSlicesEditorWidget)
{

	PUBLIC_FUNCTION(void, Setup, const ImageAssetRef&, BorderIProperty*);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PROTECTED_FUNCTION(void, InitializeImagePreview);
	PROTECTED_FUNCTION(void, InitializeSliceHandles);
	PROTECTED_FUNCTION(void, FitImage);
	PROTECTED_FUNCTION(void, UpdateBordersAnchors);
	PROTECTED_FUNCTION(void, UpdateBordersValue);
	PROTECTED_FUNCTION(Sprite*, CreateGridSprite);
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

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
}
END_META;
