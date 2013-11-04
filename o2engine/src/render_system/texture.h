#ifndef TEXTURE_H
#define TEXTURE_H

/* !!! More information about grTexture interface see in texture_base_interface.h */

#include "engine_settings.h"

#if defined(RENDER_OGL)
	#include "ogl/texture_ogl.h"
#elif defined(RENDER_D3D9C)
	#include "d3d9c/texture_d3d9c.h"
#endif

#endif //TEXTURE_H