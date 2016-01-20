#pragma once

#include "EngineSettings.h"
#include "Utils\Debug.h"
#include "Utils\FileSystem\FileSystem.h"
#include "Utils\Memory\MemoryManager.h"
#include "Utils\Reflection\Reflection.h"
#include "Utils\StackTrace.h"

#define INITIALIZE_O2                                                              	         \
o2::o2StackWalker* o2::o2StackWalker::instance = new o2StackWalker();				         \
o2::MemoryManager* o2::MemoryManager::instance = new MemoryManager();				         \
template<> o2::Debug* o2::Singleton<o2::Debug>::mInstance = mnew o2::Debug();                \
template<> o2::FileSystem* o2::Singleton<o2::FileSystem>::mInstance = mnew o2::FileSystem(); \
o2::Reflection* o2::Reflection::instance = mnew Reflection()

#define MAIN_O2 \
RegReflectionTypes()


