#include "o2/stdafx.h"
#include "SpineManager.h"

#include <spine/Extension.h>

namespace o2
{
	DECLARE_SINGLETON(SpineManager);

	SpineManager::SpineManager(RefCounter* refCounter):
		Singleton(refCounter)
	{}

	void SpineTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
	{
		TextureRef texture(String(path.buffer()));

		void* texturePtr = texture.Get();
		mUsedTextures[texturePtr] = texture;

		page.texture = texturePtr;
	}

	void SpineTextureLoader::unload(void* texture)
	{
		mUsedTextures.Remove(texture);
	}
}

spine::SpineExtension* spine::getDefaultExtension()
{
	return mnew spine::DefaultSpineExtension();
}
