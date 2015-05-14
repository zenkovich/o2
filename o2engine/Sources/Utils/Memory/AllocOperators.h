#pragma once

#include "EngineSettings.h"

void* operator new(size_t size, bool managed, const char* location, int line);
void operator delete(void* obj, bool managed, const char* location, int line);

#define mnew new(false, __FILE__, __LINE__)
#define gcnew new(true, __FILE__, __LINE__)