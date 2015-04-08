#pragma once

#include "log/log_stream.h"

OPEN_O2_NAMESPACE

/** Global log stream. Use it for putting data in log. */
extern LogStream* gLog;


/** Puts data into global log with low level. */
#define LowLog(str, ...) gLog->Out(str, __VA_ARGS__);

/** Puts data into global log with hight level. */
#define HightLog(str, ...) gLog->HOut(str, __VA_ARGS__);

/** Puts error message into global log. */
#define LogError(str, ...) gLog->Error(str, __VA_ARGS__);

/** Puts warning message into global log. */
#define LogWarning(str, ...) gLog->Warning(str, __VA_ARGS__);

CLOSE_O2_NAMESPACE