#include "o2Editor/stdafx.h"
#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Button.h"
#include "o2/Utils/Editor/EditorScope.h"
#include "o2Editor/AnimationWindow/AnimationWindow.h"
#include "o2Editor/AnimationWindow/TrackControls/KeyFramesTrackControl.h"
#include "o2Editor/AnimationWindow/TrackControls/MapKeyFramesTrackControl.h"
#include "o2Editor/AnimationWindow/TrackControls/Vec2KeyFramesTrackControl.h"
#include "o2Editor/AnimationWindow/Tree.h"
#include <Ref.hpp>

namespace Editor
{
	AnimationTree::AnimationTree() :
		Tree()
	{
		SetRearrangeType(Tree::RearrangeType::Disabled);
		InitializeContext();
	}

	AnimationTree::AnimationTree(const AnimationTree& other) :
		Tree(other)
	{
		SetRearrangeType(Tree::RearrangeType::Disabled);
		InitializeContext();
	}

	AnimationTree::~AnimationTree()
	{
		if (mRootValue)
			delete mRootValue;
	}

	AnimationTree& AnimationTree::operator=(const AnimationTree& other)
	{
		delete mZebraBackLine;

		Tree::operator=(other);

		mZebraBackLine = other.mZebraBackLine->CloneAs<Sprite>();
		InitializeContext();

		return *this;
	}

	void AnimationTree::Draw()
	{
		o2Render.EnableScissorTest(mAbsoluteClipArea);
		DrawZebraBack();
		o2Render.DisableScissorTest();

		mAnimationWindow->mHandlesSheet->Draw();

		// Disable zebra
		auto buf = mZebraBackLine;
		mZebraBackLine = nullptr;

		Tree::Draw();

		// Enable
		mZebraBackLine = buf;

		mAnimationWindow->mHandlesSheet->UpdateInputDrawOrder();
	}

	void AnimationTree::SetAnimation(const Ref<AnimationClip>& animation)
	{
		mAnimationWindow->mHandlesSheet->UnregAllTrackControls();

		mPrevSelectedNodes.Clear();

		RebuildAnimationTree();
		ExpandAll();
		OnObjectsChanged({ (void*)mRootValue });
	}

	void AnimationTree::SetTreeWidth(const float width)
	{
		mTreeWidth = width;
		UpdateTreeWidth();
	}

	void AnimationTree::SetAnimationValueColor(const String& path, const Color4& color)
	{
		TrackNode* node = mRootValue;
		while (!path.IsEmpty())
		{
			int fnd = path.Find('/', 0);
			String pathPart = path.SubStr(0, fnd);

			for (const auto& child : node->children)
			{
				if (child->name == pathPart)
				{
					node = child;
					break;
				}
			}
			
			if (fnd < 0)
				break;

			path = path.SubStr(fnd + 1);
		}

		if (!node)
			return;

		node->color = color;

		if (node->trackControl)
			node->trackControl->SetCurveViewColor(color);
	}

	float AnimationTree::GetLineNumber(float worldPosition) const
	{
		return (layout->GetWorldTop() - worldPosition + mScrollPos.y) / mNodeWidgetSample->layout->GetMinHeight();
	}

	float AnimationTree::GetLineWorldPosition(float lineNumber) const
	{
		return -(lineNumber * mNodeWidgetSample->layout->GetMinHeight() - mScrollPos.y - layout->GetWorldTop());
	}

	String AnimationTree::GetCreateMenuCategory()
	{
		return "UI/Editor";
	}

	void AnimationTree::InitializeContext()
	{
		mContextMenu = o2UI.CreateWidget<ContextMenu>();

		mContextMenu->AddItem("New", [&]() {});
		mContextMenu->AddItem("Save", [&]() {}, ImageAssetRef(), ShortcutKeys('S', true));
		mContextMenu->AddItem("Save as...", [&]() {}, ImageAssetRef(), ShortcutKeys('S', true, true));
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Delete property", THIS_FUNC(OnDeletePropertyPressed));
		mContextMenu->AddItem("---");
		mContextMenu->AddItem("Add properties", [&]() { PropertiesListDlg::Show(mAnimationWindow->mAnimation, mAnimationWindow->mTargetActor); });

		onFocused = [&]() { mAnimationWindow->mHandlesSheet->GetContextMenu()->SetItemsMaxPriority(); mContextMenu->SetItemsMaxPriority(); };
		onUnfocused = [&]() { mAnimationWindow->mHandlesSheet->GetContextMenu()->SetItemsMinPriority(); mContextMenu->SetItemsMinPriority(); };

		AddInternalWidget(mContextMenu);
	}

	void AnimationTree::RebuildAnimationTree()
	{
		if (mRootValue)
			delete mRootValue;

		mRootValue = nullptr;

		if (!mAnimationWindow->mAnimation)
			return;

		mAnimationValuesCount = mAnimationWindow->mAnimation->GetTracks().Count();

		mRootValue = mnew TrackNode();
		mRootValue->name = "Track name";

		if (mAnimationWindow->mPlayer)
		{
			for (auto trackPlayer : mAnimationWindow->mPlayer->GetTrackPlayers())
				AddAnimationTrack(trackPlayer->GetTrack(), trackPlayer);
		}
		else
		{
			for (const auto& track : mAnimationWindow->mAnimation->GetTracks())
				AddAnimationTrack(track, nullptr);
		}

		UpdateNodesStructure();
	}

	void AnimationTree::AddAnimationTrack(const Ref<IAnimationTrack>& track, const Ref<IAnimationTrack::IPlayer>& player /*= nullptr*/)
	{
		TrackNode* current = nullptr;

		int lastDel = 0;
		while (lastDel >= 0)
		{
			int del = track->path.Find('/', lastDel);
			String subPath = track->path.SubStr(lastDel, del);
			TrackNode* next = (current ? current->children : mRootValue->children)
				.FindOrDefault([&](const Ref<TrackNode>& x) { return x->name == subPath; });

			if (!next)
			{
				next = mnew TrackNode();
				next->name = subPath;
				next->path = track->path;

				if (current)
				{
					next->parent = current;
				}
				else
				{
					next->parent = mRootValue;
				}

				if (player)
				{
					Ref<TreeItem> ti = MakeShared<TreeItem>();
					ti->SetWidgetCreateCallback([&]() {
						return DO_ADD_REF_NEW<TrackControls::KeyFramesTrackControl>(RefPtr<TreeItem>(ti), mAnimationWindow, player,
							next->path, next->name, next->color, next->bDisabled);
					});
					next->trackControl = ti->As<TrackControls::KeyFramesTrackControl>();

					if (player)
					{
						mAnimationWindow->mHandlesSheet->RegTrackControl(ti.Get(), player, next->name);
					}
				}

				(current ? current->children : mRootValue->children).Add(next);
			}

			current = next;

			if (del < 0)
				break;

			lastDel = del + 1;
		}

		if (mAnimationWindow->mPlayer && player)
			mAnimationWindow->mHandlesSheet->RegTrackPlayer(player);
	}

	void AnimationTree::OnDeletePropertyPressed()
	{
		if (!mSelectedItems.Empty())
			RemoveProperty(mSelectedItems[0]->As<TrackNode>());
	}

	void AnimationTree::RemoveProperty(const Ref<TrackNode>& property)
	{
		property->RemoveFromParent();

		auto& children = (property->parent ? property->parent->children : mRootValue->children);
		children.Remove(property);

		mAnimationWindow->mHandlesSheet->UnregControl(property->trackControl.Get());

		property->trackControl = nullptr;

		property->parent = nullptr;
	}

	void AnimationTree::OnObjectsChanged(const Array<void*>& objects)
	{
		mAnimationWindow->OnAnyObjectChanged(objects);
	}
}using namespace std;

template<typename T>
class Ref {
public:
	Ref(T* ptr) : mPtr(ptr) {}
	Ref(const Ref<T>& other) : mPtr(other.mPtr) {}
	T* operator->() const { return mPtr; }
	T& operator*() const { return *mPtr; }
	operator bool() const { return mPtr != nullptr; }
	bool operator==(const Ref<T>& other) const { return mPtr == other.mPtr; }
	bool operator!=(const Ref<T>& other) const { return mPtr != other.mPtr; }
private:
	T* mPtr;
};

template<typename T>
class WeakRef {
public:
	WeakRef(T* ptr) : mPtr(ptr) {}
	WeakRef(const WeakRef<T>& other) : mPtr(other.mPtr) {}
	Ref<T> Lock() const { return Ref<T>(mPtr); }
	operator bool() const { return mPtr != nullptr; }
	bool operator==(const WeakRef<T>& other) const { return mPtr == other.mPtr; }
	bool operator!=(const WeakRef<T>& other) const { return mPtr != other.mPtr; }
private:
	T* mPtr;
};

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args) {
    return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
class DataValue;

template<typename T>
class AnimationTrack {
};

template<typename T>
class KeyFramesTrackControl;

class AnimationWindow;

template<typename T>
class AnimationTreeNode;

class AnimationTree {
public:
	using TrackNode = AnimationTreeNode<void*>;

	void RebuildAnimationTree() {
		// TODO: Implement RebuildAnimationTree
	}

	void ExpandAll() {
		// TODO: Implement ExpandAll
	}

	void UpdateTreeWidth() {
		// TODO: Implement UpdateTreeWidth
	}

	void SetCurveViewMode(bool enable) {
		// TODO: Implement SetCurveViewMode
	}

	void OnAnimationChanged() {
		// TODO: Implement OnAnimationChanged
	}

	void* GetObjectParent(void* object) {
		// TODO: Implement GetObjectParent
		return nullptr;
	}

	vector<void*> GetObjectChilds(void* object) {
		// TODO: Implement GetObjectChilds
		return { (void*)mRootValue.Get() };
	}

	string GetObjectDebug(void* object) {
		// TODO: Implement GetObjectDebug
		auto treeNode = reinterpret_cast<TrackNode*>(object);
		return treeNode ? treeNode->name : "Empty";
	}

	void FillNodeDataByObject(TreeNode* nodeWidget, void* object) {
		// TODO: Implement FillNodeDataByObject
		auto node = reinterpret_cast<AnimationTreeNode<void*>*>(nodeWidget);
		node->Setup(reinterpret_cast<TrackNode*>(object), mAnimationWindow->mTimeline, mAnimationWindow->mHandlesSheet);
	}

	void FreeNodeData(TreeNode* nodeWidget, void* object) {
		// TODO: Implement FreeNodeData
		auto node = reinterpret_cast<AnimationTreeNode<void*>*>(nodeWidget);
		node->Free();
	}

	void UpdateVisibleNodes() {
		Tree::UpdateVisibleNodes();
		UpdateTreeWidth();
	}

	void OnNodeRBClick(TreeNode* node) {
		o2UI.FocusWidget(Ref(this));
		mContextMenu->Show();
	}

	void OnNodesSelectionChanged(vector<void*> objects) {
		for (auto node : mPrevSelectedNodes) {
			node->trackControl->SetActive(false);
		}

		mAnimationWindow->mHandlesSheet->DeselectAll();

		mPrevSelectedNodes.clear();
		for (auto obj : objects) {
			auto node = reinterpret_cast<TrackNode*>(obj);
			mPrevSelectedNodes.push_back(node);

			node->trackControl->SetActive(true);

			for (auto handle : node->trackControl->GetKeyHandles()) {
				handle->handle->SetSelected(true);
			}
		}
	}

	TreeNode* CreateTreeNodeWidget() {
		PushEditorScopeOnStack scope;
		return Tree::CreateTreeNodeWidget();
	}

	void OnDeletePropertyPressed() {
		for (auto obj : GetSelectedObjects()) {
			auto data = reinterpret_cast<TrackNode*>(obj);
			mAnimationWindow->mAnimation->RemoveTrack(data->path);
		}
	}

private:
	class TreeNode {
	};

	class AnimationWindow {
	public:
		class AnimationTreeNodeBase : public TreeNode {
		};

		class TrackNode : public AnimationTreeNodeBase {
		};
	};

	class DynamicCast {
	public:
		template<typename T>
		static Ref<T> Cast(Ref<void> ptr) {
			return dynamic_pointer_cast<T>(ptr);
		}
	};

	class AnimationTreeNodeBase : public TreeNode {
	};

	class AnimationTreeNodeData {
	};

	template<typename T>
	class AnimationTreeNode : public AnimationTreeNodeBase {
	public:
		AnimationTreeNode() = default;

		AnimationTreeNode(const AnimationTreeNode& other) :
			AnimationTreeNodeBase(other),
			mTimeline(other.mTimeline),
			mHandlesSheet(other.mHandlesSheet),
			mData(other.mData),
			mNameDrawable(other.mNameDrawable),
			mTrackControl(other.mTrackControl) {
			InitializeControls();
		}

		AnimationTreeNode& operator=(const AnimationTreeNode& other) {
			AnimationTreeNodeBase::operator=(other);
			mTimeline = other.mTimeline;
			mHandlesSheet = other.mHandlesSheet;
			mData = other.mData;
			mNameDrawable = other.mNameDrawable;
			mTrackControl = other.mTrackControl;
			InitializeControls();
			return *this;
		}

		void Setup(Ref<AnimationTreeNodeData> node, Ref<AnimationTimeline> timeline, Ref<KeyHandlesSheet> handlesSheet) {
			mTimeline = timeline;
			mHandlesSheet = handlesSheet;

			mTimeline->onViewChanged -= THIS_FUNC(UpdateTrackControlView);
			mTimeline->onViewChanged += THIS_FUNC(UpdateTrackControlView);

			mData = node;
			mNameDrawable->text = node->name;

			InitilizeTrackControl();

			mData->trackControl = mTrackControl;
		}

		void Free() {
			FreeTrackControl();
		}

		void SetTreeWidth(float width) {
			if (mTrackControl) {
				*mTrackControl->layout = WidgetLayout::BothStretch(width, 0, 0, 0);

				float right = width - layout->GetOffsetLeft();
				if (auto controls = mTrackControl->GetTreePartControls()) {
					*controls->layout = WidgetLayout::VerStretch(HorAlign::Left, mPropertyBorder, mPropertyBorder + 1, mPropertySize, right - mPropertySize - mPropertyBorder);
				}
			}
		}

		void OnDoubleClicked(const Input::Cursor& cursor) {
			if (mTrackControl)
				mTrackControl->InsertNewKey(mTimeline->WorldToLocal(cursor.position.x));
		}

		Map<const Type*, o2::Vector<ITrackControl*>> mTrackControlsCache;

		void InitilizeTrackControl() {
			PushEditorScopeOnStack scope;

			static Map<const Type*, const Type*> trackToControlTrackTypes = {
				{&TypeOf(AnimationTrack<float>), &TypeOf(KeyFramesTrackControl<AnimationTrack<float>>)}
			};

			auto typeIndex = typeid(mData->track).name();
			auto controlTrackTypeIndex = trackToControlTrackTypes.find(typeIndex);
			if (controlTrackTypeIndex != trackToControlTrackTypes.end()) {
				auto nodeTrackControl = make_shared<KeyFramesTrackControl<AnimationTrack<float>>>();
				auto& nodeTrackControls = mTrackControlsCache[typeIndex];
				nodeTrackControls.push_back(nodeTrackControl.get());
				mTrackControl = nodeTrackControl;
				return;
			}

			auto existingControlsIndex = mTrackControlsCache.find(typeIndex);
			if (existingControlsIndex != mTrackControlsCache.end()) {
				mTrackControl = existingControlsIndex->second.front();
				return;
			}

			auto nodeTrackControl = DynamicCast::Cast<ITrackControl>(mmake<KeyFramesTrackControl<AnimationTrack<float>>>());
			mTrackControl = nodeTrackControl;
			auto& nodeTrackControls = mTrackControlsCache[typeIndex];
			nodeTrackControls.push_back(nodeTrackControl.Get());
		}

		void FreeTrackControl() {
			mTrackControl.reset();
		}

		void UpdateTrackControlView() {
			if (mTrackControl) {
				float start = mTimeline->GetViewStartTime();
				float end = mTimeline->GetViewEndTime();

				mTrackControl->UpdateView(start, end);
			}
		}

	private:
		Track<Ref<AnimationTrack<float>>> mData;
		Ref<AnimationTimeline> mTimeline;
		Ref<KeyHandlesSheet> mHandlesSheet;
		Ref<Text> mNameDrawable;
		Ref<ITrackControl> mTrackControl;

		const int mPropertyBorder = 4;
		const int mPropertySize = 60;

		void InitializeControls() {
			mNameDrawable = GetLayerDrawable<Text>("name");
		}
	};

	Ref<AnimationTreeNodeBase> mRootValue;
	AnimationWindow* mAnimationWindow;
	vector<Ref<AnimationTreeNodeBase>> mPrevSelectedNodes;
};

template<typename T>
Ref<AnimationTreeNodeBase> GetTreeNode(void* object) {
	return DynamicCast::Cast<AnimationTreeNode<T>>(reinterpret_cast<Ref<AnimationTreeNodeData>*>(object));
}

template<typename T>
vector<void*> GetObjectChilds(Ref<AnimationTreeNodeBase> object) {
	auto treeNode = DynamicCast::Cast<AnimationTreeNode<T>>(object);
	return treeNode ? treeNode->mData->children.Cast<void*>() : vector<void*>();
}

template<typename T>
class Ref {
public:
	Ref() : mPtr(nullptr) {}

	Ref(T* ptr) : mPtr(ptr) {}

	Ref(const Ref<T>& other) : mPtr(other.mPtr) {}

	Ref(Ref<T>&& other) noexcept : mPtr(other.mPtr) {}

	Ref<T>& operator=(const Ref<T>& other) {
		mPtr = other.mPtr;
		return *this;
	}

	Ref<T>& operator=(Ref<T>&& other) noexcept {
		mPtr = other.mPtr;
		return *this;
	}

	T* operator->() const {
		return mPtr;
	}

	T& operator*() const {
		return *mPtr;
	}

	operator bool() const {
		return mPtr != nullptr;
	}

	bool operator==(const Ref<T>& other) const {
		return mPtr == other.mPtr;
	}

	bool operator!=(const Ref<T>& other) const {
		return mPtr != other.mPtr;
	}

private:
	T* mPtr;
};

template<typename T>
class DynamicCast {
public:
	template<typename U>
	static Ref<U> Cast(Ref<T> ptr) {
		return dynamic_pointer_cast<U>(ptr);
	}
};

class TreeNode {
public:
	virtual ~TreeNode() {}

	virtual void OnDoubleClicked(const Input::Cursor& cursor) {}
};

template<typename T>
class AnimationTreeNode : public TreeNode {
public:
	void Setup(Ref<T> node) {
		mData = node;
	}

	Ref<T> GetData() const {
		return mData;
	}

	virtual void OnDoubleClicked(const Input::Cursor& cursor) {
		// TODO: Implement OnDoubleClicked
	}

private:
	Ref<T> mData;
};

template<typename T>
class AnimationTreeNodeData {
public:
};

template<typename T>
class AnimationTrack {
public:
	virtual ~AnimationTrack() {}

	virtual void InsertKeyFrame(float time, const T& value) = 0;

	virtual T Evaluate(float time) const = 0;

	virtual const string& GetPath() const = 0;
};

template<typename T>
class KeyFramesTrackControl {
public:
	virtual ~KeyFramesTrackControl() {}

	virtual void InsertNewKey(float time) = 0;

	virtual void UpdateView(float startTime, float endTime) = 0;

	virtual Ref<AnimationTrack<T>> GetTrack() const = 0;

	virtual vector<void*> GetKeyHandles() const = 0;
};

template<typename T>
class Vector {
public:
	using value_type = T;
	using iterator = T*;
	using const_iterator = const T*;

	Vector() : mData(nullptr), mSize(0), mCapacity(0) {}

	Vector(size_t size) : mData(static_cast<value_type*>(::operator new(size * sizeof(value_type)))), mSize(size), mCapacity(size) {
		for (size_t i = 0; i < mSize; ++i) {
			new#include <memory>
#include <map>
#include <iostream>
#include <vector>

template<typename T>
class Ref
{
public:
	Ref(T* ptr) : m_ptr(ptr) {}

	T* get() const { return m_ptr; }

	T* operator->() const { return m_ptr; }

	T& operator*() const { return *m_ptr; }

private:
	T* m_ptr;
};

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
	return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename U>
struct DynamicCast
{
	static Ref<U> cast(const Ref<T>& ptr)
	{
		return Ref<U>(dynamic_cast<U*>(ptr.get()));
	}
};

template<typename T>
class Type
{
public:
	Type(const char* name) : m_name(name) {}

	const char* GetName() const { return m_name; }

	template<typename U>
	bool IsSameType(const Type<U>& other) const
	{
		return std::is_same<T, U>::value;
	}

	Type<T>* DynamicCastToType(const Type<T>* other) const
	{
		return const_cast<Type<T>*>(other);
	}

	template<typename U>
	Type<U>* DynamicCastToType(Type<U>* other) const
	{
		return nullptr;
	}

	template<typename U>
	U* DynamicCastToIObject(const U* object) const
	{
		auto type = dynamic_cast<const Type<U>*>(this);
		return const_cast<U*>(type ? object : nullptr);
	}

	template<typename U>
	Type<U>* DynamicCastToType(const U* object) const
	{
		auto type = dynamic_cast<const Type<U>*>(this);
		return const_cast<Type<U>*>(type);
	}

private:
	const char* m_name;
};

template<typename T>
Type<T> TypeOf(const char*);

// --- Begin Generated Code ---

namespace Editor
{
	class AnimationTrackBase {};

	template<typename T>
	class AnimationTrack : public AnimationTrackBase {};

	template<typename T>
	class KeyFramesTrackControl {};

	using Vec2F = std::pair<float, float>;

	class Vec2KeyFramesTrackControl : public KeyFramesTrackControl<Vec2F> {};

	using Color4 = std::array<float, 4>;

	template<typename T>
	class KeyFramesTrackControl<T[]> {};

	class MapKeyFramesTrackControl : public KeyFramesTrackControl<std::map<float, Ref<AnimationTrackBase>>> {};

	class AnimationTreeNode
	{
	private:
		Ref<AnimationTrackBase> mTrack;
		Ref<ITrackControl> mTrackControl;
		Ref<AnimationTree> mOwnerTree;
		mutable std::map<const Type<AnimationTrackBase>*, std::vector<Ref<ITrackControl>>> mTrackControlsCache;
		Ref<HandlesSheet> mHandlesSheet;
		Ref<Timeline> mTimeline;
		std::string mDataName;
		Color4 mDataColor;;

		// ...

	public:
		void Initialize(Ref<AnimationTrackBase> track, const std::string& dataName, const Color4& dataColor)
		{
			// ...

			mTrack = track;
			mDataName = dataName;
			mDataColor = dataColor;

			// ...
		}

		void UpdateTrackControlView()
		{
			if (mTrackControl)
				mTrackControl->UpdateHandles();
		}

	};

	class AnimationTree {};
}

// --- META ---

template<>
Type<Editor::AnimationTrackBase> TypeOf<Editor::AnimationTrackBase>(const char* name) { return Type<Editor::AnimationTrackBase>(name); }

template<>
Type<Editor::AnimationTreeNode> TypeOf<Editor::AnimationTreeNode>(const char* name) { return Type<Editor::AnimationTreeNode>(name); }

template<>
Type<Editor::AnimationTree> TypeOf<Editor::AnimationTree>(const char* name) { return Type<Editor::AnimationTree>(name); }

// --- END META ---
