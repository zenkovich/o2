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
		typedef Vector<UIWidgetLayer*> ChildrenVec;

	public:
		PROPERTIES(UIWidgetLayer);
		PROPERTY(float, depth, SetDepth, GetDepth);                      // Drawing depth (higher depths will draw later)
		PROPERTY(float, transparency, SetTransparency, GetTransparency); // Drawable transparency property

		ACCESSOR(UIWidgetLayer*, child, String, GetChild, GetAllChildLayers); // Child layer accessor

		String          name;               // Name of layer @SERIALIZABLE
					    
		Layout          layout;             // Drawable layout @SERIALIZABLE
		Layout          interactableLayout; // Interactable area layout @SERIALIZABLE
					    
		IRectDrawable*  drawable;           // Drawable @SERIALIZABLE


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
		ChildrenVec& GetChilds();

		// Returns constant child layers
		const ChildrenVec& GetChilds() const;

		// Adds child layer
		UIWidgetLayer* AddChildLayer(const String& name, IRectDrawable* drawable, const Layout& layout = Layout::BothStretch(),
									 float depth = 0.0f);

		// Returns child layer by path
		UIWidgetLayer* GetChild(const String& path);

		// Returns child layer by name
		UIWidgetLayer* FindChild(const String& name);

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

		// Returns result transparency
		float GetResTransparency() const;

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
		UIWidget*      mOwnerWidget = nullptr;  // Owner widget pointer @EXCLUDE_POINTER_SEARCH
		UIWidgetLayer* mParent = nullptr;       // Pointer to parent layer @EXCLUDE_POINTER_SEARCH
		ChildrenVec    mChildren;               // Children layers @SERIALIZABLE

	protected:
		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Sets owner widget for this and children
		void SetOwnerWidget(UIWidget* owner);

		// It is called when added new child layer, sets his owner same as this owner and calls UpdateLayersDrawingSequence in owner
		void OnChildAdded(UIWidgetLayer* child);

		// Updates drawable rect, calling when widget's layout was changed
		void UpdateLayout();

		// It is called when transparency was changed and updates children transparencies
		void UpdateResTransparency();

		// Returns dictionary with all child layers
		Dictionary<String, UIWidgetLayer*> GetAllChildLayers();

		friend class UIWidget;
	};

	template<typename _type>
	_type* UIWidgetLayer::FindLayer() const
	{
		for (auto child : mChildren)
			if (child->drawable && child->drawable->GetType() == TypeOf(_type))
				return (_type*)(child->drawable);

		for (auto child : mChildren)
		{
			auto res = child->FindLayer<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}
}

CLASS_BASES_META(o2::UIWidgetLayer)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::UIWidgetLayer)
{
	PUBLIC_FIELD(depth);
	PUBLIC_FIELD(transparency);
	PUBLIC_FIELD(child);
	PUBLIC_FIELD(name).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(layout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(interactableLayout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(drawable).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mTransparency).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mResTransparency);
	PROTECTED_FIELD(mDepth).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mAbsolutePosition);
	PROTECTED_FIELD(mInteractableArea);
	PROTECTED_FIELD(mOwnerWidget).EXCLUDE_POINTER_SEARCH_ATTRIBUTE();
	PROTECTED_FIELD(mParent).EXCLUDE_POINTER_SEARCH_ATTRIBUTE();
	PROTECTED_FIELD(mChildren).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::UIWidgetLayer)
{

	typedef Dictionary<String, UIWidgetLayer*> _tmp1;

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(UIWidgetLayer*, AddChild, UIWidgetLayer*);
	PUBLIC_FUNCTION(bool, RemoveChild, UIWidgetLayer*, bool);
	PUBLIC_FUNCTION(void, RemoveAllChilds);
	PUBLIC_FUNCTION(void, SetParent, UIWidgetLayer*);
	PUBLIC_FUNCTION(UIWidgetLayer*, GetParent);
	PUBLIC_FUNCTION(ChildrenVec&, GetChilds);
	PUBLIC_FUNCTION(const ChildrenVec&, GetChilds);
	PUBLIC_FUNCTION(UIWidgetLayer*, AddChildLayer, const String&, IRectDrawable*, const Layout&, float);
	PUBLIC_FUNCTION(UIWidgetLayer*, GetChild, const String&);
	PUBLIC_FUNCTION(UIWidgetLayer*, FindChild, const String&);
	PUBLIC_FUNCTION(LayersVec, GetAllChilds);
	PUBLIC_FUNCTION(void, SetDepth, float);
	PUBLIC_FUNCTION(float, GetDepth);
	PUBLIC_FUNCTION(void, SetTransparency, float);
	PUBLIC_FUNCTION(float, GetTransparency);
	PUBLIC_FUNCTION(float, GetResTransparency);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(const RectF&, GetRect);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
	PROTECTED_FUNCTION(void, SetOwnerWidget, UIWidget*);
	PROTECTED_FUNCTION(void, OnChildAdded, UIWidgetLayer*);
	PROTECTED_FUNCTION(void, UpdateLayout);
	PROTECTED_FUNCTION(void, UpdateResTransparency);
	PROTECTED_FUNCTION(_tmp1, GetAllChildLayers);
}
END_META;
