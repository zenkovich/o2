#pragma once

#include "EngineSettings.h"

void* operator new(size_t size, const char* location, int line);
void operator delete(void* obj, const char* location, int line);

#define mnew new(__FILE__, __LINE__)