#pragma once

#include "O2.h"
using namespace o2;

// -------------------------
// Application configuration
// -------------------------
class ApplicationConfig: public ISerializable, public Singleton<ApplicationConfig>
{
public:
	// Default constructor. Loads data and applies to application 
	ApplicationConfig();

	// Destructor. Saves application configuration
	~ApplicationConfig();

	SERIALIZABLE_IMPL(ApplicationConfig);

	IOBJECT(ApplicationConfig)
	{
		SRLZ_FIELD(mWindowSize);
		SRLZ_FIELD(mWindowPosition);
		SRLZ_FIELD(mFullScreen);
	}

protected:
	Vec2I mWindowSize;     // Application window size
	Vec2I mWindowPosition; // Application window position
	bool  mFullScreen;     // Is application window is fullscreen
};