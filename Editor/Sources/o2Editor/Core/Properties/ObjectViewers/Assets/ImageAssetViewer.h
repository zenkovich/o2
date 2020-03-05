#pragma once

#include "o2Editor/Core/Properties/ObjectViewers/DefaultObjectViewer.h"

using namespace o2;

namespace o2
{
	class Button;
	class WidgetDragHandle;
}

namespace Editor
{
	class BorderIProperty;

	// ------------------
	// Image asset viewer
	// ------------------
	class ImageAssetViewer : public DefaultObjectViewer
	{
	public:
		// Default constructor. Initializes fields controls
		ImageAssetViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(ImageAssetViewer);

	private:
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

	private:
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

CLASS_BASES_META(Editor::ImageAssetViewer)
{
	BASE_CLASS(Editor::DefaultObjectViewer);
}
END_META;
CLASS_FIELDS_META(Editor::ImageAssetViewer)
{
	PRIVATE_FIELD(mPreviewImageContent);
	PRIVATE_FIELD(mPreviewImage);
	PRIVATE_FIELD(mPreviewImageBack);
	PRIVATE_FIELD(mBorderLeftHandle);
	PRIVATE_FIELD(mBorderRightHandle);
	PRIVATE_FIELD(mBorderTopHandle);
	PRIVATE_FIELD(mBorderBottomHandle);
	PRIVATE_FIELD(mBordersSmoothValue);
	PRIVATE_FIELD(mBorderProperty);
}
END_META;
CLASS_METHODS_META(Editor::ImageAssetViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PRIVATE_FUNCTION(void, InitializeImagePreview);
	PRIVATE_FUNCTION(void, InitializeSliceHandles);
	PRIVATE_FUNCTION(void, FitImage);
	PRIVATE_FUNCTION(void, UpdateBordersAnchors);
	PRIVATE_FUNCTION(void, UpdateBordersValue);
	PRIVATE_FUNCTION(Sprite*, CreateGridSprite);
}
END_META;

CLASS_BASES_META(Editor::ImageAssetViewer::PreviewImage)
{
	BASE_CLASS(o2::Image);
}
END_META;
CLASS_FIELDS_META(Editor::ImageAssetViewer::PreviewImage)
{
}
END_META;
CLASS_METHODS_META(Editor::ImageAssetViewer::PreviewImage)
{

	PUBLIC_FUNCTION(void, Draw);
}
END_META;
