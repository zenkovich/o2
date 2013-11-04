#include "texture_base_interface.h"

#include "render_system.h"

OPEN_O2_NAMESPACE
	
grTextureBaseInterface::grTextureBaseInterface(grRenderSystem* renderSystem):
	mRenderSystem(renderSystem), mFormat(grTexFormat::DEFAULT), mUsage(grTexUsage::DEFAULT), mRefCount(0)
{
}

grTextureBaseInterface::~grTextureBaseInterface()
{
}

int grTextureBaseInterface::getRefCount() const
{
	return mRefCount;
}

void grTextureBaseInterface::incRefCount()
{
	mRefCount++;
}

void grTextureBaseInterface::decrRefCount()
{
	mRefCount--;
}

const std::string& grTextureBaseInterface::getFileName() const
{
	return mFileName;
}

grTexFormat::type grTextureBaseInterface::getFormat() const
{
	return mFormat;
}

grTexUsage::type grTextureBaseInterface::getUsage() const
{
	return mUsage;
}

vec2f grTextureBaseInterface::getSize() const
{
	return mSize;
}

CLOSE_O2_NAMESPACE