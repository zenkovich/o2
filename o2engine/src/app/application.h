#pragma once

/* !!! More information about Application interface see in application_base_interface.h */

#include "engine_settings.h"

#if defined(PLATFORM_WIN)
#include "win/application_win.h"
#endif

OPEN_O2_NAMESPACE

inline Application* App() { return dynamic_cast<Application*>(ApplicationBaseInterface::InstancePtr()); }

CLOSE_O2_NAMESPACE