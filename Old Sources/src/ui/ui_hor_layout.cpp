#include "ui_hor_layout.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIHorLayout);

UIHorLayout::UIHorLayout(const Layout& layout, float widgetsDistance /*= 10.0f*/, const String& id /*= ""*/):
UIWidget(layout, id), mWidgetsDistance(widgetsDistance)
{
}

UIHorLayout::UIHorLayout(const UIHorLayout& widget):
UIWidget(widget)
{
	mWidgetsDistance = widget.mWidgetsDistance;
}

UIHorLayout::~UIHorLayout()
{
}

UIWidget* UIHorLayout::Clone() const
{
	return mnew UIHorLayout(*this);
}

void UIHorLayout::SetWidgetsDistance(float distance)
{
	mWidgetsDistance = distance;
	UpdateLayout();
}

float UIHorLayout::GetWidgetsDistance() const
{
	return mWidgetsDistance;
}

void UIHorLayout::LayoutUpdated()
{
	float childsLayoutSize = mChildsLayout.mSize.x - mWidgetsDistance*(float)(mChildWidgets.size() - 1);
	float childSize = childsLayoutSize/(float)mChildWidgets.size();

	mFlexibleWidgets.clear();
	float offset = 0.0f;
	float addOffset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		Layout lt = (*child)->GetLayout();

		lt.mLTAbsolute.x = offset;             lt.mLTRelative.x = 0.0f;
		lt.mRBAbsolute.x = offset + childSize; lt.mRBRelative.x = 0.0f;

		(*child)->SetLayoutParams(lt, true);
		lt = (*child)->GetLayout();

		offset = lt.Right() - mChildsLayout.left() + mWidgetsDistance;

		float d = childSize - lt.mSize.x;
		addOffset += d;
		if (Equals(d, 0.0f, 1.0f))
			mFlexibleWidgets.push_back(*child);
	}

	addOffset /= (float)mFlexibleWidgets.size();

	offset = 0.0f;
	FOREACH(WidgetsVec, mChildWidgets, child)
	{
		Layout lt = (*child)->GetLayout();

		lt.mLTAbsolute.x = offset;             lt.mLTRelative.x = 0.0f;
		lt.mRBAbsolute.x = offset + childSize; lt.mRBRelative.x = 0.0f;

		if (FIND(mFlexibleWidgets, *child) != mFlexibleWidgets.end())
			lt.mRBAbsolute.x += addOffset;

		(*child)->SetLayoutParams(lt, true);
		lt = (*child)->GetLayout();

		offset = lt.Right() - mChildsLayout.Left() + mWidgetsDistance;
	}
}

UIWidget* UIHorLayout::AddChild(UIWidget* widget, int position /*= -1*/)
{
	UIWidget* res = UIWidget::AddChild(widget, position);
	UpdateLayout(true);
	return res;
}

CLOSE_O2_NAMESPACE