#pragma once

/* !!! More information about Render System interface see in render_system_base_interface.h */

#include "engine_settings.h"

#if defined(RENDER_OGL)
#include "ogl/render_system_ogl.h"
#elif defined(RENDER_D3D9C)
#include "d3d9c\render_system_d3d9c.h"
#endif

#include "app/application.h"

OPEN_O2_NAMESPACE

//inline FontManager* FontsManager() { return AppRender()->GetFontManager(); }

CLOSE_O2_NAMESPACE