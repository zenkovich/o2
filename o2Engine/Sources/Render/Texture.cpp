#include "Texture.h"

#include "Utils/Reflection/Reflection.h"

ENUM_META_(o2::Texture::Format, Format)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(R8G8B8);
	ENUM_ENTRY(R8G8B8A8);
}
END_ENUM_META;

ENUM_META_(o2::Texture::Usage, Usage)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(RenderTarget);
}
END_ENUM_META;
