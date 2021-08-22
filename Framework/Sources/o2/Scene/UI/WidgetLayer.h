#pragma once

#include "o2/Render/RectDrawable.h"
#include "o2/Scene/UI/WidgetLayerLayout.h"
#include "o2/Utils/Basic/ITree.h"
#include "o2/Utils/Editor/Attributes/ExpandedByDefaultAttribute.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2/Utils/Math/Layout.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Widget;

#if IS_EDITOR
	typedef SceneEditableObject WidgetLayerBase;
#else
	typedef ISerializable WidgetLayerBase;
#endif

	// ---------------------
	// Widget drawable layer
	// ---------------------
	class WidgetLayer: public WidgetLayerBase
	{
	public:
		PROPERTIES(WidgetLayer);
		PROPERTY(bool, enabled, SetEnabled, IsEnabled);                  // Enable property
		PROPERTY(float, depth, SetDepth, GetDepth);                      // Drawing depth (higher depths will draw later)
		PROPERTY(float, transparency, SetTransparency, GetTransparency); // Drawable transparency property
		PROPERTY(IRectDrawable*, drawable, SetDrawable, GetDrawable);    // Drawable property @EXPANDED_BY_DEFAULT

		ACCESSOR(WidgetLayer*, child, String, GetChild, GetAllChildLayers); // Child layer accessor

	public:
		WidgetLayerLayout layout; // Drawable layout @SERIALIZABLE

		String name;                // Name of layer @SERIALIZABLE
		Layout interactableLayout;  // Interactable area layout @SERIALIZABLE

	public:
		// Default constructor
		WidgetLayer();

		// Copy-constructor
		WidgetLayer(const WidgetLayer& other);

		// Destructor
		~WidgetLayer();

		// Copy-operator
		WidgetLayer& operator=(const WidgetLayer& other);

		// Returns pointer to owner widget
		Widget* GetOwnerWidget() const;

		// Returns link to prototype
		const WidgetLayer* GetPrototypeLink() const;

		// Sets layer drawable
		void SetDrawable(IRectDrawable* drawable);

		// Returns layer drawable
		IRectDrawable* GetDrawable() const;

		// Draws drawable
		void Draw();

		// Returns is layer enabled
		bool IsEnabled() const override;

		// Returns is layer enabled and all parent are enabled too
		bool IsEnabledInHierarchy() const override;

		// Sets enabling of layer
		void SetEnabled(bool enabled) override;

		// Sets parent layer
		void SetParent(WidgetLayer* parent);

		// Returns parent layer
		WidgetLayer* GetParent() const;

		// Adds new child layer and returns him
		WidgetLayer* AddChild(WidgetLayer* node);

		// Remove child layer and releases him if needs
		void RemoveChild(WidgetLayer* node, bool release = true);

		// Removes and releases all children nodes
		void RemoveAllChildren();

		// Return child layers
		Vector<WidgetLayer*>& GetChildren();

		// Returns constant child layers
		const Vector<WidgetLayer*>& GetChildren() const;

		// Adds child layer
		WidgetLayer* AddChildLayer(const String& name, IRectDrawable* drawable, const Layout& layout = Layout::BothStretch(),
								   float depth = 0.0f);

		// Returns child layer by path
		WidgetLayer* GetChild(const String& path);

		// Returns child layer by name
		WidgetLayer* FindChild(const String& name);

		// Returns child layer with type
		template<typename _type>
		_type* FindChild() const;

		// Returns all child layers
		Vector<WidgetLayer*> GetAllChilds() const;

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

		SERIALIZABLE(WidgetLayer);

	protected:
		struct ICopyVisitor
		{
			virtual ~ICopyVisitor() {}
			virtual void OnCopy(const WidgetLayer* source, WidgetLayer* target) = 0;
		};

		struct MakePrototypeCloneVisitor: public ICopyVisitor
		{
			void OnCopy(const WidgetLayer* source, WidgetLayer* target) override;
		};

		struct InstantiatePrototypeCloneVisitor: public ICopyVisitor
		{
			void OnCopy(const WidgetLayer* source, WidgetLayer* target) override;
		};

	protected:
		mutable ICopyVisitor* mCopyVisitor = nullptr; // Copy visitor. Used when instantiating prefab

		const WidgetLayer* mPrototypeLink = nullptr; // Linked prototype layer

		IRectDrawable* mDrawable = nullptr; // Drawable @SERIALIZABLE

		SceneUID mUID = 0; // Scene editor uid @SERIALIZABLE @IGNORE_DELTA_SEARCH

		bool mEnabled = true; // Is layer enabled

		float mTransparency = 1.0f;    // Layer transparency @SERIALIZABLE
		float mResTransparency = 1.0f; // Result drawable transparency, depends on parent transparency

		float mDepth = 0.0f; // Depth of drawable @SERIALIZABLE

		RectF mAbsolutePosition; // Result absolute drawable position
		RectF mInteractableArea; // Interactable area, depends on interactableLayout

		Widget* mOwnerWidget = nullptr; // Owner widget pointer 

		WidgetLayer*         mParent = nullptr; // Pointer to parent layer 
		Vector<WidgetLayer*> mChildren;         // Children layers

		bool mUpdatingLayout = false; // It is true when updating layout now, prevents recursive layout updating 

	protected:
		// Regular serializing without prototype
		void SerializeRaw(DataValue& node) const;

		// Regular deserializing without prototype
		void DeserializeRaw(const DataValue& node);

		// Regular serializing with prototype
		void SerializeWithProto(DataValue& node) const;

		// Regular deserializing with prototype
		void DeserializeWithProto(const DataValue& node);

		// Completion deserialization callback
		void OnDeserialized(const DataValue& node) override;

		// Completion deserialization delta callback
		void OnDeserializedDelta(const DataValue& node, const IObject& origin) override;

		// Sets owner widget for this and children
		void SetOwnerWidget(Widget* owner);

		// It is called when layout was changed, calls owner widget layout updating
		void OnLayoutChanged();

		// Updates drawable rect, calling when widget's layout was changed
		void UpdateLayout();

		// It is called when transparency was changed and updates children transparencies
		void UpdateResTransparency();

		// It is called when owner widget including in scene. Registers editable object and it's children
		void OnAddToScene();

		// It is called when owner widget excluding in scene. Unregisters editable object and it's children
		void OnRemoveFromScene();

		// Returns dictionary with all child layers
		Map<String, WidgetLayer*> GetAllChildLayers();

#if IS_EDITOR
	public:
		PROPERTY(bool, locked, SetLocked, IsLocked); // Is locked property

	public:
		// Returns true when object is on scene
		bool IsOnScene() const override;

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

		// Checks that this object can be added as child to another object
		bool CanBeParentedTo(const Type& parentType) override;

		// Returns is that type of object can be enabled and disabled
		bool IsSupportsDisabling() const override;

		// Returns is that type of object can be locked
		bool IsSupportsLocking() const override;

		// Returns is object locked, override when it's supports
		bool IsLocked() const override;

		// Returns is object locked and all parent are locked too
		bool IsLockedInHierarchy() const override;

		// Sets locking of object, override when it's supports
		void SetLocked(bool locked) override;

		// Returns is that type of object can be transformed
		bool IsSupportsTransforming() const override;

		// Returns transform, override when it's supports
		Basis GetTransform() const override;

		// Sets transform of object, override when it's supports
		void SetTransform(const Basis& transform) override;

		// Updates transform immediately
		void UpdateTransform();

		// Returns is object supports pivot 
		bool IsSupportsPivot() const override;

		// Sets transform pivot point
		void SetPivot(const Vec2F& pivot) override;

		// Returns transform pivot
		Vec2F GetPivot() const override;

		// Returns is that type of object can be transformed with layout
		bool IsSupportsLayout() const override;

		// Returns layout, override when it's supports
		Layout GetLayout() const override;

		// Sets layout of object, override when it's supports
		void SetLayout(const Layout& layout) override;

		// It is called when something changed in this object
		void OnChanged() override;

		// Beginning serialization callback
		void SerializeBasicOverride(DataValue& node) const;

		// Completion deserialization callback
		void DeserializeBasicOverride(const DataValue& node);

	protected:
		bool mIsLocked = false; // Is locked

	protected:
		// It is called before making prototype from this object
		void BeginMakePrototype() const override;

		// It is called before instantiate from this object
		void BeginInstantiatePrototype() const override;

#endif // IS_EDITOR

		friend class Widget;
		friend class WidgetLayerLayout;
	};

	template<typename _type>
	_type* WidgetLayer::FindChild() const
	{
		for (auto child : mChildren)
			if (child->mDrawable && child->mDrawable->GetType() == TypeOf(_type))
				return (_type*)(child->mDrawable);

		for (auto child : mChildren)
		{
			auto res = child->FindChild<_type>();
			if (res)
				return res;
		}

		return nullptr;
	}
}

CLASS_BASES_META(o2::WidgetLayer)
{
	BASE_CLASS(o2::SceneEditableObject);
}
END_META;
CLASS_FIELDS_META(o2::WidgetLayer)
{
	FIELD().NAME(enabled).PUBLIC();
	FIELD().NAME(depth).PUBLIC();
	FIELD().NAME(transparency).PUBLIC();
	FIELD().EXPANDED_BY_DEFAULT_ATTRIBUTE().NAME(drawable).PUBLIC();
	FIELD().NAME(child).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(layout).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(name).PUBLIC();
	FIELD().SERIALIZABLE_ATTRIBUTE().NAME(interactableLayout).PUBLIC();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mCopyVisitor).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mPrototypeLink).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(nullptr).NAME(mDrawable).PROTECTED();
	FIELD().IGNORE_DELTA_SEARCH_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0).NAME(mUID).PROTECTED();
	FIELD().DEFAULT_VALUE(true).NAME(mEnabled).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(1.0f).NAME(mTransparency).PROTECTED();
	FIELD().DEFAULT_VALUE(1.0f).NAME(mResTransparency).PROTECTED();
	FIELD().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.0f).NAME(mDepth).PROTECTED();
	FIELD().NAME(mAbsolutePosition).PROTECTED();
	FIELD().NAME(mInteractableArea).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mOwnerWidget).PROTECTED();
	FIELD().DEFAULT_VALUE(nullptr).NAME(mParent).PROTECTED();
	FIELD().NAME(mChildren).PROTECTED();
	FIELD().DEFAULT_VALUE(false).NAME(mUpdatingLayout).PROTECTED();
	FIELD().NAME(locked).PUBLIC();
	FIELD().DEFAULT_VALUE(false).NAME(mIsLocked).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::WidgetLayer)
{

	typedef Map<String, WidgetLayer*> _tmp1;

	PUBLIC_FUNCTION(Widget*, GetOwnerWidget);
	PUBLIC_FUNCTION(const WidgetLayer*, GetPrototypeLink);
	PUBLIC_FUNCTION(void, SetDrawable, IRectDrawable*);
	PUBLIC_FUNCTION(IRectDrawable*, GetDrawable);
	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(bool, IsEnabled);
	PUBLIC_FUNCTION(bool, IsEnabledInHierarchy);
	PUBLIC_FUNCTION(void, SetEnabled, bool);
	PUBLIC_FUNCTION(void, SetParent, WidgetLayer*);
	PUBLIC_FUNCTION(WidgetLayer*, GetParent);
	PUBLIC_FUNCTION(WidgetLayer*, AddChild, WidgetLayer*);
	PUBLIC_FUNCTION(void, RemoveChild, WidgetLayer*, bool);
	PUBLIC_FUNCTION(void, RemoveAllChildren);
	PUBLIC_FUNCTION(Vector<WidgetLayer*>&, GetChildren);
	PUBLIC_FUNCTION(const Vector<WidgetLayer*>&, GetChildren);
	PUBLIC_FUNCTION(WidgetLayer*, AddChildLayer, const String&, IRectDrawable*, const Layout&, float);
	PUBLIC_FUNCTION(WidgetLayer*, GetChild, const String&);
	PUBLIC_FUNCTION(WidgetLayer*, FindChild, const String&);
	PUBLIC_FUNCTION(Vector<WidgetLayer*>, GetAllChilds);
	PUBLIC_FUNCTION(void, SetDepth, float);
	PUBLIC_FUNCTION(float, GetDepth);
	PUBLIC_FUNCTION(void, SetTransparency, float);
	PUBLIC_FUNCTION(float, GetTransparency);
	PUBLIC_FUNCTION(float, GetResTransparency);
	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(const RectF&, GetRect);
	PROTECTED_FUNCTION(void, SerializeRaw, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeRaw, const DataValue&);
	PROTECTED_FUNCTION(void, SerializeWithProto, DataValue&);
	PROTECTED_FUNCTION(void, DeserializeWithProto, const DataValue&);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataValue&);
	PROTECTED_FUNCTION(void, OnDeserializedDelta, const DataValue&, const IObject&);
	PROTECTED_FUNCTION(void, SetOwnerWidget, Widget*);
	PROTECTED_FUNCTION(void, OnLayoutChanged);
	PROTECTED_FUNCTION(void, UpdateLayout);
	PROTECTED_FUNCTION(void, UpdateResTransparency);
	PROTECTED_FUNCTION(void, OnAddToScene);
	PROTECTED_FUNCTION(void, OnRemoveFromScene);
	PROTECTED_FUNCTION(_tmp1, GetAllChildLayers);
	PUBLIC_FUNCTION(bool, IsOnScene);
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
	PUBLIC_FUNCTION(bool, CanBeParentedTo, const Type&);
	PUBLIC_FUNCTION(bool, IsSupportsDisabling);
	PUBLIC_FUNCTION(bool, IsSupportsLocking);
	PUBLIC_FUNCTION(bool, IsLocked);
	PUBLIC_FUNCTION(bool, IsLockedInHierarchy);
	PUBLIC_FUNCTION(void, SetLocked, bool);
	PUBLIC_FUNCTION(bool, IsSupportsTransforming);
	PUBLIC_FUNCTION(Basis, GetTransform);
	PUBLIC_FUNCTION(void, SetTransform, const Basis&);
	PUBLIC_FUNCTION(void, UpdateTransform);
	PUBLIC_FUNCTION(bool, IsSupportsPivot);
	PUBLIC_FUNCTION(void, SetPivot, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetPivot);
	PUBLIC_FUNCTION(bool, IsSupportsLayout);
	PUBLIC_FUNCTION(Layout, GetLayout);
	PUBLIC_FUNCTION(void, SetLayout, const Layout&);
	PUBLIC_FUNCTION(void, OnChanged);
	PUBLIC_FUNCTION(void, SerializeBasicOverride, DataValue&);
	PUBLIC_FUNCTION(void, DeserializeBasicOverride, const DataValue&);
	PROTECTED_FUNCTION(void, BeginMakePrototype);
	PROTECTED_FUNCTION(void, BeginInstantiatePrototype);
}
END_META;
