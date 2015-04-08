#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

UIDrawablesListWidget::UIDrawablesListWidget(const Layout& layout, const String& id /*= ""*/):
UIWidget(layout, id), mBasicDrawable("basic", NULL, Layout::Both())
{
	AddChildPropertyList(dynamic_cast<PropertyList*>(&mBasicDrawable));
}

UIDrawablesListWidget::UIDrawablesListWidget(const UIDrawablesListWidget& widget):
UIWidget(widget), mBasicDrawable(widget.mBasicDrawable)
{
	AddChildPropertyList(dynamic_cast<PropertyList*>(&mBasicDrawable));
	FOREACH(StatesMap, mStates, state)
		(*state).second->SetOwnerWidget(this);
}

UIDrawablesListWidget::~UIDrawablesListWidget()
{
	RemoveAllDrawables();
}

UIWidget* UIDrawablesListWidget::Clone() const
{
	return mnew UIDrawablesListWidget(*this);
}

UIDrawablesListWidget::Drawable* UIDrawablesListWidget::AddDrawable(IRectDrawable* drawable,
																	const String& id,
																	const Layout& layout /*= cLayout::both()*/)
{
	return AddDrawable(mnew Drawable(id, drawable, layout, &mBasicDrawable));
}

UIDrawablesListWidget::Drawable* UIDrawablesListWidget::AddDrawable(Drawable* drawable)
{
	mBasicDrawable.AddChildDrawable(drawable);
	AddedDrawable(drawable);
	return drawable;
}

UIDrawablesListWidget::Drawable* UIDrawablesListWidget::GetDrawable(const String& id)
{
	return mBasicDrawable.GetChildDrawable(id);
}

void UIDrawablesListWidget::RemoveDrawable(const String& id, bool release /*= true*/)
{
	Drawable* remDrw = GetDrawable(id);
	if (remDrw)
		mBasicDrawable.RemoveChildDrawable(remDrw, release);
}

void UIDrawablesListWidget::RemoveAllDrawables()
{
	mBasicDrawable.RemoveAllChildDrawables();
}

void UIDrawablesListWidget::LayoutUpdated()
{
	mBasicDrawable.UpdateLayoutManual(mGlobalPosition, mSize);
}

void UIDrawablesListWidget::LocalDraw()
{
	mBasicDrawable.Draw();
}


UIDrawablesListWidget::Drawable::Drawable(const String& name, IRectDrawable* drawable,
										  const Layout& layout /*= Layout::Both()*/,
										  Drawable* parentDrawable /*= NULL*/)
{
	mName = name;
	mDrawable = drawable;
	mLayout = layout;
	mParentDrawable = parentDrawable;

	SetPropertyListName(mName);

	if (mDrawable)
	{
		mDrawable->SetPropertyListName("drawable");
		AddChildPropertyList((dynamic_cast<PropertyList*>(mDrawable)));
	}

	InitializeProperties();
}

UIDrawablesListWidget::Drawable::Drawable(const Drawable& drawable)
{
	if (drawable.mDrawable)
		mDrawable = drawable.mDrawable->Clone();
	else
		mDrawable = NULL;

	mName = drawable.mName;
	mLayout = drawable.mLayout;
	mParentDrawable = NULL;

	SetPropertyListName(mName);

	if (mDrawable)
	{
		mDrawable->SetPropertyListName("drawable");
		AddChildPropertyList((dynamic_cast<PropertyList*>(mDrawable)));
	}

	FOREACH_CONST(DrawablesArr, drawable.mChildDrawables, drw)
		AddChildDrawable(mnew Drawable(**drw));

	InitializeProperties();
}

UIDrawablesListWidget::Drawable::~Drawable()
{
	RemoveAllChildDrawables();
}

UIDrawablesListWidget::Drawable& UIDrawablesListWidget::Drawable::operator=(const Drawable& drw)
{
	RemoveAllChildDrawables();
	SafeRelease(mDrawable);
	mDrawable = NULL;

	if (drw.mDrawable)
		mDrawable = drw.mDrawable->Clone();
	else
		mDrawable = NULL;

	mName = drw.mName;
	mLayout = drw.mLayout;
	mParentDrawable = NULL;

	SetPropertyListName(mName);

	if (mDrawable)
	{
		mDrawable->SetPropertyListName("drawable");
		AddChildPropertyList((dynamic_cast<PropertyList*>(mDrawable)));
	}

	FOREACH_CONST(DrawablesArr, drw.mChildDrawables, drwi)
		AddChildDrawable(mnew Drawable(**drwi));

	return *this;
}

UIDrawablesListWidget::Drawable* UIDrawablesListWidget::Drawable::AddChildDrawable(Drawable* drawable)
{
	mChildDrawables.push_back(drawable);
	drawable->mParentDrawable = (this);
	AddChildPropertyList((dynamic_cast<PropertyList*>(drawable)));
	return drawable;
}

UIDrawablesListWidget::Drawable* UIDrawablesListWidget::Drawable::AddChildDrawable(const String& name,
																				   IRectDrawable* drawable, 
																				   const Layout& layout /*= Layout::Both()*/)
{
	return AddChildDrawable(mnew Drawable(name, drawable, layout));
}

UIDrawablesListWidget::Drawable* UIDrawablesListWidget::Drawable::GetChildDrawable(const String& path)
{
	int delPos = path.find("/");
	String pathPart = path.substr(0, delPos);

	FOREACH(DrawablesArr, mChildDrawables, it)
	{
		if ((*it)->mName == pathPart)
		{
			if (delPos == path.npos)
				return (*it);
			else
				return (*it)->getChildDrawable(path.substr(delPos + 1));
		}
	}

	return NULL;
}

void UIDrawablesListWidget::Drawable::RemoveChildDrawable(Drawable* drawable, bool release /*= true*/)
{
	DrawablesArr::iterator fnd = FIND(mChildDrawables, drawable);
	if (fnd == mChildDrawables.end())
		return;

	RemoveChildPropertyList((dynamic_cast<PropertyList*>(drawable)), false);
	if (release)
		SafeRelease(drawable);

	mChildDrawables.erase(fnd);
}

void UIDrawablesListWidget::Drawable::RemoveAllChildDrawables()
{
	FOREACH(DrawablesArr, mChildDrawables, drw)
		SafeRelease(*drw);

	RemoveAllChildPropertyLists();

	mChildDrawables.clear();
}

void UIDrawablesListWidget::Drawable::UpdateLayoutManual(const Vec2F& parPos, const Vec2F& parSize)
{
	mLayout.update(parPos, parSize);
	LayoutUpdated();
}

void UIDrawablesListWidget::Drawable::LayoutUpdated()
{
	if (mDrawable)
		mDrawable->GetRect(mLayout.getRect());

	FOREACH(DrawablesArr, mChildDrawables, drw)
		(*drw)->updateLayoutManual(mLayout.mPosition, mLayout.mSize);
}

void UIDrawablesListWidget::Drawable::UpdateLayout()
{
	UpdateLayoutManual(mParentDrawable->mLayout.mPosition, mParentDrawable->mLayout.mSize);
}

String UIDrawablesListWidget::Drawable::GetName() const
{
	return mName;
}

void UIDrawablesListWidget::Drawable::SetDrawable(IRectDrawable* drawable)
{
	if (mDrawable)
	{
		RemoveChildPropertyList((dynamic_cast<PropertyList*>(mDrawable)));
		SafeRelease(mDrawable);
	}

	mDrawable = drawable;

	if (mDrawable)
	{
		mDrawable->SetPropertyListName("drawable");
		AddChildPropertyList((dynamic_cast<PropertyList*>(mDrawable)));
	}
}

IRectDrawable* UIDrawablesListWidget::Drawable::GetDrawable()
{
	return mDrawable;
}

void UIDrawablesListWidget::Drawable::SetLayout(const Layout& layout)
{
	mLayout = layout;
}

Layout UIDrawablesListWidget::Drawable::GetLayout() const
{
	return mLayout;
}

void UIDrawablesListWidget::Drawable::InitializeProperties()
{
	REG_PROPERTY(Drawable, layout, SetLayout, GetLayout);
}

void UIDrawablesListWidget::Drawable::Draw()
{
	if (mDrawable)
		mDrawable->Draw();

	FOREACH(DrawablesArr, mChildDrawables, drw)
		(*drw)->Draw();
}

String UIDrawablesListWidget::Drawable::GetPathId() const
{
	String res = mName;
	Drawable* parent = mParentDrawable;
	while (parent && parent->mParentDrawable)
	{
		res = parent->mName + "/" + res;
		parent = parent->mParentDrawable;
	}

	return res;
}

CLOSE_O2_NAMESPACE