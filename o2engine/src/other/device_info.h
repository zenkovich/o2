#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include "public.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class cApplication;

/** Device info. Containin different parametres about device, render etc. */
class cDeviceInfo:public cSingleton<cDeviceInfo>
{
public:
	enum DeviceParam { DP_RENDER_TARGET_AVAILABLE = 0,
	                   DP_RENDER_MAX_TEXTURE_SIZE,   
					   DP_DEVICE_ID,

					   DEVICE_PARAMETRES_COUNT };

	enum { nParamDataSize = 128 };

protected:
	char mDeviceParametres[DEVICE_PARAMETRES_COUNT][nParamDataSize]; /**< Parametres data. */

public:
	/** ctor. */
	cDeviceInfo();

	/** dtor. */
	~cDeviceInfo();

	/** Getting parametres from application systems. */
	void initialize(cApplication* app);

	/** Returns parameter with specified type cast. */
	template<typename T>
	T getParameter(DeviceParam paramId);
};

#define deviceInfo() cDeviceInfo::instance()

CLOSE_O2_NAMESPACE

#endif //DEVICE_INFO_H