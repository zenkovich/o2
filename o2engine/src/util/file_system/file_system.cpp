#include "file_system.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cFileSystem);

cFileSystem::cFileSystem()
{
	mExtensions[cFileType::FT_IMAGE].push_back("png");

	mExtensions[cFileType::FT_CONFIG].push_back("xml");
}

cFileSystem::~cFileSystem()
{
}

void cFileSystem::setResourcePath( const std::string& path )
{
	mResourcePath = path;
}

const cFileSystem::ExtensionsVec& cFileSystem::getExtensions( cFileType::value fileType ) const
{
	return mExtensions.at(fileType);
}

const std::string& cFileSystem::getResourcePath() const
{
	return mResourcePath;
}

CLOSE_O2_NAMESPACE