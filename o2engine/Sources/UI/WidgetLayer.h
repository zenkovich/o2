#pragma once

#include "Render/RectDrawable.h"
#include "Utils/ITree.h"
#include "Utils/Math/Layout.h"
#include "Utils/Serializable.h"
#include "Utils/String.h"

namespace o2
{
	class UIWidget;

	class UIWidgetLayer;
	typedef Vector<UIWidgetLayer*>  LayersVec;

	// ---------------------
	// Widget drawable layer
	// ---------------------
	class UIWidgetLayer: public ISerializable
	{
	public:
		typedef Vector<UIWidgetLayer*> ChildsVec;

	public:
		Layout                                  layout;             // Drawable layout @SERIALIZABLE
		Layout                                  interactableLayout; // Interactable area layout
		String                                  name;               // Name of layer @SERIALIZABLE
		Property<float>                         depth;              // Drawing depth (higher depths will draw later)
		Property<float>                         transparency;       // Drawable transparency property
		IRectDrawable*                          drawable;           // Drawable @SERIALIZABLE
		Accessor<UIWidgetLayer*, const String&> child;              // Child layer accessor

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
		UIWidgetLayer* AddChild(UIWidgetLayer* node);

		// Remove child layer and releases him if needs
		bool RemoveChild(UIWidgetLayer* node, bool release = true);

		// Removes and releases all children nodes
		void RemoveAllChilds();

		// Sets parent layer
		void SetParent(UIWidgetLayer* parent);

		// Returns parent layer
		UIWidgetLayer* GetParent() const;

		// Return child layers
		ChildsVec& GetChilds();

		// Returns constant child layers
		const ChildsVec& GetChilds() const;

		// Adds child layer
		UIWidgetLayer* AddChildLayer(const String& name, IRectDrawable* drawable, const Layout& layout = Layout::BothStretch(),
									 float depth = 0.0f);

		// Returns child layer by path ()
		UIWidgetLayer* GetChild(const String& path);

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

		// Returns layout rectangle
		const RectF& GetRect() const;

		// Returns child layer with type
		template<typename _type>
		_type* FindLayer() const;

		SERIALIZABLE(UIWidgetLayer);

	protected:
		float          mTransparency = 1.0f;    // Layer transparency @SERIALIZABLE
		float          mResTransparency = 1.0f; // Result drawable transparency, depends on parent transparency
		float          mDepth = 0.0f;           // Depth of drawable @SERIALIZABLE
		RectF          mAbsolutePosition;       // Result absolute drawable position
		RectF          mInteractableArea;       // Interactable area, depends on interactableLayout
		UIWidget*      mOwnerWidget = nullptr;  // Owner widget pointer
		UIWidgetLayer* mParent = nullptr;       // Pointer to parent layer
		ChildsVec      mChilds;                 // Children layers @SERIALIZABLE

	protected:
		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Sets owner widget for this and children
		void SetOwnerWidget(UIWidget* owner);

		// Calls when added new child layer, sets his owner same as this owner and calls UpdateLayersDrawingSequence in owner
		void OnChildAdded(UIWidgetLayer* child);

		// Updates drawable rect, calling when widget's layout was changed
		void UpdateLayout();

		// Calls when transparency was changed and updates children transparencies
		void UpdateResTransparency();

		// Returns dictionary with all child layers
		Dictionary<String, UIWidgetLayer*> GetAllChildLayers();

		// Initializes properties
		void InitializeProperties();

		friend class UIWidget;
	};

	template<typename _type>
	_type* UIWidgetLayer::FindLayer() const
	{
		for (auto child : mChilds)
			if (child->drawable && child->drawable->GetType() == TypeOf(_type))
				return (_type*)(child->drawable);

		for (auto child : mChilds)
		{
			auto res = child->FindLayer<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}
}
