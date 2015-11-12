#pragma once

#include "Render/RectDrawable.h"
#include "Utils/Math/Layout.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Serialization.h"
#include "Utils/String.h"
#include "Utils/Tree.h"

namespace o2
{
	class UIWidget;

	class UIWidgetLayer;
	typedef Vector<Ptr<UIWidgetLayer>>  LayersVec;

	// ---------------------
	// Widget drawable layer
	// ---------------------
	class UIWidgetLayer: public ISerializable, public ITreeNode<UIWidgetLayer>
	{
	public:
		Layout                                      layout;             // Drawable layout
		Layout                                      interactableLayout; // Interactable area layout
		String                                      name;               // Name of layer
		Property<float>                             depth;              // Drawing depth (higher depths will draw later)
		Property<float>                             transparency;       // Drawable transparency property
		Ptr<IRectDrawable>                          drawable;           // Drawable
		Accessor<Ptr<UIWidgetLayer>, const String&> child;              // Child layer accessor

		// Default constructor
		UIWidgetLayer();

		// Copy-constructor
		UIWidgetLayer(const UIWidgetLayer& other);

		// Destructor
		~UIWidgetLayer();

		// Copy-operator
		UIWidgetLayer& operator=(const UIWidgetLayer& other);

		// Draws drawable
		void Draw();

		// Updates drawable and layout
		void Update(float dt);

		// Adds child layer
		Ptr<UIWidgetLayer> AddChildLayer(const String& name, Ptr<IRectDrawable> drawable, const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Returns child layer by path ()
		Ptr<UIWidgetLayer> GetChild(const String& path);

		// Returns all child layers
		LayersVec GetAllChilds() const;

		// Sets depth
		void SetDepth(float depth);

		// Returns depth
		float GetDepth() const;

		// Sets transparency and updates this and children result transparencies
		void SetTransparency(float transparency);

		// Returns transparency
		float GetTransparency();

		// Returns true if layer is under point
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE_IMPL(UIWidgetLayer);

		IOBJECT(UIWidgetLayer)
		{
			SRLZ_FIELD(layout);
			SRLZ_FIELD(name);
			SRLZ_FIELD(mDepth);
			SRLZ_FIELD(drawable);
			SRLZ_FIELD(mTransparency);
			SRLZ_FIELD(mChilds);

			FIELD(mAbsolutePosition);
			FIELD(depth);
			FIELD(transparency);
			FIELD(child);
			//FIELD(mOwnerWidget);
		}

	protected:
		float         mTransparency;     // Layer transparency
		float         mResTransparency;  // Result drawable transparency, depends on parent transparency
		float         mDepth;            // Depth of drawable
		RectF         mAbsolutePosition; // Result absolute drawable position
		RectF         mInteractableArea; // Interactable area, depends on interactableLayout
		Ptr<UIWidget> mOwnerWidget;      // Owner widget pointer

	protected:
		// Sets owner widget for this and children
		void SetOwnerWidget(Ptr<UIWidget> owner);

		// Calls when added new child layer, sets his owner same as this owner and calls UpdateLayersDrawingSequence in owner
		void OnChildAdded(Ptr<UIWidgetLayer> child);

		// Updates drawable rect, calling when widget's layout was changed
		void UpdateLayout();

		// Calls when transparency was changed and updates children transparencies
		void UpdateResTransparency();

		// Returns dictionary with all child layers
		Dictionary<String, Ptr<UIWidgetLayer>> GetAllChildLayers();

		// Initializes properties
		void InitializeProperties();

		friend class UIWidget;
	};
}
