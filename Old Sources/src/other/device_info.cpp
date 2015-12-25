#include "device_info.h"

#include "app/application.h"
#include "render_system/render_system.h"
#include "util/log/log_stream.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(DeviceInfo);

DeviceInfo::DeviceInfo()
{
}

DeviceInfo::~DeviceInfo()
{
}

template<typename T>
T DeviceInfo::GetParameter(DeviceParam paramId)
{
	return (*(T*)mDeviceParametres[Clamp<int>(paramId, 0, DEVICE_PARAMETRES_COUNT - 1)]);
}

void DeviceInfo::Initialize(Application* app)
{
	const bool logInfo = true;

	//device id
	if (IsWinPlatform())
		strcpy(mDeviceParametres[DP_DEVICE_ID], "PC WIN");

	//render target available
	(*(bool*)&mDeviceParametres[DP_RENDER_TARGET_AVAILABLE]) = app->mRenderSystem->IsRenderTargetAvailable();

	//max texture size
	(*(Vec2I*)&mDeviceParametres[DP_RENDER_MAX_TEXTURE_SIZE]) = app->mRenderSystem->GetMaxTextureSize();

	//output results
	if (logInfo)
	{
		app->mLog->HOut("=== DEVICE INFO ===");

		app->mLog->HOut("Device id: '%s'", mDeviceParametres[DP_DEVICE_ID]);

		app->mLog->HOut("Render targets: %s", GetParameter<bool>(DP_RENDER_TARGET_AVAILABLE) ? "available":"not supporting");

		Vec2I maxTexSize = GetParameter<Vec2I>(DP_RENDER_MAX_TEXTURE_SIZE);
		app->mLog->HOut("Max texture size: %i x %i", maxTexSize.x, maxTexSize.y);

		app->mLog->HOut("===================");
	}
}


CLOSE_O2_NAMESPACE