#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <vector>
#include <map>

#include "public.h"
#include "util/singleton.h"
#include "file.h"

OPEN_O2_NAMESPACE
	
/** File system, singleton. Containing basic resource path, extension strings. */
class cFileSystem:public cSingleton<cFileSystem>
{
public:
	typedef std::vector<std::string> ExtensionsVec;

private:
	std::string                               mResourcePath; /**< Basic application resources path. */
	std::map<cFileType::value, ExtensionsVec> mExtensions;   /**< Extensions strings map. */

public:
	cFileSystem();
	~cFileSystem();

	/** Sets resource path. */
	void setResourcePath(const std::string& path);

	/** Returns resource path. */
	const std::string& getResourcePath() const;

	/** Returns vector of extension strings for extension type. */
	const ExtensionsVec& getExtensions(cFileType::value fileType) const;
};

#define getFileSystem() cFileSystem::instance()

CLOSE_O2_NAMESPACE

#endif //FILE_SYSTEM_H