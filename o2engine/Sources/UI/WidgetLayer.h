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
	class UIWidgetLayer: public ISerializable
	{
	public:
		typedef Vector<Ptr<UIWidgetLayer>> ChildsVec;

	public:
		Layout                                      layout;             // Drawable layout @SERIALIZABLE
		Layout                                      interactableLayout; // Interactable area layout
		String                                      name;               // Name of layer @SERIALIZABLE
		Property<float>                             depth;              // Drawing depth (higher depths will draw later)
		Property<float>                             transparency;       // Drawable transparency property
		Ptr<IRectDrawable>                          drawable;           // Drawable @SERIALIZABLE
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

		// Adds new child layer and returns him
		Ptr<UIWidgetLayer> AddChild(Ptr<UIWidgetLayer> node);

		// Remove child layer and releases him if needs
		bool RemoveChild(Ptr<UIWidgetLayer> node, bool release = true);

		// Removes and releases all children nodes
		void RemoveAllChilds();

		// Sets parent layer
		void SetParent(Ptr<UIWidgetLayer> parent);

		// Returns parent layer
		Ptr<UIWidgetLayer> GetParent() const;

		// Return child layers
		ChildsVec& GetChilds();

		// Returns constant child layers
		const ChildsVec& GetChilds() const;

		// Adds child layer
		Ptr<UIWidgetLayer> AddChildLayer(const String& name, Ptr<IRectDrawable> drawable, const Layout& layout = Layout::BothStretch(), float depth = 0.0f);

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

		SERIALIZABLE(UIWidgetLayer);

	protected:
		float              mTransparency;     // Layer transparency @SERIALIZABLE
		float              mResTransparency;  // Result drawable transparency, depends on parent transparency
		float              mDepth;            // Depth of drawable @SERIALIZABLE
		RectF              mAbsolutePosition; // Result absolute drawable position
		RectF              mInteractableArea; // Interactable area, depends on interactableLayout
		Ptr<UIWidget>      mOwnerWidget;      // Owner widget pointer
		Ptr<UIWidgetLayer> mParent;           // Pointer to parent layer
		ChildsVec          mChilds;           // Children layers @SERIALIZABLE

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
