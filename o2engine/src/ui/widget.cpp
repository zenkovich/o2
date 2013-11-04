#include "widget.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiWidget);

uiWidget::uiWidget( const std::string& id /*= ""*/, uiWidget* parent /*= NULL*/, const vec2f& localPos /*= vec2f()*/ ):
	mId(id), mLocalPosition(localPos), mGlobalPosition(localPos), mChildsOffset(), mParent(parent)
{
}

uiWidget::uiWidget( const uiWidget& widget )
{
	mId = widget.mId;
	mLocalPosition = widget.mLocalPosition;
	mGlobalPosition = widget.mLocalPosition.value();
}

uiWidget::~uiWidget()
{

}

void uiWidget::draw()
{

}

void uiWidget::update( float dt )
{

}

void uiWidget::processInputMessage( const cInputMessage& msg )
{

}

void uiWidget::setPosition( const vec2f& position )
{
	mLocalPosition = position;
}

vec2f uiWidget::getPosition() const
{
	return mLocalPosition.value();
}

uiWidget* uiWidget::clone() const
{

}

uiWidget* uiWidget::addChild( uiWidget* widget )
{

}

bool uiWidget::removeChild( uiWidget* widget )
{

}

void uiWidget::removeAllChilds()
{

}

uiWidget* uiWidget::getWidget( const std::string& id )
{
	int delPos = id.find("/");
	std::string pathPart = id.substr(0, delPos);

	if (pathPart == "..")
	{
		if (mParent)
		{
			if (delPos == id.npos)
				return mParent;
			else
				return mParent->getWidget(id.substr(delPos + 1));
		}

		return NULL;
	}

	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
	{
		if (it->mId == pathPart)
		{
			if (delPos == id.npos)
				return &(*it);
			else
				return it->getWidget(id.substr(delPos + 1));
		}
	}

	return NULL;
}

	CLOSE_O2_NAMESPACE
