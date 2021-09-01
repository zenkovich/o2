#pragma once

#include "o2/Assets/Types/AnimationAsset.h"
#include "o2/Scene/Actor.h"
#include "o2/Scene/ISceneDrawable.h"
#include "o2/Scene/UI/WidgetState.h"
#include "o2/Utils/Editor/Attributes/AnimatableAttribute.h"
#include "o2/Utils/Editor/Attributes/DefaultTypeAttribute.h"
#include "o2/Utils/Editor/Attributes/DontDeleteAttribute.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "o2/Utils/Editor/Attributes/InvokeOnChangeAttribute.h"
#include "o2/Utils/Math/Layout.h"

namespace o2
{
	class IRectDrawable;
	class WidgetLayer;
	class WidgetLayout;
	class WidgetLayoutData;

	// ------------------------------------------------------
	// Basic UI Widget. Its a simple and basic element of UI, 
	// everything other UI's are based on this
	// ------------------------------------------------------
	class Widget: public Actor, public ISceneDrawable
	{
	public:
		PROPERTIES(Widget);

		PROPERTY(bool, enabledForcibly, SetEnabledForcible, IsEnabled); // Enable property, works forcibly @EDITOR_IGNORE @ANIMATABLE

		PROPERTY(float, transparency, SetTransparency, GetTransparency); // Transparency property
		GETTER(float, resTransparency, GetResTransparency);              // Result transparency getter, depends on parent transparency @EDITOR_IGNORE @ANIMATABLE

		GETTER(Vector<Widget*>, childrenWidgets, GetChildrenNonConst); // Widget children getter

		GETTER(Vector<WidgetLayer*>, layers, GetLayers); // Layers getter
		GETTER(Vector<WidgetState*>, states, GetStates); // States getter

		ACCESSOR(Widget*, childWidget, String, GetChildWidget, GetAllChilds);                // Widget child accessor by path like "child/subchild/somechild"
		ACCESSOR(Widget*, internalWidget, String, GetInternalWidget, GetAllInternalWidgets); // Widget internals accessor by path like "child/subchild/somechild"
		ACCESSOR(WidgetLayer*, layer, String, GetLayer, GetAllLayers);                       // Widget layer accessor by path like "layer/sublayer/target"
		ACCESSOR(WidgetState*, state, String, GetStateObject, GetAllStates);                 // Widget state accessor by name

	public:
		WidgetLayout* const layout; // Widget layout @EDITOR_IGNORE

	public:
		Function<void()> onLayoutUpdated; // Layout change event
		Function<void()> onFocused;       // Widget focused event
		Function<void()> onUnfocused;     // Widget unfocused event
		Function<void()> onShow;          // Widget showing vent
		Function<void()> onHide;          // Widget hiding event

	public:
		// Default constructor
		Widget(ActorCreateMode mode = ActorCreateMode::Default);

		// Widget constructor from prototype
		Widget(const ActorAssetRef& prototype, ActorCreateMode mode = ActorCreateMode::Default);

		// Widget constructor with components
		Widget(Vector<Component*> components, ActorCreateMode mode = ActorCreateMode::Default);

		// Copy-constructor
		Widget(const Widget& other);

		// Virtual destructor
		virtual ~Widget();

		// Copy-operator
		Widget& operator=(const Widget& other);

		// Updates layers, states and widget
		void Update(float dt) override;

		// Updates childs
		void UpdateChildren(float dt) override;

		// Updates self transform, dependent parents and children transforms
		void UpdateTransform() override;

		// Updates children and internal children transforms
		void UpdateChildrenTransforms() override;

		// Draws widget and child widgets with not overridden depth
		void Draw() override;

		// Forcible drawing in area with transparency
		void ForceDraw(const RectF& area, float transparency);

		// Sets layout dirty, and update it in update loop
		void SetLayoutDirty();

		// Returns parent widget
		Widget* GetParentWidget() const;

		// Returns children rectangle layout
		const RectF& GetChildrenWorldRect() const;

		// Returns child widget by path (like "root/some node/other node/target node")
		Widget* GetChildWidget(const String& path) const;

		// Adds child widget and returns them
		Widget* AddChildWidget(Widget* widget);

		// Adds child widget at position and returns them
		Widget* AddChildWidget(Widget* widget, int position);

		// Returns constant children widgets vector
		const Vector<Widget*>& GetChildWidgets() const;

		// Sets index position in parent or scene
		void SetIndexInSiblings(int index) override;

		// Adds layer
		WidgetLayer* AddLayer(WidgetLayer* layer);

		// Adds layer
		WidgetLayer* AddLayer(const String& name, IRectDrawable* drawable,
							  const Layout& layout = Layout::BothStretch(), float depth = 0.0f);

		// Removes layer
		void RemoveLayer(WidgetLayer* layer, bool release = true);

		// Removes layer
		void RemoveLayer(const String& path);

		// Removes all layers
		void RemoveAllLayers();

		// Returns layer by path. Returns null if layer isn't exist
		WidgetLayer* GetLayer(const String& path) const;

		// Returns layer by name in layers hierarchy. Returns null if layer isn't exist
		WidgetLayer* FindLayer(const String& name) const;

		// Searches layer with drawable with specified type
		template<typename _type>
		_type* GetLayerDrawableByType();

		// Returns layer by path. Returns null if layer isn't exist or layer drawable has different type
		template<typename _type>
		_type* GetLayerDrawable(const String& path) const;

		// Returns all layers
		const Vector<WidgetLayer*>& GetLayers() const;

		// Adds new state with name
		WidgetState* AddState(const String& name);

		// Adds new state with name and animation
		WidgetState* AddState(const String& name, const AnimationClip& animation);

		// Adds new state with name and animation
		WidgetState* AddState(const String& name, const AnimationAssetRef& animation);

		// Adds state
		WidgetState* AddState(WidgetState* state, bool showAnimErrors = true);

		// Removes state by name
		bool RemoveState(const String& name);

		// Removes state
		bool RemoveState(WidgetState* state);

		// Removes all states
		void RemoveAllStates();

		// Sets state value
		void SetState(const String& name, bool state);

		// Sets state value immediately
		void SetStateForcible(const String& name, bool state);

		// Returns state value
		bool GetState(const String& name) const;

		// Returns state object by name
		WidgetState* GetStateObject(const String& name) const;

		// Returns all states
		const Vector<WidgetState*>& GetStates() const;

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
		void SetEnabledForcible(bool visible);

		// Sets visibility to true
		void Show(bool forcible = false);

		// Sets visibility to false
		void Hide(bool forcible = false);

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

		// Sets parent,  doesn't adds to parent's children but adds to internal children
		void SetInternalParent(Widget* parent, bool worldPositionStays = false);

		// Adds widget to internal children
		void AddInternalWidget(Widget* widget, bool worldPositionStays = false);

		// Returns internal child widget by path (like "root/some node/other node/target node")
		Widget* GetInternalWidget(const String& path) const;

		// Returns internal child widget by path (like "root/some node/other node/target node")
		template<typename _type>
		_type* GetInternalWidgetByType(const String& path) const;

		// Searches widget with name in internal widgets hierarchy
		Widget* FindInternalWidget(const String& name) const;

		// Searches widget with type and name in internal widgets hierarchy
		template<typename _type>
		_type* FindInternalWidgetByType(const String& name) const;

		// Searches widget with type in internal widgets hierarchy
		template<typename _type>
		_type* FindInternalWidgetByType() const;

		// Searches actor with id in this and this children
		Actor* FindActorById(SceneUID id) override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(Widget);

	protected:
		using Actor::mLayer;
		using Actor::mSceneStatus;

		Vector<WidgetLayer*> mLayers; // Layers array @DONT_DELETE @DEFAULT_TYPE(o2::WidgetLayer)
		Vector<WidgetState*> mStates; // States array @DONT_DELETE @DEFAULT_TYPE(o2::WidgetState) @EDITOR_PROPERTY @INVOKE_ON_CHANGE(OnStatesListChanged)

		Widget*         mParentWidget = nullptr; // Parent widget. When parent is not widget, this field will be null 
		Vector<Widget*> mChildWidgets;           // Children widgets, a part of all children @DONT_DELETE @DEFAULT_TYPE(o2::Widget)
		Vector<Widget*> mInternalWidgets;        // Internal widgets, used same as children widgets, but not really children @DONT_DELETE @DEFAULT_TYPE(o2::Widget)
		Vector<Widget*> mDrawingChildren;        // Children widgets, which drawing depth isn't overridden @DONT_DELETE @DEFAULT_TYPE(o2::Widget)

		bool mOverrideDepth = false; // Is sorting order depth overridden. If not, sorting order depends on hierarchy @SERIALIZABLE

		float mTransparency = 1.0f;	   // Widget transparency @SERIALIZABLE
		float mResTransparency = 1.0f; // Widget result transparency, depends on parent's result transparency

		Vector<WidgetLayer*> mDrawingLayers;    // Layers ordered by depth, which drawing before children (depth < 1000) @DONT_DELETE @DEFAULT_TYPE(o2::WidgetLayer)
		Vector<WidgetLayer*> mTopDrawingLayers; // Layers ordered by depth, which drawing after children (depth > 1000) @DONT_DELETE @DEFAULT_TYPE(o2::WidgetLayer)

		WidgetState* mFocusedState = nullptr; // Focused widget state @DONT_DELETE @DEFAULT_TYPE(o2::WidgetState)
		bool         mIsFocused = false;      // Is widget focused
		bool         mIsFocusable = false;    // Is widget can be focused @SERIALIZABLE

		WidgetState* mVisibleState = nullptr; // Widget visibility state @DONT_DELETE @DEFAULT_TYPE(o2::WidgetState)

		bool mIsClipped = false; // Is widget fully clipped by some scissors

		RectF mBounds;           // Widget bounds by drawing layers
		RectF mBoundsWithChilds; // Widget with childs bounds

	protected:
		// Regular serializing without prototype
		void SerializeRaw(DataValue& node) const override;

		// Regular deserializing without prototype
		void DeserializeRaw(const DataValue& node) override;

		// Regular serializing with prototype
		void SerializeWithProto(DataValue& node) const override;

		// Regular deserializing with prototype
		void DeserializeWithProto(const DataValue& node) override;

		// Completion deserialization callback; initializes layers and children
		void OnDeserialized(const DataValue& node) override;

		// Completion deserialization delta callback; initializes layers and children
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		// Updates result read enable flag
		void UpdateResEnabled() override;

		// Updates enabling
		void UpdateResEnabledInHierarchy() override;

		// It is called when transformation was changed and updated
		void OnTransformUpdated() override;

		// It is called when parent changed
		void OnParentChanged(Actor* oldParent) override;

		// It is called when child actor was added
		void OnChildAdded(Actor* child) override;

		// It is called when child actor was removed
		void OnChildRemoved(Actor* child) override;

		// It is called when actor excluding from scene, removes this from layer drawables
		void OnRemoveFromScene() override;

		// It is called when actor including from scene, including this to layer drawables
		void OnAddToScene() override;

		// Returns current scene layer
		SceneLayer* GetSceneDrawableSceneLayer() const override;

		// Returns is drawable enabled
		bool IsSceneDrawableEnabled() const override;

		// Updates child widgets list
		void UpdateChildWidgetsList();

		// Returns layout data reference
		WidgetLayoutData& GetLayoutData();

		// Returns layout data reference
		const WidgetLayoutData& GetLayoutData() const;

		// Sets children world rectangle. Here you can override children position
		virtual void SetChildrenWorldRect(const RectF& childrenWorldRect);

		// Moves widget's to delta and checks for clipping
		virtual void MoveAndCheckClipping(const Vec2F& delta, const RectF& clipArea);

		// It is called when child widget was added
		virtual void OnChildAdded(Widget* child);

		// It is called when child widget was removed
		virtual void OnChildRemoved(Widget* child);

		// It is called when widget was selected
		virtual void OnFocused();

		// It is called when widget was deselected
		virtual void OnUnfocused();

		// Returns layout width with children
		virtual float GetMinWidthWithChildren() const;

		// Returns layout height with childer
		virtual float GetMinHeightWithChildren() const;

		// Returns layout width weight with children
		virtual float GetWidthWeightWithChildren() const;

		// Returns layout height weight with children
		virtual float GetHeightWeightWithChildren() const;

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

		// It is called when child widget was selected
		virtual void OnChildFocused(Widget* child);

		// It is called when layer added and updates drawing sequence
		virtual void OnLayerAdded(WidgetLayer* layer);

		// It is called when widget state was added
		virtual void OnStateAdded(WidgetState* state);

		// It is called from editor, refreshes states
		void OnStatesListChanged();

		// Draws debug frame by mAbsoluteRect
		void DrawDebugFrame();

		// Updates drawing children widgets list
		void UpdateDrawingChildren();

		// Updates layers drawing sequence
		void UpdateLayersDrawingSequence();

		// Sets new target for all states animations
		void RetargetStatesAnimations();

		// Sets parent widget, used for property
		void SetParentWidget(Widget* widget);

		// Returns children widgets (for property)
		Vector<Widget*> GetChildrenNonConst();

		// Returns layers (for property)
		Vector<WidgetLayer*> GetLayersNonConst();

		// Returns states (for property)
		Vector<WidgetState*> GetStatesNonConst();

		// Returns dictionary of all layers by names
		Map<String, WidgetLayer*> GetAllLayers();

		// Returns dictionary of all children by names
		Map<String, Widget*> GetAllChilds();

		// Returns dictionary of all internal widgets by names
		Map<String, Widget*> GetAllInternalWidgets();

		// Returns dictionary of all states by names
		Map<String, WidgetState*> GetAllStates();

		friend class ContextMenu;
		friend class CustomDropDown;
		friend class CustomList;
		friend class DropDown;
		friend class EditBox;
		friend class GridLayout;
		friend class GridLayoutScrollArea;
		friend class HorizontalLayout;
		friend class HorizontalProgress;
		friend class HorizontalScrollBar;
		friend class Label;
		friend class List;
		friend class LongList;
		friend class MenuPanel;
		friend class PopupWidget;
		friend class Scene;
		friend class ScrollArea;
		friend class Spoiler;
		friend class Tree;
		friend class TreeNode;
		friend class UIManager;
		friend class VerticalLayout;
		friend class VerticalProgress;
		friend class VerticalScrollBar;
		friend class WidgetLayer;
		friend class WidgetLayout;
		friend class Window;

#if IS_EDITOR
	public:
		class LayersEditable: public SceneEditableObject
		{
		public:
			Widget*  widget = nullptr;
			SceneUID UID = Math::Random();

			const LayersEditable* prototypeLink = nullptr;

		public:
			// Default constructor
			LayersEditable();

			// Constructor with widget
			LayersEditable(Widget* widget);

			// Returns unique id
			SceneUID GetID() const override;

			// Generates new random id 
			void GenerateNewID(bool childs = true) override;

			// Returns name of object
			const String& GetName() const override;

			// Sets name of object
			void SetName(const String& name) override;

			// Returns object's link to prototype
			const SceneEditableObject* GetEditableLink() const override;

			// Returns list of object's children
			Vector<SceneEditableObject*> GetEditableChildren() const override;

			// Returns object's parent object. Return nullptr when it is a root scene object
			SceneEditableObject* GetEditableParent() const override;

			// Sets parent object. nullptr means make this object as root. idx is place in parent children. idx == -1 means last
			void SetEditableParent(SceneEditableObject* object) override;

			// Adds child. idx is place in parent children. idx == -1 means last
			void AddEditableChild(SceneEditableObject* object, int idx = -1) override;

			// Sets index in siblings - children of parent
			void SetIndexInSiblings(int idx) override;

			// Returns is that type of object can be deleted from editor
			bool IsSupportsDeleting() const override;

			// Returns transform, override when it's supports
			Basis GetTransform() const override;

			SERIALIZABLE(LayersEditable);

			friend class Widget;
		};

		class InternalChildrenEditableEditable: public SceneEditableObject
		{
		public:
			Widget*  widget = nullptr;
			SceneUID UID = Math::Random();

			const InternalChildrenEditableEditable* prototypeLink = nullptr;

		public:
			// Default constructor
			InternalChildrenEditableEditable();

			// Constructor with widget
			InternalChildrenEditableEditable(Widget* widget);

			// Returns unique id
			SceneUID GetID() const override;

			// Generates new random id 
			void GenerateNewID(bool childs = true) override;

			// Returns name of object
			const String& GetName() const override;

			// Sets name of object
			void SetName(const String& name) override;

			// Returns object's link to prototype
			const SceneEditableObject* GetEditableLink() const override;

			// Returns list of object's children
			Vector<SceneEditableObject*> GetEditableChildren() const override;

			// Returns object's parent object. Return nullptr when it is a root scene object
			SceneEditableObject* GetEditableParent() const override;

			// Sets parent object. nullptr means make this object as root
			void SetEditableParent(SceneEditableObject* object) override;

			// Adds child. idx is place in parent children. idx == -1 means last
			void AddEditableChild(SceneEditableObject* object, int idx = -1) override;

			// Sets index in siblings - children of parent
			void SetIndexInSiblings(int idx) override;

			// Returns is that type of object can be deleted from editor
			bool IsSupportsDeleting() const override;

			// Returns transform, override when it's supports
			Basis GetTransform() const override;

			SERIALIZABLE(InternalChildrenEditableEditable);

			friend class Widget;
		};

	public:
		static bool isEditorLayersVisible;           // Is widgets layers visible in hierarchy
		static bool isEditorInternalChildrenVisible; // Is widgets internal children visible in hierarchy

		LayersEditable                   layersEditable = LayersEditable(this);
		InternalChildrenEditableEditable internalChildrenEditable = InternalChildrenEditableEditable(this);

	public:
		// Sets parent object. nullptr means make this object as root
		void SetEditableParent(SceneEditableObject* object) override;

		// Returns object's parent object. Return nullptr when it is a root scene object
		SceneEditableObject* GetEditableParent() const override;

		// Returns list of object's children
		Vector<SceneEditableObject*> GetEditableChildren() const override;

		// Adds child. idx is place in parent children. idx == -1 means last
		void AddEditableChild(SceneEditableObject* object, int idx = -1) override;

		// Returns is that type of object can be transformed
		bool IsSupportsTransforming() const override;

		// Returns transform, override when it's supports
		Basis GetTransform() const override;

		// Sets transform of object, override when it's supports
		void SetTransform(const Basis& transform) override;

		// Returns is that type of object can be transformed with layout
		bool IsSupportsLayout() const override;

		// Returns layout, override when it's supports
		Layout GetLayout() const override;

		// Sets layout of object, override when it's supports
		void SetLayout(const Layout& layout) override;

		// Returns pointer to owner editable object
		SceneEditableObject* GetEditableOwner() override;

		friend class LayersEditable;
		friend class InternalChildrenEditableEditable;
#endif // IS_EDITOR
	};
}

#include "o2/Scene/UI/WidgetLayer.h"

namespace o2
{
	template<typename _type>
	_type* Widget::GetLayerDrawableByType()
	{
		for (auto layer : mLayers)
		{
			if (layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(_type))
				return (_type*)layer->GetDrawable();
		}

		for (auto layer : mLayers)
		{
			auto res = layer->FindChild<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}

	template<typename _type>
	_type* Widget::GetLayerDrawable(const String& path) const
	{
		auto layer = GetLayer(path);
		if (layer && layer->GetDrawable() && layer->GetDrawable()->GetType() == TypeOf(_type))
			return (_type*)layer->GetDrawable();

		return nullptr;
	}

	template<typename _type>
	_type* Widget::GetInternalWidgetByType(const String& path) const
	{
		return dynamic_cast<_type*>(GetInternalWidget(path));
	}

	template<typename _type>
	_type* Widget::FindInternalWidgetByType(const String& name) const
	{
		for (auto widget : mInternalWidgets)
		{
			if (widget->GetName() == name)
			{
				if (_type* res = dynamic_cast<_type*>(widget))
					return res;
			}

			if (_type* res = widget->FindChildByTypeAndName<_type>((String)name))
				return res;

			if (auto internalWidget = widget->FindInternalWidget(name))
				if (_type* res = dynamic_cast<_type*>(internalWidget))
					return res;
		}

		return nullptr;
	}

	template<typename _type>
	_type* Widget::FindInternalWidgetByType() const
	{
		for (auto widget : mInternalWidgets)
		{
			if (_type* res = dynamic_cast<_type*>(widget))
				return res;

			if (_type* res = widget->FindChildByType<_type>())
				return res;

			if (_type* res = widget->FindInternalWidgetByType<_type>())
				return res;
		}

		return nullptr;
	}

}

CLASS_BASES_META(o2::Widget)
{
	BASE_CLASS(o2::Actor);
	BASE_CLASS(o2::ISceneDrawable);
}
END_META;
CLASS_FIELDS_META(o2::Widget)
{
	FIELD().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(enabledForcibly).PUBLIC();
	FIELD().NAME(transparency).PUBLIC();
	FIELD().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(resTransparency).PUBLIC();
	FIELD().NAME(childrenWidgets).PUBLIC();
	FIELD().NAME(layers).PUBLIC();
	FIELD().NAME(states).PUBLIC();
	FIELD().NAME(childWidget).PUBLIC();
	FIELD().NAME(internalWidget).PUBLIC();
	FIELD().NAME(layer).PUBLIC();
	FIELD().NAME(state).PUBLIC();
	FIELD().EDITOR_IGNORE_ATTRIBUTE().NAME(layout).PUBLIC();
	FIELD().NAME(onLayoutUpdated).PUBLIC();
	FIELD().NAME(onFocused).PUBLIC();
	FIELD().NAME(onUnfocused).PUBLIC();
	FIELD().NAME(onShow).PUBLIC();
	FIELD().NAME(onHide).PUBLIC();
	FIELD().DONT_DELETE_ATTRIBUTE().NAME(mLayers).PROTECTED();
	FIELD().DONT_DELETE_ATTRIBUTE().EDITOR_PROPERTY_ATTRIBUTE().NAME(mStates).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mParentWidget).PROTECTED();
	FIELD().DONT_DELETE_ATTRIBUTE().NAME(mChildWidgets).PROTECTED();
	FIELD().DONT_DELETE_ATTRIBUTE().NAME(mInternalWidgets).PROTECTED();
	FIELD().DONT_DELETE_ATTRIBUTE().NAME(mDrawingChildren).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mOverrideDepth).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mTransparency).PROTECTED();
	FIELD().DEFAULT_VALUE(1.0f).NAME(mResTransparency).PROTECTED();
	FIELD().NAME(mDrawingLayers).PROTECTED();
	FIELD().DONT_DELETE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mFocusedState).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mIsFocused).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mIsFocusable).PROTECTED();
	FIELD().DONT_DELETE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mVisibleState).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mIsClipped).PROTECTED();
	FIELD().NAME(mBounds).PROTECTED();
	FIELD().NAME(mBoundsWithChilds).PROTECTED();
	FIELD().NAME(layersEditable).PROTECTED();
	FIELD().NAME(internalChildrenEditable).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::Widget)
{

	typedef Map<String, WidgetLayer*> _tmp1;
	typedef Map<String, Widget*> _tmp2;
	typedef Map<String, Widget*> _tmp3;
	typedef Map<String, WidgetState*> _tmp4;

	PUBLIC_FUNCTION(void, Update, float);
	PUBLIC_FUNCTION(void, UpdateChildren, float);
	PUBLIC_FUNCTION(void, UpdateTransform);
	PUBLIC_FUNCTION(void, UpdateChildrenTransforms);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, ForceDraw, const RectF&, float);
	PUBLIC_FUNCTION(void, SetLayoutDirty);
	PUBLIC_FUNCTION(Widget*, GetParentWidget);
	PUBLIC_FUNCTION(const RectF&, GetChildrenWorldRect);
	PUBLIC_FUNCTION(Widget*, GetChildWidget, const String&);
	PUBLIC_FUNCTION(Widget*, AddChildWidget, Widget*);
	PUBLIC_FUNCTION(Widget*, AddChildWidget, Widget*, int);
	PUBLIC_FUNCTION(const Vector<Widget*>&, GetChildWidgets);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(WidgetLayer*, AddLayer, WidgetLayer*);
	PUBLIC_FUNCTION(WidgetLayer*, AddLayer, const String&, IRectDrawable*, const Layout&, float);
	PUBLIC_FUNCTION(void, RemoveLayer, WidgetLayer*, bool);
	PUBLIC_FUNCTION(void, RemoveLayer, const String&);
	PUBLIC_FUNCTION(void, RemoveAllLayers);
	PUBLIC_FUNCTION(WidgetLayer*, GetLayer, const String&);
	PUBLIC_FUNCTION(WidgetLayer*, FindLayer, const String&);
	PUBLIC_FUNCTION(const Vector<WidgetLayer*>&, GetLayers);
	PUBLIC_FUNCTION(WidgetState*, AddState, const String&);
	PUBLIC_FUNCTION(WidgetState*, AddState, const String&, const AnimationClip&);
	PUBLIC_FUNCTION(WidgetState*, AddState, const String&, const AnimationAssetRef&);
	PUBLIC_FUNCTION(WidgetState*, AddState, WidgetState*, bool);
	PUBLIC_FUNCTION(bool, RemoveState, const String&);
	PUBLIC_FUNCTION(bool, RemoveState, WidgetState*);
	PUBLIC_FUNCTION(void, RemoveAllStates);
	PUBLIC_FUNCTION(void, SetState, const String&, bool);
	PUBLIC_FUNCTION(void, SetStateForcible, const String&, bool);
	PUBLIC_FUNCTION(bool, GetState, const String&);
	PUBLIC_FUNCTION(WidgetState*, GetStateObject, const String&);
	PUBLIC_FUNCTION(const Vector<WidgetState*>&, GetStates);
	PUBLIC_FUNCTION(void, SetDepthOverridden, bool);
	PUBLIC_FUNCTION(bool, IsDepthOverriden);
	PUBLIC_FUNCTION(void, SetTransparency, float);
	PUBLIC_FUNCTION(float, GetTransparency);
	PUBLIC_FUNCTION(float, GetResTransparency);
	PUBLIC_FUNCTION(void, SetEnabledForcible, bool);
	PUBLIC_FUNCTION(void, Show, bool);
	PUBLIC_FUNCTION(void, Hide, bool);
	PUBLIC_FUNCTION(void, Focus);
	PUBLIC_FUNCTION(void, Unfocus);
	PUBLIC_FUNCTION(bool, IsFocused);
	PUBLIC_FUNCTION(bool, IsFocusable);
	PUBLIC_FUNCTION(void, SetFocusable, bool);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, SetInternalParent, Widget*, bool);
	PUBLIC_FUNCTION(void, AddInternalWidget, Widget*, bool);
	PUBLIC_FUNCTION(Widget*, GetInternalWidget, const String&);
	PUBLIC_FUNCTION(Widget*, FindInternalWidget, const String&);
	PUBLIC_FUNCTION(Actor*, FindActorById, SceneUID);
	PUBLIC_STATIC_FUNCTION(String, GetCreateMenuCategory);
	PROTECTED_FUNCTION(void, SerializeRaw, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeRaw, const DataValue&);
	PROTECTED_FUNCTION(void, SerializeWithProto, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeWithProto, const DataValue&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnDeserializedDelta, const DataValue&, const IObject&);
	PROTECTED_FUNCTION(void, UpdateResEnabled);
	PROTECTED_FUNCTION(void, UpdateResEnabledInHierarchy);
	PROTECTED_FUNCTION(void, OnTransformUpdated);
	PROTECTED_FUNCTION(void, OnParentChanged, Actor*);
	PROTECTED_FUNCTION(void, OnChildAdded, Actor*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Actor*);
	PROTECTED_FUNCTION(void, OnRemoveFromScene);
	PROTECTED_FUNCTION(void, OnAddToScene);
	PROTECTED_FUNCTION(SceneLayer*, GetSceneDrawableSceneLayer);
	PROTECTED_FUNCTION(bool, IsSceneDrawableEnabled);
	PROTECTED_FUNCTION(void, UpdateChildWidgetsList);
	PROTECTED_FUNCTION(WidgetLayoutData&, GetLayoutData);
	PROTECTED_FUNCTION(const WidgetLayoutData&, GetLayoutData);
	PROTECTED_FUNCTION(void, SetChildrenWorldRect, const RectF&);
	PROTECTED_FUNCTION(void, MoveAndCheckClipping, const Vec2F&, const RectF&);
	PROTECTED_FUNCTION(void, OnChildAdded, Widget*);
	PROTECTED_FUNCTION(void, OnChildRemoved, Widget*);
	PROTECTED_FUNCTION(void, OnFocused);
	PROTECTED_FUNCTION(void, OnUnfocused);
	PROTECTED_FUNCTION(float, GetMinWidthWithChildren);
	PROTECTED_FUNCTION(float, GetMinHeightWithChildren);
	PROTECTED_FUNCTION(float, GetWidthWeightWithChildren);
	PROTECTED_FUNCTION(float, GetHeightWeightWithChildren);
	PROTECTED_FUNCTION(void, UpdateBounds);
	PROTECTED_FUNCTION(void, UpdateBoundsWithChilds);
	PROTECTED_FUNCTION(void, CheckClipping, const RectF&);
	PROTECTED_FUNCTION(void, UpdateTransparency);
	PROTECTED_FUNCTION(void, UpdateVisibility, bool);
	PROTECTED_FUNCTION(void, UpdateLayersLayouts);
	PROTECTED_FUNCTION(void, OnChildFocused, Widget*);
	PROTECTED_FUNCTION(void, OnLayerAdded, WidgetLayer*);
	PROTECTED_FUNCTION(void, OnStateAdded, WidgetState*);
	PROTECTED_FUNCTION(void, OnStatesListChanged);
	PROTECTED_FUNCTION(void, DrawDebugFrame);
	PROTECTED_FUNCTION(void, UpdateDrawingChildren);
	PROTECTED_FUNCTION(void, UpdateLayersDrawingSequence);
	PROTECTED_FUNCTION(void, RetargetStatesAnimations);
	PROTECTED_FUNCTION(void, SetParentWidget, Widget*);
	PROTECTED_FUNCTION(Vector<Widget*>, GetChildrenNonConst);
	PROTECTED_FUNCTION(Vector<WidgetLayer*>, GetLayersNonConst);
	PROTECTED_FUNCTION(Vector<WidgetState*>, GetStatesNonConst);
	PROTECTED_FUNCTION(_tmp1, GetAllLayers);
	PROTECTED_FUNCTION(_tmp2, GetAllChilds);
	PROTECTED_FUNCTION(_tmp3, GetAllInternalWidgets);
	PROTECTED_FUNCTION(_tmp4, GetAllStates);
	PUBLIC_FUNCTION(void, SetEditableParent, SceneEditableObject*);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditableChildren);
	PUBLIC_FUNCTION(void, AddEditableChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(bool, IsSupportsTransforming);
	PUBLIC_FUNCTION(Basis, GetTransform);
	PUBLIC_FUNCTION(void, SetTransform, const Basis&);
	PUBLIC_FUNCTION(bool, IsSupportsLayout);
	PUBLIC_FUNCTION(Layout, GetLayout);
	PUBLIC_FUNCTION(void, SetLayout, const Layout&);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableOwner);
}
END_META;

CLASS_BASES_META(o2::Widget::LayersEditable)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::Widget::LayersEditable)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(widget).PUBLIC();
	FIELD().DEFAULT_VALUE(Math::Random()).NAME(UID).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(prototypeLink).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::Widget::LayersEditable)
{

	PUBLIC_FUNCTION(SceneUID, GetID);
	PUBLIC_FUNCTION(void, GenerateNewID, bool);
	PUBLIC_FUNCTION(const String&, GetName);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(const SceneEditableObject*, GetEditableLink);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditableChildren);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(void, SetEditableParent, SceneEditableObject*);
	PUBLIC_FUNCTION(void, AddEditableChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(bool, IsSupportsDeleting);
	PUBLIC_FUNCTION(Basis, GetTransform);
}
END_META;

CLASS_BASES_META(o2::Widget::InternalChildrenEditableEditable)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::Widget::InternalChildrenEditableEditable)
{
	FIELD().DEFAULT_VALUE(nullptr).NAME(widget).PUBLIC();
	FIELD().DEFAULT_VALUE(Math::Random()).NAME(UID).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(prototypeLink).PUBLIC();
}
END_META;
CLASS_METHODS_META(o2::Widget::InternalChildrenEditableEditable)
{

	PUBLIC_FUNCTION(SceneUID, GetID);
	PUBLIC_FUNCTION(void, GenerateNewID, bool);
	PUBLIC_FUNCTION(const String&, GetName);
	PUBLIC_FUNCTION(void, SetName, const String&);
	PUBLIC_FUNCTION(const SceneEditableObject*, GetEditableLink);
	PUBLIC_FUNCTION(Vector<SceneEditableObject*>, GetEditableChildren);
	PUBLIC_FUNCTION(SceneEditableObject*, GetEditableParent);
	PUBLIC_FUNCTION(void, SetEditableParent, SceneEditableObject*);
	PUBLIC_FUNCTION(void, AddEditableChild, SceneEditableObject*, int);
	PUBLIC_FUNCTION(void, SetIndexInSiblings, int);
	PUBLIC_FUNCTION(bool, IsSupportsDeleting);
	PUBLIC_FUNCTION(Basis, GetTransform);
}
END_META;
