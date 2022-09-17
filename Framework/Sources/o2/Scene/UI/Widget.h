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

		PROPERTY(bool, enabledForcibly, SetEnabledForcible, IsEnabled); // Enable property, works forcibly @EDITOR_IGNORE @ANIMATABLE @SCRIPTABLE

		PROPERTY(float, transparency, SetTransparency, GetTransparency); // Transparency property @SCRIPTABLE
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
		Function<void()> onLayoutUpdated; // Layout change event @EDITOR_IGNORE
		Function<void()> onFocused;       // Widget focused event @EDITOR_IGNORE
		Function<void()> onUnfocused;     // Widget unfocused event @EDITOR_IGNORE
		Function<void()> onShow;          // Widget showing vent @EDITOR_IGNORE
		Function<void()> onHide;          // Widget hiding event @EDITOR_IGNORE

	public:
		// Default constructor @SCRIPTABLE
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

		// Forcible drawing in area with transparency @SCRIPTABLE
		void ForceDraw(const RectF& area, float transparency);

		// Sets layout dirty, and update it in update loop @SCRIPTABLE
		void SetLayoutDirty();

		// Returns parent widget @SCRIPTABLE
		Widget* GetParentWidget() const;

		// Returns children rectangle layout @SCRIPTABLE
		const RectF& GetChildrenWorldRect() const;

		// Returns child widget by path (like "root/some node/other node/target node") @SCRIPTABLE
		Widget* GetChildWidget(const String& path) const;

		// Add child actor @SCRIPTABLE
		Actor* AddChild(Actor* actor) override;

		// Add child actor @SCRIPTABLE
		Actor* AddChild(Actor* actor, int index) override;

		// Adds child widget and returns them @SCRIPTABLE
		Widget* AddChildWidget(Widget* widget);

		// Adds child widget at position and returns them
		Widget* AddChildWidget(Widget* widget, int position);

		// Returns constant children widgets vector
		const Vector<Widget*>& GetChildWidgets() const;

		// Sets index position in parent or scene @SCRIPTABLE
		void SetIndexInSiblings(int index) override;

		// Adds layer @SCRIPTABLE
		WidgetLayer* AddLayer(WidgetLayer* layer);

		// Adds layer @SCRIPTABLE_NAME(AddLayerNew)
		WidgetLayer* AddLayer(const String& name, IRectDrawable* drawable,
							  const Layout& layout = Layout::BothStretch(), float depth = 0.0f);

		// Removes layer @SCRIPTABLE
		void RemoveLayer(WidgetLayer* layer, bool release = true);

		// Removes layer @SCRIPTABLE_NAME(RemoveLayerByPath)
		void RemoveLayer(const String& path);

		// Removes all layers @SCRIPTABLE
		void RemoveAllLayers();

		// Returns layer by path. Returns null if layer isn't exist @SCRIPTABLE
		WidgetLayer* GetLayer(const String& path) const;

		// Returns layer by name in layers hierarchy. Returns null if layer isn't exist @SCRIPTABLE
		WidgetLayer* FindLayer(const String& name) const;

		// Searches layer with drawable with specified type
		template<typename _type>
		_type* GetLayerDrawableByType();

		// Returns layer by path. Returns null if layer isn't exist or layer drawable has different type
		template<typename _type>
		_type* GetLayerDrawable(const String& path) const;

		// Returns all layers @SCRIPTABLE
		const Vector<WidgetLayer*>& GetLayers() const;

		// Adds new state with name @SCRIPTABLE_NAME(AddStateNew)
		WidgetState* AddState(const String& name);

		// Adds new state with name and animation @SCRIPTABLE_NAME(AddStateWithClip)
		WidgetState* AddState(const String& name, const AnimationClip& animation);

		// Adds new state with name and animation @SCRIPTABLE_NAME(AddStateWithClipAsset)
		WidgetState* AddState(const String& name, const AnimationAssetRef& animation);

		// Adds state @SCRIPTABLE
		WidgetState* AddState(WidgetState* state, bool showAnimErrors = true);

		// Removes state by name @SCRIPTABLE_NAME(RemoveStateByName)
		bool RemoveState(const String& name);

		// Removes state @SCRIPTABLE
		bool RemoveState(WidgetState* state);

		// Removes all states @SCRIPTABLE
		void RemoveAllStates();

		// Sets state value @SCRIPTABLE
		void SetState(const String& name, bool state);

		// Sets state value immediately @SCRIPTABLE
		void SetStateForcible(const String& name, bool state);

		// Returns state value @SCRIPTABLE
		bool GetState(const String& name) const;

		// Returns state object by name @SCRIPTABLE
		WidgetState* GetStateObject(const String& name) const;

		// Returns all states @SCRIPTABLE
		const Vector<WidgetState*>& GetStates() const;

		// Sets depth overriding @SCRIPTABLE
		void SetDepthOverridden(bool overrideDepth);

		// Is sorting depth overridden @SCRIPTABLE
		bool IsDepthOverriden() const;

		// Sets widget's transparency
		void SetTransparency(float transparency);

		// Returns widget's transparency
		float GetTransparency() const;

		// Returns widget's result transparency (depends on parent's result transparency) @SCRIPTABLE
		float GetResTransparency() const;

		// Sets visibility @SCRIPTABLE
		void SetEnabledForcible(bool visible);

		// Sets visibility to true @SCRIPTABLE
		void Show(bool forcible = false);

		// Sets visibility to false @SCRIPTABLE
		void Hide(bool forcible = false);

		// Focus this widget @SCRIPTABLE
		void Focus();

		// Unfocus this widget @SCRIPTABLE
		void Unfocus();

		// Returns is this widget focused @SCRIPTABLE
		bool IsFocused() const;

		// Returns is this widget can be focused @SCRIPTABLE
		virtual bool IsFocusable() const;

		// Sets widget can be focused @SCRIPTABLE
		void SetFocusable(bool focusable);

		// Returns true if point is under drawable @SCRIPTABLE
		bool IsUnderPoint(const Vec2F& point) override;

		// Sets parent,  doesn't adds to parent's children but adds to internal children @SCRIPTABLE
		void SetInternalParent(Widget* parent, bool worldPositionStays = false);

		// Adds widget to internal children @SCRIPTABLE
		void AddInternalWidget(Widget* widget, bool worldPositionStays = false);

		// Returns internal child widget by path (like "root/some node/other node/target node") @SCRIPTABLE
		Widget* GetInternalWidget(const String& path) const;

		// Returns internal child widget by path (like "root/some node/other node/target node")
		template<typename _type>
		_type* GetInternalWidgetByType(const String& path) const;

		// Searches widget with name in internal widgets hierarchy @SCRIPTABLE
		Widget* FindInternalWidget(const String& name) const;

		// Searches widget with type and name in internal widgets hierarchy
		template<typename _type>
		_type* FindInternalWidgetByType(const String& name) const;

		// Searches widget with type in internal widgets hierarchy
		template<typename _type>
		_type* FindInternalWidgetByType() const;

		// Searches actor with id in this and this children @SCRIPTABLE
		Actor* FindActorById(SceneUID id) override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(Widget);

	protected:
		using Actor::mLayer;
		using Actor::mState;

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
	FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().SCRIPTABLE_ATTRIBUTE().NAME(enabledForcibly);
	FIELD().PUBLIC().SCRIPTABLE_ATTRIBUTE().NAME(transparency);
	FIELD().PUBLIC().ANIMATABLE_ATTRIBUTE().EDITOR_IGNORE_ATTRIBUTE().NAME(resTransparency);
	FIELD().PUBLIC().NAME(childrenWidgets);
	FIELD().PUBLIC().NAME(layers);
	FIELD().PUBLIC().NAME(states);
	FIELD().PUBLIC().NAME(childWidget);
	FIELD().PUBLIC().NAME(internalWidget);
	FIELD().PUBLIC().NAME(layer);
	FIELD().PUBLIC().NAME(state);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(layout);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onLayoutUpdated);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onFocused);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onUnfocused);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onShow);
	FIELD().PUBLIC().EDITOR_IGNORE_ATTRIBUTE().NAME(onHide);
	FIELD().PROTECTED().DEFAULT_TYPE_ATTRIBUTE(o2::WidgetLayer).DONT_DELETE_ATTRIBUTE().NAME(mLayers);
	FIELD().PROTECTED().DEFAULT_TYPE_ATTRIBUTE(o2::WidgetState).DONT_DELETE_ATTRIBUTE().EDITOR_PROPERTY_ATTRIBUTE().INVOKE_ON_CHANGE_ATTRIBUTE(OnStatesListChanged).NAME(mStates);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mParentWidget);
	FIELD().PROTECTED().DEFAULT_TYPE_ATTRIBUTE(o2::Widget).DONT_DELETE_ATTRIBUTE().NAME(mChildWidgets);
	FIELD().PROTECTED().DEFAULT_TYPE_ATTRIBUTE(o2::Widget).DONT_DELETE_ATTRIBUTE().NAME(mInternalWidgets);
	FIELD().PROTECTED().DEFAULT_TYPE_ATTRIBUTE(o2::Widget).DONT_DELETE_ATTRIBUTE().NAME(mDrawingChildren);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mOverrideDepth);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mTransparency);
	FIELD().PROTECTED().DEFAULT_VALUE(1.0f).NAME(mResTransparency);
	FIELD().PROTECTED().NAME(mDrawingLayers);
	FIELD().PROTECTED().DEFAULT_TYPE_ATTRIBUTE(o2::WidgetState).DONT_DELETE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mFocusedState);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsFocused);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(false).NAME(mIsFocusable);
	FIELD().PROTECTED().DEFAULT_TYPE_ATTRIBUTE(o2::WidgetState).DONT_DELETE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mVisibleState);
	FIELD().PROTECTED().DEFAULT_VALUE(false).NAME(mIsClipped);
	FIELD().PROTECTED().NAME(mBounds);
	FIELD().PROTECTED().NAME(mBoundsWithChilds);
	FIELD().PUBLIC().NAME(layersEditable);
	FIELD().PUBLIC().NAME(internalChildrenEditable);
}
END_META;
CLASS_METHODS_META(o2::Widget)
{

	typedef Map<String, WidgetLayer*> _tmp1;
	typedef Map<String, Widget*> _tmp2;
	typedef Map<String, Widget*> _tmp3;
	typedef Map<String, WidgetState*> _tmp4;

	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(ActorCreateMode);
	FUNCTION().PUBLIC().CONSTRUCTOR(const ActorAssetRef&, ActorCreateMode);
	FUNCTION().PUBLIC().CONSTRUCTOR(Vector<Component*>, ActorCreateMode);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Widget&);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateChildren, float);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateTransform);
	FUNCTION().PUBLIC().SIGNATURE(void, UpdateChildrenTransforms);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, ForceDraw, const RectF&, float);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetLayoutDirty);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Widget*, GetParentWidget);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const RectF&, GetChildrenWorldRect);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Widget*, GetChildWidget, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, AddChild, Actor*);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, AddChild, Actor*, int);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Widget*, AddChildWidget, Widget*);
	FUNCTION().PUBLIC().SIGNATURE(Widget*, AddChildWidget, Widget*, int);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<Widget*>&, GetChildWidgets);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetIndexInSiblings, int);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(WidgetLayer*, AddLayer, WidgetLayer*);
	FUNCTION().PUBLIC().SIGNATURE(WidgetLayer*, AddLayer, const String&, IRectDrawable*, const Layout&, float);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveLayer, WidgetLayer*, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_NAME_ATTRIBUTE(RemoveLayerByPath).SIGNATURE(void, RemoveLayer, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveAllLayers);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(WidgetLayer*, GetLayer, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(WidgetLayer*, FindLayer, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const Vector<WidgetLayer*>&, GetLayers);
	FUNCTION().PUBLIC().SCRIPTABLE_NAME_ATTRIBUTE(AddStateNew).SIGNATURE(WidgetState*, AddState, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_NAME_ATTRIBUTE(AddStateWithClip).SIGNATURE(WidgetState*, AddState, const String&, const AnimationClip&);
	FUNCTION().PUBLIC().SCRIPTABLE_NAME_ATTRIBUTE(AddStateWithClipAsset).SIGNATURE(WidgetState*, AddState, const String&, const AnimationAssetRef&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(WidgetState*, AddState, WidgetState*, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_NAME_ATTRIBUTE(RemoveStateByName).SIGNATURE(bool, RemoveState, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, RemoveState, WidgetState*);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, RemoveAllStates);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetState, const String&, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetStateForcible, const String&, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, GetState, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(WidgetState*, GetStateObject, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(const Vector<WidgetState*>&, GetStates);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetDepthOverridden, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsDepthOverriden);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTransparency, float);
	FUNCTION().PUBLIC().SIGNATURE(float, GetTransparency);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(float, GetResTransparency);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetEnabledForcible, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Show, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Hide, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Focus);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, Unfocus);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsFocused);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsFocusable);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetFocusable, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, SetInternalParent, Widget*, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(void, AddInternalWidget, Widget*, bool);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Widget*, GetInternalWidget, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Widget*, FindInternalWidget, const String&);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(Actor*, FindActorById, SceneUID);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
	FUNCTION().PROTECTED().SIGNATURE(void, SerializeRaw, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, DeserializeRaw, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, SerializeWithProto, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, DeserializeWithProto, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserializedDelta, const DataValue&, const IObject&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateResEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateResEnabledInHierarchy);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
	FUNCTION().PROTECTED().SIGNATURE(void, OnParentChanged, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildAdded, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildRemoved, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(SceneLayer*, GetSceneDrawableSceneLayer);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsSceneDrawableEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateChildWidgetsList);
	FUNCTION().PROTECTED().SIGNATURE(WidgetLayoutData&, GetLayoutData);
	FUNCTION().PROTECTED().SIGNATURE(const WidgetLayoutData&, GetLayoutData);
	FUNCTION().PROTECTED().SIGNATURE(void, SetChildrenWorldRect, const RectF&);
	FUNCTION().PROTECTED().SIGNATURE(void, MoveAndCheckClipping, const Vec2F&, const RectF&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildAdded, Widget*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildRemoved, Widget*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnFocused);
	FUNCTION().PROTECTED().SIGNATURE(void, OnUnfocused);
	FUNCTION().PROTECTED().SIGNATURE(float, GetMinWidthWithChildren);
	FUNCTION().PROTECTED().SIGNATURE(float, GetMinHeightWithChildren);
	FUNCTION().PROTECTED().SIGNATURE(float, GetWidthWeightWithChildren);
	FUNCTION().PROTECTED().SIGNATURE(float, GetHeightWeightWithChildren);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateBounds);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateBoundsWithChilds);
	FUNCTION().PROTECTED().SIGNATURE(void, CheckClipping, const RectF&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateTransparency);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateVisibility, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLayersLayouts);
	FUNCTION().PROTECTED().SIGNATURE(void, OnChildFocused, Widget*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnLayerAdded, WidgetLayer*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnStateAdded, WidgetState*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnStatesListChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, DrawDebugFrame);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateDrawingChildren);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLayersDrawingSequence);
	FUNCTION().PROTECTED().SIGNATURE(void, RetargetStatesAnimations);
	FUNCTION().PROTECTED().SIGNATURE(void, SetParentWidget, Widget*);
	FUNCTION().PROTECTED().SIGNATURE(Vector<Widget*>, GetChildrenNonConst);
	FUNCTION().PROTECTED().SIGNATURE(Vector<WidgetLayer*>, GetLayersNonConst);
	FUNCTION().PROTECTED().SIGNATURE(Vector<WidgetState*>, GetStatesNonConst);
	FUNCTION().PROTECTED().SIGNATURE(_tmp1, GetAllLayers);
	FUNCTION().PROTECTED().SIGNATURE(_tmp2, GetAllChilds);
	FUNCTION().PROTECTED().SIGNATURE(_tmp3, GetAllInternalWidgets);
	FUNCTION().PROTECTED().SIGNATURE(_tmp4, GetAllStates);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEditableParent, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableParent);
	FUNCTION().PUBLIC().SIGNATURE(Vector<SceneEditableObject*>, GetEditableChildren);
	FUNCTION().PUBLIC().SIGNATURE(void, AddEditableChild, SceneEditableObject*, int);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsTransforming);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetTransform);
	FUNCTION().PUBLIC().SIGNATURE(void, SetTransform, const Basis&);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsLayout);
	FUNCTION().PUBLIC().SIGNATURE(Layout, GetLayout);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLayout, const Layout&);
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableOwner);
}
END_META;

CLASS_BASES_META(o2::Widget::LayersEditable)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::Widget::LayersEditable)
{
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(widget);
	FIELD().PUBLIC().DEFAULT_VALUE(Math::Random()).NAME(UID);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(prototypeLink);
}
END_META;
CLASS_METHODS_META(o2::Widget::LayersEditable)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(Widget*);
	FUNCTION().PUBLIC().SIGNATURE(SceneUID, GetID);
	FUNCTION().PUBLIC().SIGNATURE(void, GenerateNewID, bool);
	FUNCTION().PUBLIC().SIGNATURE(const String&, GetName);
	FUNCTION().PUBLIC().SIGNATURE(void, SetName, const String&);
	FUNCTION().PUBLIC().SIGNATURE(const SceneEditableObject*, GetEditableLink);
	FUNCTION().PUBLIC().SIGNATURE(Vector<SceneEditableObject*>, GetEditableChildren);
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableParent);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEditableParent, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, AddEditableChild, SceneEditableObject*, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetIndexInSiblings, int);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsDeleting);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetTransform);
}
END_META;

CLASS_BASES_META(o2::Widget::InternalChildrenEditableEditable)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::Widget::InternalChildrenEditableEditable)
{
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(widget);
	FIELD().PUBLIC().DEFAULT_VALUE(Math::Random()).NAME(UID);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(prototypeLink);
}
END_META;
CLASS_METHODS_META(o2::Widget::InternalChildrenEditableEditable)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(Widget*);
	FUNCTION().PUBLIC().SIGNATURE(SceneUID, GetID);
	FUNCTION().PUBLIC().SIGNATURE(void, GenerateNewID, bool);
	FUNCTION().PUBLIC().SIGNATURE(const String&, GetName);
	FUNCTION().PUBLIC().SIGNATURE(void, SetName, const String&);
	FUNCTION().PUBLIC().SIGNATURE(const SceneEditableObject*, GetEditableLink);
	FUNCTION().PUBLIC().SIGNATURE(Vector<SceneEditableObject*>, GetEditableChildren);
	FUNCTION().PUBLIC().SIGNATURE(SceneEditableObject*, GetEditableParent);
	FUNCTION().PUBLIC().SIGNATURE(void, SetEditableParent, SceneEditableObject*);
	FUNCTION().PUBLIC().SIGNATURE(void, AddEditableChild, SceneEditableObject*, int);
	FUNCTION().PUBLIC().SIGNATURE(void, SetIndexInSiblings, int);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsSupportsDeleting);
	FUNCTION().PUBLIC().SIGNATURE(Basis, GetTransform);
}
END_META;
