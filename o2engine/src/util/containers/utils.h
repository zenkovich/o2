#pragma once

#include "util/public_namespace.h"
#include "array.h"

OPEN_O2_NAMESPACE

#define foreach(_enumerable, _iterator) \
for (auto _iterator = (_enumerable).Begin(); (_iterator).Index() < (_enumerable).Count(); ++(_iterator))

#define foreach_back(_enumerable, _iterator) \
for (auto _iterator = (_enumerable).End(); (_iterator).Index() >= 0; --(_iterator))

#define foreach_const(_enumerable, _iterator) \
for (auto _iterator = (_enumerable).Begin(); (_iterator).Index() < (_enumerable).Count(); ++(_iterator))

#define foreach_rem(_enumerable, _iterator) \
for (auto _iterator = _enumerable.Begin(); (_iterator).Index() < (_enumerable)(_enumerable).Count();)

#define ReleaseArray(_enumerable) \
for (auto _iterator = (_enumerable).Begin(); (_iterator).Index() < (_enumerable).Count(); ++(_iterator)) \
	SafeRelease(*(_iterator)); \
	(_enumerable).Clear()

CLOSE_O2_NAMESPACE
