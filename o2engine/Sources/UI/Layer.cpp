#include "Layer.h"

#include "UI/Widget.h"
#include "Render/RectDrawable.h"

namespace o2
{
	Layer::Layer():
		mDepth(0.0f), id((String)Math::Random<UInt>(0, UINT_MAX))
	{}

	Layer::~Layer()
	{}

	void Layer::Draw()
	{
		mDrawable->Draw();
	}

	void Layer::Update(float dt)
	{
		if (mParent)
			mAbsolutePosition = layout.Calculate(mParent->mAbsolutePosition);
		else
			mAbsolutePosition = layout.Calculate(RectF(mOwnerWidget->mAbsolutePosition, 
													   mOwnerWidget->mAbsolutePosition + mOwnerWidget->mSize));

		mDrawable->SetRect(mAbsolutePosition);
	}

	Ptr<Layer> Layer::GetChild(const String& path)
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
			if (child->id == pathPart)
			{
				if (delPos == -1)
					return child;
				else
					return child->GetChild(path.SubStr(delPos + 1));
			}
		}

		return nullptr;
	}

	LayersVec Layer::GetAllChilds() const
	{
		LayersVec res = mChilds;
		for (auto child : mChilds)
			res.Add(child->GetAllChilds());

		return res;
	}

	void Layer::SetDepth(float depth)
	{
		mDepth = depth;
		mOwnerWidget->UpdateLayersDrawingSequence();
	}

	float Layer::GetDepth() const
	{
		return mDepth;
	}

	void Layer::OnChildAdded(Ptr<Layer> child)
	{
		child->mOwnerWidget = mOwnerWidget;
		mOwnerWidget->UpdateLayersDrawingSequence();
	}
}