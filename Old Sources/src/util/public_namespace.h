#pragma once

#include "engine_settings.h"

/* Engine namespace macros. */
#ifdef ENGINE_NAMESPACE_NAME

/** Opening engine namespace macros. */
#	define OPEN_O2_NAMESPACE namespace ENGINE_NAMESPACE_NAME {

/** Closing engine namespace macros.  */
#	define CLOSE_O2_NAMESPACE }

#else

/** Opening engine namespace macros. */
#	define OPEN_O2_NAMESPACE

/** Closing engine namespace macros.  */
#	define CLOSE_O2_NAMESPACE

#endif //ENGINE_NAMESPACE_NAME
