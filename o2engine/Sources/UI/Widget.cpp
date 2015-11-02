#include "Widget.h"

#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"

namespace o2
{
	IOBJECT_CPP(Widget);

	Widget::Widget():
		mParent(nullptr)
	{
		layout.mOwner = this;
		InitializeProperties();
	}

	Widget::Widget(const Widget& other):
		mName(other.mName), layout(other.layout), mParent(nullptr)
	{
		layout.mOwner = this;
		InitializeProperties();

		for (auto layer : other.mLayers)
		{
			auto newLayer = mnew WidgetLayer(*layer);
			newLayer->mOwnerWidget = this;
			mLayers.Add(newLayer);
		}

		for (auto child : other.mChilds)
		{
			auto cloned = child->Clone();
			mChilds.Add(cloned);
			cloned->mParent = this;
		}

		for (auto state : other.mStates)
		{
			auto newState = state->Clone();
			newState->animation.SetTarget(this);
			mStates.Add(newState);
		}

		UpdateLayersDrawingSequence();
	}

	Widget::~Widget()
	{
		if (mParent)
			mParent->RemoveChild(Ptr<Widget>(this), false);

		for (auto layer : mLayers)
			layer.Release();

		for (auto state : mStates)
			state.Release();

		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			child.Release();
		}
	}

	Widget& Widget::operator=(const Widget& other)
	{
		for (auto layer : mLayers)
			layer.Release();

		for (auto state : mStates)
			state.Release();

		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			child.Release();
		}

		mName = other.mName;
		layout = other.layout;

		for (auto layer : other.mLayers)
			mLayers.Add(mnew WidgetLayer(*layer));

		for (auto child : other.mChilds)
		{
			auto cloned = child->Clone();
			mChilds.Add(cloned);
			cloned->mParent = this;
		}

		for (auto state : other.mStates)
			mStates.Add(state->Clone());

		UpdateLayoutRecursive();

		return *this;
	}

	void Widget::Update(float dt)
	{
		for (auto layer : mLayers)
			layer->Update(dt);

		for (auto state : mStates)
			state->Update(dt);

		for (auto child : mChilds)
			child->Update(dt);
	}

	void Widget::Draw()
	{
		for (auto layer : mDrawingLayers)
			layer->Draw();

		for (auto child : mChilds)
			child->Draw();

		static int colr = 0;
		static int lastFrame = 0;

		if (lastFrame != o2Time.GetCurrentFrame())
			colr = 0;
		lastFrame = o2Time.GetCurrentFrame();

		o2Render.DrawRectFrame(layout.mAbsoluteRect, Color4::SomeColor(colr++));
	}

	void Widget::SetName(const String& name)
	{
		mName = name;
	}

	String Widget::GetName() const
	{
		return mName;
	}

	Ptr<Widget> Widget::GetParent() const
	{
		return mParent;
	}

	void Widget::SetParent(Ptr<Widget> parent)
	{
		if (parent->mChilds.Contains(this))
			return;

		if (mParent)
		{
			mParent->RemoveChild(Ptr<Widget>(this), false);
			mParent->layout.Recalculate();
		}

		mParent = parent;

		if (mParent)
		{
			mParent->mChilds.Add(this);
			mParent->layout.Recalculate();
		}
	}

	Ptr<Widget> Widget::AddChild(Ptr<Widget> widget)
	{
		if (mChilds.Contains(widget))
			return widget;

		if (widget->mParent)
			widget->mParent->RemoveChild(widget);

		mChilds.Add(widget);
		widget->mParent = this;

		layout.Recalculate();

		OnChildAdded(widget);

		return widget;
	}

	bool Widget::RemoveChild(const String& path)
	{
		auto child = GetChild(path);
		if (!child)
			return false;

		bool res = child->mParent->RemoveChild(child);
		child->mParent->layout.Recalculate();

		return res;
	}

	bool Widget::RemoveChild(Ptr<Widget> widget, bool release /*= true*/)
	{
		if (!mChilds.Contains(widget))
			return false;

		widget->mParent = nullptr;
		mChilds.Remove(widget);
		layout.Recalculate();

		OnChildRemoved(widget);

		if (release)
			widget.Release();

		return true;
	}

	Ptr<Widget> Widget::GetChild(const String& path)
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		if (pathPart == "..")
		{
			if (mParent)
			{
				if (delPos == -1)
					return mParent;
				else
					return mParent->GetChild(path.SubStr(delPos + 1));
			}

			return nullptr;
		}

		for (auto child : mChilds)
		{
			if (child->mName == pathPart)
			{
				if (delPos == -1)
					return child;
				else
					return child->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	void Widget::RemoveAllChilds()
	{
		for (auto child : mChilds)
		{
			child->mParent = nullptr;
			OnChildRemoved(child);
			child.Release();
		}

		mChilds.Clear();
		layout.Recalculate();
	}

	const Widget::WidgetsVec& Widget::GetChilds() const
	{
		return mChilds;
	}

	Ptr<WidgetLayer> Widget::AddLayer(Ptr<WidgetLayer> layer)
	{
		mLayers.Add(layer);
		layer->mOwnerWidget = this;
		UpdateLayersDrawingSequence();
		OnLayerAdded(layer);
		return layer;
	}

	Ptr<WidgetLayer> Widget::AddLayer(const String& name, Ptr<IRectDrawable> drawable,
									  const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		if (Math::Equals(depth, 0.0f))
			depth = (float)mLayers.Count();

		Ptr<WidgetLayer> layer = mnew WidgetLayer();
		layer->depth = depth;
		layer->name = name;
		layer->drawable = drawable;
		layer->layout = layout;

		AddLayer(layer);

		return layer;
	}

	Ptr<WidgetLayer> Widget::AddSpriteLayer(const String& name, const String& fileName, 
											const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		return AddLayer(name, mnew Sprite(fileName), layout, depth);
	}

	Ptr<WidgetLayer> Widget::AddSpriteLayer(const String& name, AssetId assetId, 
											const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		return AddLayer(name, mnew Sprite(assetId), layout, depth);
	}

	Ptr<WidgetLayer> Widget::AddSpriteLayer(const String& name, Ptr<ImageAsset> asset,
											const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		return AddLayer(name, mnew Sprite(asset), layout, depth);
	}

	Ptr<WidgetLayer> Widget::AddTextLayer(const String& name, const String& text, const String& fontFileName, 
										  Text::HorAlign horAlign /*= Text::HorAlign::Middle*/, 
										  Text::VerAlign verAlign /*= Text::VerAlign::Middle*/, 
										  const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		Ptr<Text> textDrawable = mnew Text(fontFileName);
		textDrawable->SetHorAlign(horAlign);
		textDrawable->SetVerAlign(verAlign);
		textDrawable->SetCText(text);
		return AddLayer(name, textDrawable.Cast<IRectDrawable>(), layout, depth);
	}

	Ptr<WidgetLayer> Widget::AddTextLayer(const String& name, const String& text, AssetId fontAssetId, 
										  Text::HorAlign horAlign /*= Text::HorAlign::Middle*/, 
										  Text::VerAlign verAlign /*= Text::VerAlign::Middle*/, 
										  const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		Ptr<Text> textDrawable = mnew Text(fontAssetId);
		textDrawable->SetHorAlign(horAlign);
		textDrawable->SetVerAlign(verAlign);
		textDrawable->SetCText(text);
		return AddLayer(name, textDrawable.Cast<IRectDrawable>(), layout, depth);
	}

	Ptr<WidgetLayer> Widget::AddTextLayer(const String& name, const String& text, Ptr<VectorFontAsset> fontAsset,
										  Text::HorAlign horAlign /*= Text::HorAlign::Middle*/, 
										  Text::VerAlign verAlign /*= Text::VerAlign::Middle*/,
										  const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		Ptr<Text> textDrawable = mnew Text(fontAsset);
		textDrawable->SetHorAlign(horAlign);
		textDrawable->SetVerAlign(verAlign);
		textDrawable->SetCText(text);
		return AddLayer(name, textDrawable.Cast<IRectDrawable>(), layout, depth);
	}

	Ptr<WidgetLayer> Widget::AddTextLayer(const String& name, const String& text, Ptr<BitmapFontAsset> fontAsset,
										  Text::HorAlign horAlign /*= Text::HorAlign::Middle*/, 
										  Text::VerAlign verAlign /*= Text::VerAlign::Middle*/, 
										  const Layout& layout /*= Layout::Both()*/, float depth /*= 0.0f*/)
	{
		Ptr<Text> textDrawable = mnew Text(fontAsset);
		textDrawable->SetHorAlign(horAlign);
		textDrawable->SetVerAlign(verAlign);
		textDrawable->SetCText(text);
		return AddLayer(name, textDrawable.Cast<IRectDrawable>(), layout, depth);
	}

	Ptr<WidgetLayer> Widget::GetLayer(const String& path) const
	{
		int delPos = path.Find("/");
		WString pathPart = path.SubStr(0, delPos);

		for (auto layer : mLayers)
		{
			if (layer->name == pathPart)
			{
				if (delPos == -1)
					return layer;
				else
					return layer->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	bool Widget::RemoveLayer(Ptr<WidgetLayer> layer)
	{
		bool res = mLayers.Remove(layer);
		layer.Release();
		UpdateLayersDrawingSequence();
		return res;
	}

	bool Widget::RemoveLayer(const String& path)
	{
		auto layer = GetLayer(path);

		if (!layer)
			return false;

		if (layer->GetParent())
			return layer->GetParent()->RemoveChild(layer);

		bool res =  mLayers.Remove(layer);
		UpdateLayersDrawingSequence();
		return res;
	}

	void Widget::RemoveAllLayers()
	{
		for (auto layer : mLayers)
			layer.Release();

		mDrawingLayers.Clear();

		mLayers.Clear();
	}

	const LayersVec& Widget::GetLayers() const
	{
		return mLayers;
	}

	Ptr<WidgetState> Widget::AddState(const String& name)
	{
		Ptr<WidgetState> newState = mnew WidgetState();
		newState->name = name;
		newState->animation.SetTarget(this);
		mStates.Add(newState);
		return newState;
	}

	Ptr<WidgetState> Widget::AddState(const String& name, const Animation& animation)
	{
		Ptr<WidgetState> newState = mnew WidgetState();
		newState->name = name;
		newState->animation = animation;
		newState->animation.SetTarget(this);
		newState->animation.relTime = 0.0f;
		mStates.Add(newState);
		return newState;
	}

	Ptr<WidgetState> Widget::AddState(Ptr<WidgetState> state)
	{
		mStates.Add(state);
		return state;
	}

	bool Widget::RemoveState(const String& name)
	{
		int idx = mStates.FindIdx([&](const Ptr<WidgetState>& state) { return state->name == name; });
		if (idx < 0)
			return false;

		mStates[idx].Release();
		mStates.RemoveAt(idx);

		return true;
	}

	bool Widget::RemoveState(Ptr<WidgetState> state)
	{
		int idx = mStates.Find(state);
		if (idx < 0)
			return false;

		mStates[idx].Release();
		mStates.RemoveAt(idx);

		return true;
	}

	void Widget::RemoveAllStates()
	{
		for (auto state : mStates)
			state.Release();

		mStates.Clear();
	}

	void Widget::SetState(const String& name, bool state)
	{
		auto stateObj = GetStateObject(name);

		if (stateObj)
			stateObj->SetState(state);
	}

	bool Widget::GetState(const String& name) const
	{
		auto state = GetStateObject(name);

		if (state)
			return state->GetState();

		return false;
	}

	Ptr<WidgetState> Widget::GetStateObject(const String& name) const
	{
		return mStates.FindMatch([&](auto state) { return state->name == name; });
	}

	const StatesVec& Widget::GetStates() const
	{
		return mStates;
	}

	void Widget::UpdateLayoutRecursive()
	{
		for (auto child : mChilds)
			child->layout.Recalculate();
	}

	void Widget::OnLayoutUpdated()
	{}

	void Widget::UpdateLayersLayouts()
	{
		for (auto layer : mLayers)
			layer->UpdateLayout();
	}

	void Widget::UpdateLayersDrawingSequence()
	{
		mDrawingLayers.Clear();
		for (auto layer : mLayers)
		{
			mDrawingLayers.Add(layer);
			mDrawingLayers.Add(layer->GetAllChilds());
		}

		mDrawingLayers.Sort([](auto a, auto b) { return a->mDepth < b->mDepth; });
	}

	Widget::WidgetsVec Widget::GetChildsNonConst()
	{
		return mChilds;
	}

	LayersVec Widget::GetLayersNonConst()
	{
		return mLayers;
	}

	StatesVec Widget::GetStatesNonConst()
	{
		return mStates;
	}

	Dictionary<String, Ptr<WidgetLayer>> Widget::GetAllLayers()
	{
		Dictionary<String, Ptr<WidgetLayer>> res;
		for (auto layer : mLayers)
			res.Add(layer->name, layer);

		return res;
	}

	void Widget::OnLayerAdded(Ptr<WidgetLayer> layer)
	{}

	void Widget::OnChildAdded(Ptr<Widget> child)
	{}

	void Widget::OnChildRemoved(Ptr<Widget> child)
	{}

	void Widget::OnDeserialized(const DataNode& node)
	{
		for (auto layer : mLayers)
			layer->mOwnerWidget = this;

		for (auto state : mStates)
			state->animation.SetTarget(this);

		UpdateLayersDrawingSequence();
	}

	void Widget::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Widget, name, SetName, GetName);
		INITIALIZE_PROPERTY(Widget, parent, SetParent, GetParent);
		INITIALIZE_GETTER(Widget, childs, GetChildsNonConst);
		INITIALIZE_GETTER(Widget, layers, GetLayersNonConst);
		INITIALIZE_GETTER(Widget, states, GetStatesNonConst);
		INITIALIZE_ACCESSOR(Widget, child, GetChild);
		INITIALIZE_ACCESSOR(Widget, layer, GetLayer);
		INITIALIZE_ACCESSOR(Widget, state, GetStateObject);

		layer.SetAllAccessFunc(this, &Widget::GetAllLayers);
	}
}