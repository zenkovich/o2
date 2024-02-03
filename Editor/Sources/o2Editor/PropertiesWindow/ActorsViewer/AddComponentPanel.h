#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/Widgets/Tree.h"
#include <memory>

using namespace o2;

namespace o2
{
	class EditBox;
	class Button;
}

namespace Editor
{
	class ActorViewer;
	class ComponentsTree;

	// --------------------------------------------------------------------------------
	// Add component to actor panel. Shows filter input, add button and components tree
	// --------------------------------------------------------------------------------
	class AddComponentPanel : public Widget, public CursorEventsArea, public KeyboardEventsListener
	{
	public:
		// Default constructor
		AddComponentPanel();

		// Constructor
		AddComponentPanel(ActorViewer* viewer);

		// Draws widget, calls OnDrawn for CursorEventsListener
		void Draw() override;

		// Returns filter widget
		Ref<const EditBox> GetFilter() const;

		// Returns tree widget
		Ref<const ComponentsTree> GetTree() const;

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point) override;

		// Returns true when input events can be handled by down listeners
		bool IsInputTransparent() const override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(AddComponentPanel);

	private:
		ActorViewer* mViewer = nullptr; // Owner actors viewer

		Ref<const EditBox> mFilterBox = nullptr; // Components names filter edit box, updates list of component when edit
		Ref<const Button> mAddButton = nullptr; // Add button
		Ref<const ComponentsTree> mTree = nullptr; // Components tree

	private:
		// Called when add button pressed. Adds selected component to target actors from viewer
		void OnAddPressed();

		// Creates component by type
		void CreateComponent(const ObjectType* objType);

		// Called when tree node was double clicked
		void OnNodeDblClick(const Ref<const TreeNode>& nodeWidget);

		// Called when key was released. When returns has pressed, component is creating
		void OnKeyReleased(const Input::Key& key) override;
	};
	
	// ---------------------------------------------------------------------
	// Components tree. Builds data tree by available components with filter
	// ---------------------------------------------------------------------
	class ComponentsTree : public Tree
	{
	public:
		struct NodeData
		{
			NodeData* parent = nullptr;
			Vector<NodeData*> children;

			String name;
			String path;
			String icon = "ui/UI4_component_icon.png";

			const Type* type = nullptr;

		public:
			~NodeData();

			void Clear();
			NodeData* AddChild(const String& name, const Type* type);
		};

	public:
		// Default constructor
		ComponentsTree();

		explicit ComponentsTree(const ComponentsTree& other);

		ComponentsTree& operator=(const ComponentsTree& other);

		// Refreshes components list
		void Refresh();

		// Sets filter and refreshes tree
		void SetFilter(const WString& filter);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ComponentsTree);

	private:
		WString mFilterStr; // Filtering string
		NodeData mRoot; // Root properties data node

	private:
		// Updates visible nodes (calculates range and initializes nodes), enables editor mode
		void UpdateVisibleNodes() override;

		// Gets tree node from pool or creates new, enables editor mode
		TreeNode* CreateTreeNodeWidget() override;

		// Returns object's parent
		void* GetObjectParent(void* object) override;

		// Returns object's children
		Vector<void*> GetObjectChilds(void* object) override;

		// Returns debugging string for object
		String GetObjectDebug(void* object) override;

		// Sets nodeWidget data by object
		void FillNodeDataByObject(TreeNode* nodeWidget, const Ref<const void>& object) override;

		void OnDeserialized(const DataValue& node) override;

		friend class ComponentsTreeNode;
	};

	// --------------------------------------------------------------------------
	// Components tree node. Can bea  group of components or the component itself
	// --------------------------------------------------------------------------
	class ComponentsTreeNode : public TreeNode
	{
	public:
		Ref<const ComponentsTree::NodeData> data;

	public:
		// Default constructor
		ComponentsTreeNode();

		explicit ComponentsTreeNode(const ComponentsTreeNode& other);

		ComponentsTreeNode& operator=(const ComponentsTreeNode& other);

		// Initializes node by data
		void Setup(const Ref<const ComponentsTree::NodeData>& data, const Ref<const ComponentsTree>& tree);

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(ComponentsTreeNode);

	private:
		Ref<const Text> mName;
		Ref<const Sprite> mIcon;

		Ref<const ComponentsTree> mTree;

	private:
		// Called on deserialization, initializes controls
		void OnDeserialized(const DataValue& node) override;

		// initializes controls and widgets
		void InitializeControls();

		friend class ComponentsTree;
	};
}
// --- META ---#include <memory>

template <typename T>
class Ref {
public:
    Ref(T* ptr = nullptr) : m_ptr(ptr) {}
    
    T* operator->() { return m_ptr; }
    const T* operator->() const { return m_ptr; }
    
    T& operator*() { return *m_ptr; }
    const T& operator*() const { return *m_ptr; }
    
    operator bool() const { return m_ptr != nullptr; }
    bool operator==(const Ref<T>& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const Ref<T>& other) const { return m_ptr != other.m_ptr; }
    
private:
    T* m_ptr;
};

template <typename T>
Ref<T> mmake() {
    return Ref<T>(new T());
}

template <typename T>
class WeakRef {
public:
    WeakRef(Ref<T> ref = nullptr) : m_ref(ref) {}
    
    Ref<T> lock() const { return m_ref; }
    
    operator bool() const { return m_ref != nullptr; }
    bool operator==(const WeakRef<T>& other) const { return m_ref == other.m_ref; }
    bool operator!=(const WeakRef<T>& other) const { return m_ref != other.m_ref; }
    
private:
    Ref<T> m_ref;
};

template <typename T>
class DynamicCast {
public:
    template <typename U>
    static Ref<U> cast(Ref<T> ref) {
        return std::dynamic_pointer_cast<U>(ref);
    }
};

class EditBox {};
class ComponentsTree {};
class Widget {};
class CursorEventsArea {};
class KeyboardEventsListener {};
class TreeNode {};
class DataValue {};
class ActorViewer {};

using Vec2F = std::pair<float, float>;
using WString = std::wstring;
using String = std::string;

// --- CLASS_BASES_META ---

namespace Editor {
    class AddComponentPanel : public o2::Widget, public o2::CursorEventsArea, public o2::KeyboardEventsListener {
    public:
        AddComponentPanel() {}
        AddComponentPanel(Ref<ActorViewer> viewer) : mViewer(viewer) {}
        void Draw() {}
        EditBox* GetFilter() { return mFilterBox; }
        ComponentsTree* GetTree() { return mTree; }
        bool IsUnderPoint(const Vec2F& point) {}
        bool IsInputTransparent() {}
        static String GetCreateMenuCategory() {}
    private:
        void OnAddPressed() {}
        void CreateComponent(const ObjectType* type) {}
        void OnNodeDblClick(TreeNode* node) {}
        void OnKeyReleased(const Input::Key& key) {}
        
        Ref<ActorViewer> mViewer;
        Ref<EditBox> mFilterBox;
        Ref<> mAddButton;
        Ref<ComponentsTree> mTree;
    };

    class ComponentsTree : public o2::Tree {
    public:
        ComponentsTree() {}
        ComponentsTree(const ComponentsTree& other) {}
        void Refresh() {}
        void SetFilter(const WString& filter) {}
        static String GetCreateMenuCategory() {}
    private:
        void UpdateVisibleNodes() {}
        TreeNode* CreateTreeNodeWidget() {}
        void* GetObjectParent(void* object) {}
        Vector<void*> GetObjectChilds(void* object) {}
        String GetObjectDebug(void* object) {}
        void FillNodeDataByObject(TreeNode* node, void* object) {}
        void OnDeserialized(const DataValue& value) {}
        
        String mFilterStr;
        Ref<> mRoot;
    };

    class ComponentsTreeNode : public o2::TreeNode {
    public:
        ComponentsTreeNode() {}
        ComponentsTreeNode(const ComponentsTreeNode& other) {}
        void Setup(ComponentsTree::NodeData* data, ComponentsTree* tree) {}
        static String GetCreateMenuCategory() {}
    private:
        void OnDeserialized(const DataValue& value) {}
        void InitializeControls() {}
        
        Ref<ComponentsTree::NodeData> data;
        String mName;
        String mIcon;
        WeakRef<ComponentsTree> mTree;
    };
}

// --- END META ---