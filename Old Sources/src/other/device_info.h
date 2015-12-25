#pragma once

#include "public.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class Application;

/** Device info. Containing different parameters about device, render etc. */
class DeviceInfo: public Singleton<DeviceInfo>
{
public:
	enum DeviceParam {
		DP_RENDER_TARGET_AVAILABLE = 0,
		DP_RENDER_MAX_TEXTURE_SIZE,
		DP_DEVICE_ID,

		DEVICE_PARAMETRES_COUNT
	};

	enum { nParamDataSize = 128 };

protected:
	char mDeviceParametres[DEVICE_PARAMETRES_COUNT][nParamDataSize]; /**< Parameters data. */

public:
	/** ctor. */
	DeviceInfo();

	/** dtor. */
	~DeviceInfo();

	/** Getting parameters from application systems. */
	void Initialize(Application* app);

	/** Returns parameter with specified type cast. */
	template<typename T>
	T GetParameter(DeviceParam paramId);
};

#define deviceInfo() DeviceInfo::InstancePtr()

CLOSE_O2_NAMESPACE