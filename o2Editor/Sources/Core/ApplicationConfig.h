#pragma once

#include "Utils/Math/Vector2.h"
#include "Utils/Serialization.h"
#include "Utils/Singleton.h"

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

	SERIALIZABLE(ApplicationConfig);

protected:
	Vec2I mWindowSize;     // Application window size @SERIALIZABLE
	Vec2I mWindowPosition; // Application window position @SERIALIZABLE
	bool  mFullScreen;     // Is application window is fullscreen @SERIALIZABLE
};