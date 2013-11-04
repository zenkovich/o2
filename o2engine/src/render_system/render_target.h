#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

/* !!! More information about Render Target interface see in render_target_base_interface.h */

#include "engine_settings.h"

#if defined(RENDER_OGL)
	#include "ogl/render_target_ogl.h"
#elif defined(RENDER_D3D9C)
	#include "d3d9c/render_target_d3d9c.h"
#endif


#endif //RENDER_TARGET_H