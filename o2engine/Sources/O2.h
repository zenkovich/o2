#pragma once

#include "EngineSettings.h"

#include "Utils/Memory/MemoryManager.h"
#include "Utils/Memory/Ptr.h"

#include "Utils/Math/Math.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Color.h"

#include "Utils/Containers/Vector.h"
#include "Utils/Containers/Dictionary.h"

#include "Utils/Data/DataDoc.h"
#include "Utils/Serialization.h"

#include "Utils/Log/LogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/ConsoleLogStream.h"

#include "Utils/FileSystem/File.h"
#include "Utils/FileSystem/FileInfo.h"
#include "Utils/FileSystem/FileSystem.h"

#include "Utils/Assert.h"
#include "Utils/CommonTypes.h"
#include "Utils/Debug.h"
#include "Utils/Delegates.h"
#include "Utils/Property.h"
#include "Utils/Singleton.h"
#include "Utils/SmartPointers.h"
#include "Utils/String.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"

#include "Application/Input.h"
#include "Application/Application.h"

#define INITIALIZE_O2 \
	MemoryManager* MemoryManager::mInstance = new MemoryManager(); \
	template<> Ptr<Debug> Singleton<Debug>::mInstance = mnew Debug(); \
	Dictionary<String, ISerializable*> SerializableTypesSamples::mObjectSamples