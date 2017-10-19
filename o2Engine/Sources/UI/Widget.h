#pragma once

#include "Scene/Actor.h"
#include "Scene/Drawable.h"
#include "Utils/Math/Layout.h"

namespace o2
{
	class IRectDrawable;
	class UIWidgetLayer;
	class UIWidgetLayout;
	class UIWidgetState;

	// ------------------------------------------------------
	// Basic UI Widget. Its a simple and basic element of UI, 
	// everything other UI's are based on this
	// ------------------------------------------------------
	class UIWidget: public Actor, public SceneDrawable
	{
	public:
		typedef Vector<UIWidget*> WidgetsVec;
		typedef Vector<UIWidgetLayer*> LayersVec;
		typedef Vector<UIWidgetState*> StatesVec;

	public:
		UIWidgetLayout* const layout;          // Widget's layout
							  
		Property<bool>        visible;         // Is widget visible property
							  
		Property<float>       transparency;    // Transparency property
		Getter<float>         resTransparency; // Result transparency getter, depends on parent transparency
							  
		Property<UIWidget*>   parentWidget;    // Parent widget property
							  
		Getter<WidgetsVec>    childrenWidgets; // Widget children getter
							  
		Getter<LayersVec>     layers;          // Layers getter
							  
		Getter<StatesVec>     states;          // States getter

		Accessor<UIWidget*, const String&>      childWidget; // Widget child accessor by path like "child/subchild/somechild"
		Accessor<UIWidgetLayer*, const String&> layer;       // Widget layer accessor by path like "layer/sublayer/target"
		Accessor<UIWidgetState*, const String&> state;       // Widget state accessor by name


		Function<void()> onLayoutUpdated; // Layout change event
		Function<void()> onFocused;       // Widget focused event
		Function<void()> onUnfocused;     // Widget unfocused event
		Function<void()> onShow;          // Widget showing vent
		Function<void()> onHide;          // Widget hiding event

	public:
		// Default constructor
		UIWidget(ActorCreateMode mode = ActorCreateMode::InScene);

		// Widget constructor from prototype
		UIWidget(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::InScene);

		// Widget constructor with components
		UIWidget(ComponentsVec components, ActorCreateMode mode = ActorCreateMode::InScene);

		// Copy-constructor
		UIWidget(const UIWidget& other);

		// Virtual destructor
		virtual ~UIWidget();

		// Copy-operator
		UIWidget& operator=(const UIWidget& other);

		// Updates layers, states and widget
		void Update(float dt) override;

		// Updates layout
		virtual void UpdateLayout(bool withChildren = true);

		// Updates children layouts
		virtual void UpdateChildrenLayouts();

		// Draws widget and child widgets with not overridden depth
		void Draw() override;

		// Forcible drawing in area with transparency
		void ForceDraw(const RectF& area, float transparency);

		// Returns parent widget
		UIWidget* GetParentWidget() const;

		// Returns child widget by path (like "root/some node/other node/target node")
		UIWidget* GetChildWidget(const String& path) const;

		// Adds child widget and returns them
		UIWidget* AddChildWidget(UIWidget* widget);

		// Adds child widget at position and returns them
		UIWidget* AddChildWidget(UIWidget* widget, int position);

		// Returns constant children widgets vector
		const WidgetsVec& GetChildWidgets() const;

		// Adds layer
		UIWidgetLayer* AddLayer(UIWidgetLayer* layer);

		// Adds layer
		UIWidgetLayer* AddLayer(const String& name, IRectDrawable* drawable,
								const Layout& layout = Layout::BothStretch(), float depth = 0.0f);

		// Removes layer
		bool RemoveLayer(UIWidgetLayer* layer);

		// Removes layer
		bool RemoveLayer(const String& path);

		// Removes all layers
		void RemoveAllLayers();

		// Returns layer by path. Returns null if layer isn't exist
		UIWidgetLayer* GetLayer(const String& path) const;

		// Searches layer with drawable with specified type
		template<typename _type>
		_type* GetLayerDrawableByType();

		// Returns layer by path. Returns null if layer isn't exist or layer drawable has different type
		template<typename _type>
		_type* GetLayerDrawable(const String& path) const;

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

		// Sets state value immediately
		void SetStateForcible(const String& name, bool state);

		// Returns state value
		bool GetState(const String& name) const;

		// Returns state object by name
		UIWidgetState* GetStateObject(const String& name) const;

		// Returns all states
		const StatesVec& GetStates() const;

		// Sets depth overriding
		void SetDepthOverridden(bool overrideDepth);

		// Is sorting depth overridden
		bool IsDepthOverriden() const;

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

		// Focus this widget
		void Focus();

		// Unfocus this widget
		void Unfocus();

		// Returns is this widget focused
		bool IsFocused() const;

		// Returns is this widget can be focused
		virtual bool IsFocusable() const;

		// Sets widget can be focused
		void SetFocusable(bool focusable);

		// Returns true if point is under drawable
		bool IsUnderPoint(const Vec2F& point);

		SERIALIZABLE(UIWidget);

	protected:
		using Actor::mLayer;

		String         mName;                   // Name @SERIALIZABLE
					   						    
		LayersVec      mLayers;                 // Layers array @SERIALIZABLE
		StatesVec      mStates;                 // States array @SERIALIZABLE
					   						    
		UIWidget*      mParentWidget;           // Parent widget. When parent is not widget, this field will be null
		WidgetsVec     mChildWidgets;           // Children widgets, a part of all children
		WidgetsVec     mDrawingChildren;        // Children widgets, which drawing depth isn't overridden

		RectF          mChildrenWorldRect;      // World rectangle for children arranging
					   						    
		bool           mOverrideDepth = false;  // Is sorting order depth overridden. If not, sorting order depends on hierarchy @SERIALIZABLE

		float          mTransparency = 1.0f;	// Widget transparency @SERIALIZABLE
		float          mResTransparency = 1.0f; // Widget result transparency, depends on parent's result transparency

		LayersVec      mDrawingLayers;          // Layers ordered by depth, which drawing before children (depth < 1000)
		LayersVec      mTopDrawingLayers;       // Layers ordered by depth, which drawing after children (depth > 1000)
					   						    
		UIWidgetState* mFocusedState = nullptr; // Focused widget state
		bool           mIsFocused = false;      // Is widget focused
		bool           mIsFocusable = false;    // Is widget can be focused @SERIALIZABLE
					   						    
		UIWidgetState* mVisibleState = nullptr; // Widget visibility state
		bool           mVisible = true;         // Visibility of widget. Uses state 'visible' @SERIALIZABLE
		bool           mResVisible = true;      // Result visibility of widget. Depends on this visibility and parent result visibility

		bool           mFullyDisabled = false;  // True, if widget is not visible and visible state is fully false
		bool           mIsClipped = false;      // Is widget fully clipped by some scissors

		RectF          mBounds;                 // Widget bounds by drawing layers
		RectF          mBoundsWithChilds;       // Widget with childs bounds

	protected:
		// Draws debug frame by mAbsoluteRect
		void DrawDebugFrame();

		// It is called when transformation was changed and updated
		void OnTransformUpdated() override;

		// It is called when parent changed
		void OnParentChanged(Actor* oldParent) override;

		// It is called when child actor was added
		void OnChildAdded(Actor* child) override;

		// It is called when child actor was removed
		void OnChildRemoved(Actor* child) override;

		// It is called when layer was changed
		void OnLayerChanged(SceneLayer* oldLayer) override;

		// It is called when child widget was added
		virtual void OnChildAdded(UIWidget* child);

		// It is called when child widget was removed
		virtual void OnChildRemoved(UIWidget* child);

		// It is called when widget was selected
		virtual void OnFocused();

		// It is called when widget was deselected
		virtual void OnUnfocused();

		// Returns layout width
		virtual float GetMinWidthWithChildren() const;

		// Returns layout height
		virtual float GetMinHeightWithChildren() const;

		// Updates bounds by drawing layers
		virtual void UpdateBounds();

		// Updates bound with children
		virtual void UpdateBoundsWithChilds();

		// Checks widget clipping by area
		virtual void CheckClipping(const RectF& clipArea);

		// Updates transparency for this and children widgets
		virtual void UpdateTransparency();

		// Updates result visibility
		virtual void UpdateVisibility(bool updateLayout = true);

		// Updates layers layouts, calls after updating widget layout
		virtual void UpdateLayersLayouts();

		// Updates drawing children widgets list
		void UpdateDrawingChildren();

		// Updates layers drawing sequence
		void UpdateLayersDrawingSequence();

		// It is called when child widget was selected
		virtual void OnChildFocused(UIWidget* child);

		// It is called when layer added and updates drawing sequence
		virtual void OnLayerAdded(UIWidgetLayer* layer);

		// It is called when widget state was added
		virtual void OnStateAdded(UIWidgetState* state);

		// It is called when visible was changed
		virtual void OnVisibleChanged();

		// Sets new target for all states animations
		void RetargetStatesAnimations();

		// Sets parent widget, used for property
		void SetParentWidget(UIWidget* widget);

		// Returns children widgets (for property)
		WidgetsVec GetChildrenNonConst();

		// Returns layers (for property)
		LayersVec GetLayersNonConst();

		// Returns states (for property)
		StatesVec GetStatesNonConst();

		// Returns dictionary of all layers by names
		Dictionary<String, UIWidgetLayer*> GetAllLayers();

		// Returns dictionary of all children by names
		Dictionary<String, UIWidget*> GetAllChilds();

		// Beginning serialization callback
		void OnSerialize(DataNode& node) const override;

		// It is called when deserialized
		void OnDeserialized(const DataNode& node) override;

		// Initializes properties
		void InitializeProperties();

		friend class UIContextMenu;
		friend class UICustomDropDown;
		friend class UICustomList;
		friend class UIDropDown;
		friend class UIEditBox;
		friend class UIGridLayout;
		friend class UIHorizontalLayout;
		friend class UIHorizontalProgress;
		friend class UIHorizontalScrollBar;
		friend class UILabel;
		friend class UIList;
		friend class UILongList;
		friend class UIManager;
		friend class UIMenuPanel;
		friend class UIScrollArea;
		friend class UISpoiler;
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
	_type* UIWidget::GetLayerDrawableByType()
	{
		for (auto layer : mLayers)
		{
			if (layer->drawable && layer->drawable->GetType() == TypeOf(_type))
				return (_type*)layer->drawable;
		}

		for (auto layer : mLayers)
		{
			auto res = layer->GetLayerDrawableByType<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}

	template<typename _type>
	_type* UIWidget::GetLayerDrawable(const String& path) const
	{
		auto layer = GetLayer(path);
		if (layer && layer->drawable && layer->drawable->GetType() == TypeOf(_type))
			return (_type*)layer->drawable;

		return nullptr;
	}
}
