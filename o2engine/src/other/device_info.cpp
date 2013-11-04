#include "device_info.h"

#include "app/application.h"
#include "render_system/render_system.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cDeviceInfo);

cDeviceInfo::cDeviceInfo()
{
}

cDeviceInfo::~cDeviceInfo()
{
}

template<typename T>
T cDeviceInfo::getParameter( DeviceParam paramId )
{
	return (*(T*)mDeviceParametres[clamp<int>(paramId, 0, DEVICE_PARAMETRES_COUNT - 1)]);
}

void cDeviceInfo::initialize( cApplication* app )
{
	const bool logInfo = true;

//device id
	if (isWinPlatform())
		strcpy(mDeviceParametres[DP_DEVICE_ID], "PC WIN");

//render target available
	(*(bool*)&mDeviceParametres[DP_RENDER_TARGET_AVAILABLE]) = app->mRenderSystem->isRenderTargetAvailable();

//max texture size
	(*(vec2i*)&mDeviceParametres[DP_RENDER_MAX_TEXTURE_SIZE]) = app->mRenderSystem->getMaxTextureSize();

//output results
	if (logInfo)
	{
		app->mLog->hout("=== DEVICE INFO ===");
		
		app->mLog->hout("Device id: '%s'", mDeviceParametres[DP_DEVICE_ID]);

		app->mLog->hout("Render targets: %s", getParameter<bool>(DP_RENDER_TARGET_AVAILABLE) ? "available":"not supporting");
		
		vec2i maxTexSize = getParameter<vec2i>(DP_RENDER_MAX_TEXTURE_SIZE);
		app->mLog->hout("Max texture size: %i x %i", maxTexSize.x, maxTexSize.y);
		
		app->mLog->hout("===================");
	}
}


CLOSE_O2_NAMESPACE