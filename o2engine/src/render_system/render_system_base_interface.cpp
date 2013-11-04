#include "render_system_base_interface.h"

#include <algorithm>

#include "util/log/file_log_stream.h"
#include "util/log.h"
#include "app/application.h"
#include "texture.h"
#include "texture.h"

OPEN_O2_NAMESPACE

grRenderSystemBaseInterface::grRenderSystemBaseInterface( cApplication* application ):
	mCurrentCamera(NULL), mApplication(application)
{
	mLog = new cFileLogStream("Render", gLog->getLevel(), "render_log.txt");
	gLog->bindStream(mLog);
}

grRenderSystemBaseInterface::~grRenderSystemBaseInterface()
{
	removeAllTextures();
	gLog->unbindStream(mLog);
}

bool grRenderSystemBaseInterface::bindCamera( grCamera* camera )
{
	mCurrentCamera = camera;
	updateCameraTransforms();

	return true;
}

grTexture* grRenderSystemBaseInterface::addTexture( grTexture* texture )
{
	mTextures.push_back(texture);
	return texture;
}

grTexture* grRenderSystemBaseInterface::createTexture( const std::string& fileName )
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
	{
		if (fileName == (*it)->getFileName())
		{
			(*it)->incRefCount();
			return *it;
		}
	}

	grTexture* newTexture = new grTexture((grRenderSystem*)this, fileName);
	//newTexture->incRefCount();

	mLog->hout("Created texture '%s'", fileName.c_str());

	return addTexture(newTexture);
}

bool grRenderSystemBaseInterface::removeTexture( grTexture* texture )
{
	texture->decrRefCount();

	if (texture->getRefCount() == 0)
	{
		bool inVec = false;

		TexturesVec::iterator fnd = std::find(mTextures.begin(), mTextures.end(), texture);
		if (fnd != mTextures.end())
		{
			mTextures.erase(fnd);
			inVec = true;
		}

		safe_release(texture);

		return inVec;
	}

	return true;
}

bool grRenderSystemBaseInterface::removeAllTextures()
{
	for (TexturesVec::iterator it = mTextures.begin(); it != mTextures.end(); ++it)
		safe_release(*it);

	mTextures.clear();

	return true;
}

vec2i grRenderSystemBaseInterface::getResolution() const
{
	return mResolution;
}

CLOSE_O2_NAMESPACE