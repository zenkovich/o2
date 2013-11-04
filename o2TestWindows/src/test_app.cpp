#include "test_app.h"

#include "render_system/mesh.h"
#include "render_system/render_system.h"
#include "render_system/sprite.h"
#include "render_system/texture.h"
#include "render_system/render_target.h"
#include "render_system/camera.h"
#include "util/file_system/file_system.h"
#include "util/log.h"
#include "util/math/color.h"
#include "util/math/vector2.h"
#include "util/math/basis.h"
#include "util/graphics/stretch_rect.h"

#include "man_field.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(TestApp);

TestApp::TestApp():
	cApplication(), mManField(NULL)
{
	setOption(cApplicationOption::WND_SIZE, vec2i(800, 600));
	setOption(cApplicationOption::WND_CAPTION, (std::string)"Man field test");
	setOption(cApplicationOption::RESIZIBLE, false);

	getFileSystem().setResourcePath("../data/");
}

TestApp::~TestApp()
{
	safe_release(mManField);
}

void TestApp::onInitialized()
{
	mManField = mnew cManField(this);

	grTexture* testUITexture = mRenderSystem->createTexture("ui_test");
	mStretchRect = mnew cStretchRect(mRenderSystem, 9, testUITexture);

	mStretchRect->addPart(vec2f(0, 0), vec2f(-7, -7), vec2f(0, 0), vec2f(18, 18), fRect(0, 0, 26, 26)); //lt corner
	mStretchRect->addPart(vec2f(0, 0), vec2f(18, -7), vec2f(1, 0), vec2f(-18, 18), fRect(26, 0, 230, 26)); //top side
	mStretchRect->addPart(vec2f(1, 0), vec2f(-18, -7), vec2f(1, 0), vec2f(7, 18), fRect(230, 0, 256, 26)); //rt corner
	
	mStretchRect->addPart(vec2f(0, 0), vec2f(-7, 18), vec2f(0, 1), vec2f(18, -18), fRect(0, 26, 26, 230)); //left side
	mStretchRect->addPart(vec2f(0, 0), vec2f(18, 18), vec2f(1, 1), vec2f(-18, -18), fRect(26, 26, 230, 230)); //center
	mStretchRect->addPart(vec2f(1, 0), vec2f(-18, 18), vec2f(1, 1), vec2f(7, -18), fRect(230, 26, 256, 230)); //right side
	
	mStretchRect->addPart(vec2f(0, 1), vec2f(-7, -18), vec2f(0, 1), vec2f(18, 7), fRect(0, 230, 26, 256)); //lb corner
	mStretchRect->addPart(vec2f(0, 1), vec2f(18, -18), vec2f(1, 1), vec2f(-18, 7), fRect(26, 230, 230, 256)); //bottom side
	mStretchRect->addPart(vec2f(1, 1), vec2f(-18, -18), vec2f(1, 1), vec2f(7, 7), fRect(230, 230, 256, 256)); //rb corner

	mStretchRect->setRect(fRect(20, 30, 140, 160));

	mDragRect = mResizeRect = false;
}

void TestApp::onUpdate( float dt )
{
	//mManField->update(dt);

	if (mInputMessage.isKeyPressed(VK_LBUTTON))
	{
		vec2f cursorPos = mInputMessage.getCursorPos();
		if ((cursorPos - mStretchRect->getPosition()).len() < 17.0f)
		{
			mDragRect = true;
		}
		else if ((cursorPos - mStretchRect->getRect().getrdCorner()).len() < 17.0f)
		{
			mResizeRect = true;
		}
	}

	if (mInputMessage.isKeyReleased(VK_LBUTTON))
	{
		mDragRect = mResizeRect = false;
	}

	if (mDragRect)
		mStretchRect->setPosition(mStretchRect->getPosition() + mInputMessage.getCursorPos() - mLastCursorPos);

	if (mResizeRect)
		mStretchRect->setSize(mStretchRect->getSize() + mInputMessage.getCursorPos() - mLastCursorPos);

	mLastCursorPos = mInputMessage.getCursorPos();
}

void TestApp::processMessage( cApplacationMessage::type message )
{
	cApplication::processMessage(message);

	if (message == cApplacationMessage::ON_STARTED)
	{
		onInitialized();
	}
	else if (message == cApplacationMessage::ON_SIZING)
	{
		draw();
	}
}

void TestApp::onDraw()
{
	mRenderSystem->clear(color4(0, 100, 0, 255));

	mManField->draw();

	mStretchRect->draw();
}

CLOSE_O2_NAMESPACE
