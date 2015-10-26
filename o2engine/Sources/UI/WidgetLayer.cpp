#include "WidgetLayer.h"

#include "UI/Widget.h"

namespace o2
{
	IOBJECT_CPP(WidgetLayer);

	WidgetLayer::WidgetLayer():
		mDepth(0.0f), name((String)Math::Random<UInt>(0, UINT_MAX))
	{
		InitializeProperties();
	}

	WidgetLayer::WidgetLayer(const WidgetLayer& other):
		mDepth(other.mDepth), name(other.name), layout(other.layout)
	{
		if (other.drawable)
			drawable = other.drawable->Clone();

		InitializeProperties();
	}

	WidgetLayer::~WidgetLayer()
	{
		drawable.Release();
	}

	WidgetLayer& WidgetLayer::operator=(const WidgetLayer& other)
	{
		drawable.Release();
			
		mDepth = other.mDepth;
		name = other.name;
		drawable = other.drawable->Clone();
		if (mOwnerWidget)
			mOwnerWidget->UpdateLayersDrawingSequence();

		return *this;
	}

	void WidgetLayer::Draw()
	{
		drawable->Draw();
	}

	void WidgetLayer::Update(float dt)
	{
	}

	Ptr<WidgetLayer> WidgetLayer::GetChild(const String& path)
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
			if (child->name == pathPart)
			{
				if (delPos == -1)
					return child;
				else
					return child->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	LayersVec WidgetLayer::GetAllChilds() const
	{
		LayersVec res = mChilds;
		for (auto child : mChilds)
		{
			res.Add(child);
			res.Add(child->GetAllChilds());
		}

		return res;
	}

	void WidgetLayer::SetDepth(float depth)
	{
		mDepth = depth;
		if (mOwnerWidget)
			mOwnerWidget->UpdateLayersDrawingSequence();
	}

	float WidgetLayer::GetDepth() const
	{
		return mDepth;
	}

	void WidgetLayer::OnChildAdded(Ptr<WidgetLayer> child)
	{
		child->mOwnerWidget = mOwnerWidget;

		if (mOwnerWidget)
			mOwnerWidget->UpdateLayersDrawingSequence();
	}

	void WidgetLayer::UpdateLayout()
	{
		if (mParent)
			mAbsolutePosition = layout.Calculate(mParent->mAbsolutePosition);
		else
			mAbsolutePosition = layout.Calculate(mOwnerWidget->layout.mAbsoluteRect);

		drawable->SetRect(mAbsolutePosition);
	}

	void WidgetLayer::InitializeProperties()
	{
		INITIALIZE_PROPERTY(WidgetLayer, depth, SetDepth, GetDepth);
	}

}