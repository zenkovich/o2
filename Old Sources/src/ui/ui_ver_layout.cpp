#include "ui_ver_layout.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIVerLayout);

UIVerLayout::UIVerLayout(const Layout& layout, float widgetsDistance /*= 10.0f*/, const String& id /*= ""*/):
UIWidget(layout, id), mWidgetsDistance(widgetsDistance)
{
}

UIVerLayout::UIVerLayout(const UIVerLayout& widget):
UIWidget(widget)
{
	mWidgetsDistance = widget.mWidgetsDistance;
}

UIVerLayout::~UIVerLayout()
{
}

UIWidget* UIVerLayout::Clone() const
{
	return mnew UIVerLayout(*this);
}

void UIVerLayout::SetWidgetsDistance(float distance)
{
	mWidgetsDistance = distance;
	UpdateLayout();
}

float UIVerLayout::GetWidgetsDistance() const
{
	return mWidgetsDistance;
}

void UIVerLayout::LayoutUpdated()
{
	float childsLayoutSize = mChildsLayout.mSize.y - mWidgetsDistance*(float)(mChildWidgets.size() - 1);
	float childSize = childsLayoutSize/(float)mChildWidgets.size();

	mFlexibleWidgets.clear();
	float offset = 0.0f;
	float addOffset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		Layout lt = (*child)->GetLayout();

		lt.mLTAbsolute.y = offset;             lt.mLTRelative.y = 0.0f;
		lt.mRBAbsolute.y = offset + childSize; lt.mRBRelative.y = 0.0f;

		(*child)->SetLayoutParams(lt, true);
		lt = (*child)->GetLayout();

		offset = lt.Down() - mChildsLayout.Top() + mWidgetsDistance;

		float d = childSize - lt.mSize.y;
		addOffset += d;
		if (Equals(d, 0.0f, 1.0f))
			mFlexibleWidgets.push_back(*child);
	}

	addOffset /= (float)mFlexibleWidgets.size();

	offset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		Layout lt = (*child)->GetLayout();

		lt.mLTAbsolute.y = offset;             lt.mLTRelative.y = 0.0f;
		lt.mRBAbsolute.y = offset + childSize; lt.mRBRelative.y = 0.0f;

		if (FIND(mFlexibleWidgets, *child) != mFlexibleWidgets.end())
			lt.mRBAbsolute.y += addOffset;

		(*child)->SetLayoutParams(lt, true);
		lt = (*child)->GetLayout();

		offset = lt.Down() - mChildsLayout.top() + mWidgetsDistance;
	}
}

UIWidget* UIVerLayout::AddChild(UIWidget* widget, int position /*= -1*/)
{
	UIWidget* res = UIWidget::AddChild(widget, position);
	UpdateLayout(true);
	return res;
}

CLOSE_O2_NAMESPACE