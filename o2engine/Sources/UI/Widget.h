#pragma once

#include "Render/Text.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Math/Layout.h"

#include "Utils/Property.h"
#include "Utils/Serialization.h"
#include "Utils/Tree.h"

namespace o2
{
	class ImageAsset;
	class VectorFontAsset;
	class BitmapFontAsset;

	// ------------------------------------------------------
	// Basic UI Widget. Its a simple and basic element of UI, 
	// everything other UI's are based on this
	// ------------------------------------------------------
	class UIWidget: public ISerializable
	{
	public:
		typedef Vector<UIWidget*> WidgetsVec;

	public:
		Property<String>                        name;
		Property<UIWidget*>                     parent;
		Getter<WidgetsVec>                      childs;
		Getter<LayersVec>                       layers;
		Getter<StatesVec>                       states;
		Property<float>                         transparency;
		Getter<float>                           resTransparency;
		Property<bool>                          visible;
		Accessor<UIWidget*, const String&>      child;
		Accessor<UIWidgetLayer*, const String&> layer;
		Accessor<UIWidgetState*, const String&> state;
		UIWidgetLayout                          layout; // @SERIALIZABLE

		// Default constructor
		UIWidget();

		// Copy-constructor
		UIWidget(const UIWidget& other);

		// Virtual destructor
		virtual ~UIWidget();

		// Copy-operator
		UIWidget& operator=(const UIWidget& other);

		// Updates drawables, states and widget
		virtual void Update(float dt);

		// Draws widget
		virtual void Draw();

		// Sets widget name
		virtual void SetName(const String& name);

		// Returns widget name
		virtual String GetName() const;

		// Returns parent widget
		virtual UIWidget* GetParent() const;

		// Sets widget parent
		virtual void SetParent(UIWidget* parent);

		// Adds child widget
		virtual UIWidget* AddChild(UIWidget* widget);

		// Adds child widget at index
		virtual UIWidget* AddChild(UIWidget* widget, int index);

		// Removes child by path
		virtual bool RemoveChild(const String& path);

		// Removes child widget
		virtual bool RemoveChild(UIWidget* widget, bool release = true);

		// Returns child by path. Returns nullptr if isn't exist
		virtual UIWidget* GetChild(const String& path);

		// Searches child with specified type
		template<typename _type>
		_type* FindChild();

		// Searches layer with drawable with specified type
		template<typename _type>
		_type* FindLayer();

		// Removes all child widgets
		virtual void RemoveAllChilds();

		// Returns constant children widgets vector
		virtual const WidgetsVec& GetChilds() const;

		// Adds layer
		UIWidgetLayer* AddLayer(UIWidgetLayer* layer);

		// Adds layer
		UIWidgetLayer* AddLayer(const String& name, IRectDrawable* drawable,
								const Layout& layout = Layout::BothStretch(), float depth = 0.0f);

		// Returns layer by path. Returns null if layer isn't exist
		UIWidgetLayer* GetLayer(const String& path) const;

		// Returns layer by path. Returns null if layer isn't exist or layer drawable has different type
		template<typename _type>
		_type* GetLayerDrawable(const String& path) const;

		// Removes layer
		bool RemoveLayer(UIWidgetLayer* layer);

		// Removes layer
		bool RemoveLayer(const String& path);

		// Removes all layers
		void RemoveAllLayers();

		// Returns all layers
		const LayersVec& GetLayers() const;

		// Adds new state with name
		UIWidgetState* AddState(const String& name);

		// Adds new state with name and animation
		UIWidgetState* AddState(const String& name, const Animation& animation);

		// Adds state
		UIWidgetState* AddState(UIWidgetState* state);

		// Removes state by name
		bool RemoveState(const String& name);

		// Removes state
		bool RemoveState(UIWidgetState* state);

		// Removes all states
		void RemoveAllStates();

		// Sets state value
		void SetState(const String& name, bool state);

		// Returns state value
		bool GetState(const String& name) const;

		// Returns state object by name
		UIWidgetState* GetStateObject(const String& name) const;

		// Returns all states
		const StatesVec& GetStates() const;

		// Sets widget's transparency
		void SetTransparency(float transparency);

		// Returns widget's transparency
		float GetTransparency() const;

		// Returns widget's result transparency (depends on parent's result transparency)
		float GetResTransparency() const;

		// Sets visibility
		void SetVisible(bool visible);

		// Sets visibility
		void SetVisibleForcible(bool visible);

		// Sets visibility to true
		void Show(bool forcible = false);

		// Sets visibility to false
		void Hide(bool forcible = false);

		// Returns visibility
		bool IsVisible() const;

		// Selects this widget
		void Select();

		// Deselects this widget
		void Deselect();

		// Returns is this widget selected
		bool IsSelected() const;

		// Returns is this widget can be selected
		virtual bool IsSelectable() const;

		SERIALIZABLE(UIWidget);

	protected:
		String         mName;             // Name @SERIALIZABLE
					   
		LayersVec      mLayers;           // Layers array @SERIALIZABLE
		StatesVec      mStates;           // States array @SERIALIZABLE
					   
		UIWidget*      mParent;           // Parent widget
		WidgetsVec     mChilds;           // Children widgets @SERIALIZABLE
		RectF          mChildsAbsRect;    // Absolute rectangle for children arranging
					   
		float          mTransparency;	  // Widget transparency @SERIALIZABLE
		float          mResTransparency;  // Widget result transparency, depends on parent's result transparency
		LayersVec      mDrawingLayers;    // Layers ordered by depth, which drawing before children (depth < 1000)
		LayersVec      mTopDrawingLayers; // Layers ordered by depth, which drawing after children (depth > 1000)
					   
		UIWidgetState* mSelectedState;    // Selected widget state
		bool           mIsSelected;       // Is widget selected
					   
		UIWidgetState* mVisibleState;     // Widget visibility state
		bool           mVisible;          // Visibility of widget. Uses state 'visible'
		bool           mResVisible;       // Result visibility of widget. Depends on this visibility and parent result visibility
		bool           mFullyDisabled;    // True, if widget is not visible and visible state is fully false

	protected:
		// Draws debug frame by mAbsoluteRect
		void DrawDebugFrame();

		// Calls when widget was selected
		virtual void OnSelected();

		// Calls when widget was deselected
		virtual void OnDeselected();

		// Updates layout
		virtual void UpdateLayout(bool forcible = false);

		// Updates transparency for this and children widgets
		virtual void UpdateTransparency();

		// Updates result visibility
		virtual void UpdateVisibility();

		// Calls when child widget was selected
		virtual void OnChildSelected(UIWidget* child);

		// Sets target for all states animations
		void RetargetStatesAnimations();

		// Recalculates absolute and local rectangles
		void RecalculateAbsRect();

		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts();

		// Updates layers drawing sequence
		void UpdateLayersDrawingSequence();

		// Returns depth of last drawn layer
		float GetMaxDrawingDepth();

		// Returns children widgets (for property)
		WidgetsVec GetChildsNonConst();

		// Returns layers (for property)
		LayersVec GetLayersNonConst();

		// Returns states (for property)
		StatesVec GetStatesNonConst();

		// Returns dictionary of all layers by names
		Dictionary<String, UIWidgetLayer*> GetAllLayers();

		// Returns dictionary of all children by names
		Dictionary<String, UIWidget*> GetAllChilds();

		// Calls when layer added and updates drawing sequence
		virtual void OnLayerAdded(UIWidgetLayer* layer);

		// Calls when widget state was added
		virtual void OnStateAdded(UIWidgetState* state);

		// Calls when child widget was added
		virtual void OnChildAdded(UIWidget* child);

		// Calls when child widget was removed
		virtual void OnChildRemoved(UIWidget* child);

		// Calls when deserialized
		void OnDeserialized(const DataNode& node);

		// Forcible drawing in area
		void ForceDraw(const RectF& area, float transparency);

		// Calls when visible was changed
		virtual void OnVisibleChanged();

		// Initializes properties
		void InitializeProperties();

		friend class UIContextMenu;
		friend class UICustomDropDown;
		friend class UICustomList;
		friend class UIDropDown;
		friend class UIEditBox;
		friend class UIHorizontalLayout;
		friend class UIHorizontalProgress;
		friend class UIHorizontalScrollBar;
		friend class UILabel;
		friend class UIList;
		friend class UILongList;
		friend class UIManager;
		friend class UIMenuPanel;
		friend class UIScrollArea;
		friend class UITree;
		friend class UITreeNode;
		friend class UIVerticalLayout;
		friend class UIVerticalProgress;
		friend class UIVerticalScrollBar;
		friend class UIWidgetLayer;
		friend class UIWidgetLayout;
		friend class UIWindow;
	};

	template<typename _type>
	_type* UIWidget::FindChild()
	{
		for (auto child : mChilds)
			if (child->GetType() == *_type::type)
				return (_type*)child;

		return nullptr;
	}

	template<typename _type>
	_type* UIWidget::FindLayer()
	{
		for (auto layer : mLayers)
		{
			if (layer->drawable && layer->drawable->GetType() == *_type::type)
				return (_type*)layer->drawable;
		}

		for (auto layer : mLayers)
		{
			auto res = layer->FindLayer<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}

	template<typename _type>
	_type* UIWidget::GetLayerDrawable(const String& path) const
	{
		auto layer = GetLayer(path);
		if (layer && layer->drawable && layer->drawable->GetType() == *_type::type)
			return (_type*)layer->drawable;

		return nullptr;
	}
}