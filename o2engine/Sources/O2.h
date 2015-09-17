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
#include "Utils/Bitmap.h"
#include "Utils/CommonTypes.h"
#include "Utils/Debug.h"
#include "Utils/Delegates.h"
#include "Utils/IObject.h"
#include "Utils/Property.h"
#include "Utils/Singleton.h"
#include "Utils/SmartPointers.h"
#include "Utils/StackTrace.h"
#include "Utils/String.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"

#include "Utils/Reflection/Types.h"

#include "Application/Input.h"
#include "Application/BaseApplication.h"

#include "Assets/Asset.h"
#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Assets/BinaryAsset.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"

#include "Render/Render.h"
#include "Render/Camera.h"
#include "Render/Mesh.h"
#include "Render/Sprite.h"
#include "Render/TextureRef.h"
#include "Render/Texture.h"
#include "Render/Font.h"
#include "Render/Text.h"

#define INITIALIZE_O2                                                              					  \
o2::o2StackWalker* o2::o2StackWalker::mInstance = new o2StackWalker();								  \
o2::MemoryManager* o2::MemoryManager::mInstance = new MemoryManager();								  \
template<> o2::Ptr<o2::Debug> o2::Singleton<o2::Debug>::mInstance = mnew o2::Debug();  \
o2::Vector<o2::Type*> o2::Types::mTypes


