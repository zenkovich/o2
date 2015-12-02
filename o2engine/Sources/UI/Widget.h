#pragma once

#include "Render/Text.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Math/Layout.h"
#include "Utils/Memory/Ptr.h"
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
		typedef Vector<Ptr<UIWidget>> WidgetsVec;

	public:
		Property<String>                            name;
		Property<Ptr<UIWidget>>                     parent;
		Getter<WidgetsVec>                          childs;
		Getter<LayersVec>                           layers;
		Getter<StatesVec>                           states;
		Property<float>                             transparency;
		Getter<float>                               resTransparency;
		Property<bool>                              visible;
		Accessor<Ptr<UIWidget>, const String&>      child;
		Accessor<Ptr<UIWidgetLayer>, const String&> layer;
		Accessor<Ptr<UIWidgetState>, const String&> state;
		UIWidgetLayout                              layout;

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
		virtual Ptr<UIWidget> GetParent() const;

		// Sets widget parent
		virtual void SetParent(Ptr<UIWidget> parent);

		// Adds child widget
		virtual Ptr<UIWidget> AddChild(Ptr<UIWidget> widget);

		// Adds child widget at index
		virtual Ptr<UIWidget> AddChild(Ptr<UIWidget> widget, int index);

		// Removes child by path
		virtual bool RemoveChild(const String& path);

		// Removes child widget
		virtual bool RemoveChild(Ptr<UIWidget> widget, bool release = true);

		// Returns child by path. Returns nullptr if isn't exist
		virtual Ptr<UIWidget> GetChild(const String& path);

		// Searches child with specified type
		template<typename _type>
		Ptr<_type> FindChild();

		// Removes all child widgets
		virtual void RemoveAllChilds();

		// Returns constant children widgets vector
		virtual const WidgetsVec& GetChilds() const;

		// Adds layer
		Ptr<UIWidgetLayer> AddLayer(Ptr<UIWidgetLayer> layer);

		// Adds layer
		Ptr<UIWidgetLayer> AddLayer(const String& name, Ptr<IRectDrawable> drawable,
									const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Returns layer by path. Returns null if layer isn't exist
		Ptr<UIWidgetLayer> GetLayer(const String& path) const;

		// Returns layer by path. Returns null if layer isn't exist or layer drawable has different type
		template<typename _type>
		Ptr<_type> GetLayerDrawable(const String& path) const;

		// Removes layer
		bool RemoveLayer(Ptr<UIWidgetLayer> layer);

		// Removes layer
		bool RemoveLayer(const String& path);

		// Removes all layers
		void RemoveAllLayers();

		// Returns all layers
		const LayersVec& GetLayers() const;

		// Adds new state with name
		Ptr<UIWidgetState> AddState(const String& name);

		// Adds new state with name and animation
		Ptr<UIWidgetState> AddState(const String& name, const Animation& animation);

		// Adds state
		Ptr<UIWidgetState> AddState(Ptr<UIWidgetState> state);

		// Removes state by name
		bool RemoveState(const String& name);

		// Removes state
		bool RemoveState(Ptr<UIWidgetState> state);

		// Removes all states
		void RemoveAllStates();

		// Sets state value
		void SetState(const String& name, bool state);

		// Returns state value
		bool GetState(const String& name) const;

		// Returns state object by name
		Ptr<UIWidgetState> GetStateObject(const String& name) const;

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

		SERIALIZABLE_IMPL(UIWidget);

		IOBJECT(UIWidget)
		{
			FIELD(name);
			FIELD(parent);
			FIELD(layout);
			FIELD(childs);
			FIELD(layers);
			FIELD(states);
			FIELD(child);
			FIELD(layer);
			FIELD(state);
			FIELD(transparency);
			FIELD(visible);

			SRLZ_FIELD(mName);
			SRLZ_FIELD(layout);
			SRLZ_FIELD(mLayers);
			SRLZ_FIELD(mStates);
			SRLZ_FIELD(mChilds);
			SRLZ_FIELD(mTransparency);

			FIELD(mDrawingLayers);
			//FIELD(mParent);
		}

	protected:
		String             mName;             // Name

		LayersVec          mLayers;           // Layers array
		StatesVec          mStates;           // States array

		Ptr<UIWidget>      mParent;           // Parent widget
		WidgetsVec         mChilds;           // Children widgets
		RectF              mChildsAbsRect;    // Absolute rectangle for children arranging

		float              mTransparency;	  // Widget transparency
		float              mResTransparency;  // Widget result transparency, depends on parent's result transparency
		LayersVec          mDrawingLayers;    // Layers ordered by depth, which drawing before children (depth < 1000)
		LayersVec          mTopDrawingLayers; // Layers ordered by depth, which drawing after children (depth > 1000)

		Ptr<UIWidgetState> mSelectedState;    // Selected widget state
		bool               mIsSelected;       // Is widget selected

		Ptr<UIWidgetState> mVisibleState;     // Widget visibility state
		bool               mVisible;          // Visibility of widget. Uses state 'visible'
		bool               mResVisible;       // Result visibility of widget. Depends on this visibility and parent result visibility
		bool               mFullyDisabled;    // True, if widget is not visible and visible state is fully false

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
		virtual void OnChildSelected(Ptr<UIWidget> child);

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
		Dictionary<String, Ptr<UIWidgetLayer>> GetAllLayers();

		// Returns dictionary of all children by names
		Dictionary<String, Ptr<UIWidget>> GetAllChilds();

		// Calls when layer added and updates drawing sequence
		virtual void OnLayerAdded(Ptr<UIWidgetLayer> layer);

		// Calls when child widget was added
		virtual void OnChildAdded(Ptr<UIWidget> child);

		// Calls when child widget was removed
		virtual void OnChildRemoved(Ptr<UIWidget> child);

		// Calls when deserialized
		void OnDeserialized(const DataNode& node);

		// Forcible drawing in area
		void ForceDraw(const RectF& area, float transparency);

		// Calls when visible was changed
		virtual void OnVisibleChanged();

		// Initializes properties
		void InitializeProperties();

		friend class UICustomDropDown;
		friend class UICustomList;
		friend class UIDropDown;
		friend class UIEditBox;
		friend class UIHorizontalLayout;
		friend class UIHorizontalProgress;
		friend class UIHorizontalScrollBar;
		friend class UIList;
		friend class UIManager;
		friend class UIScrollArea;
		friend class UIVerticalLayout;
		friend class UIVerticalProgress;
		friend class UIVerticalScrollBar;
		friend class UIWidgetLayer;
		friend class UIWidgetLayout;
		friend class UIWindow;
	};

	template<typename _type>
	Ptr<_type> UIWidget::FindChild()
	{
		for (auto child : mChilds)
			if (child->GetTypeId() == _type::type->ID())
				return child;

		return nullptr;
	}

	template<typename _type>
	Ptr<_type> UIWidget::GetLayerDrawable(const String& path) const
	{
		auto layer = GetLayer(path);
		if (layer && layer->drawable->GetTypeId() == _type::type->ID())
			return layer->drawable.Cast<_type>();

		return nullptr;
	}
}