#pragma once

#include "EngineSettings.h"

#include "Utils/Memory/MemoryManager.h"
#include "Utils/Memory/Ptr.h"

#include "Utils/Math/Color.h"
#include "Utils/Math/Curve.h"
#include "Utils/Math/Interpolation.h"
#include "Utils/Math/Layout.h"
#include "Utils/Math/Math.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vertex2.h"

#include "Utils/Containers/Dictionary.h"
#include "Utils/Containers/Vector.h"

#include "Utils/Data/DataDoc.h"
#include "Utils/Serialization.h"

#include "Utils/Log/ConsoleLogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/LogStream.h"

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

#include "Utils/Editor/DragHandle.h"

#include "Application/BaseApplication.h"
#include "Application/Input.h"

#include "Assets/Asset.h"
#include "Assets/Assets.h"
#include "Assets/AtlasAsset.h"
#include "Assets/BinaryAsset.h"
#include "Assets/FolderAsset.h"
#include "Assets/ImageAsset.h"
#include "Assets/BitmapFontAsset.h"
#include "Assets/VectorFontAsset.h"

#include "Render/BitmapFont.h"
#include "Render/Camera.h"
#include "Render/Font.h"
#include "Render/Mesh.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "Render/Texture.h"
#include "Render/TextureRef.h"
#include "Render/VectorFont.h"
#include "Render/VectorFontEffects.h"

#include "Events/ApplicationEventsListener.h"
#include "Events/CursorEventsListener.h"
#include "Events/DragEventsListener.h"
#include "Events/EventSystem.h"
#include "Events/KeyboardEventsListener.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedValue.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Animation/Tween.h"

#include "UI/Button.h"
#include "UI/CustomDropDown.h"
#include "UI/CustomList.h"
#include "UI/DropDown.h"
#include "UI/EditBox.h"
#include "UI/HorizontalLayout.h"
#include "UI/HorizontalProgress.h"
#include "UI/HorizontalScrollBar.h"
#include "UI/Label.h"
#include "UI/List.h"
#include "UI/ScrollArea.h"
#include "UI/Toggle.h"
#include "UI/UIManager.h"
#include "UI/VerticalLayout.h"
#include "UI/VerticalProgress.h"
#include "UI/VerticalScrollBar.h"
#include "UI/Widget.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetState.h"

#define INITIALIZE_O2                                                              	                 \
o2::o2StackWalker* o2::o2StackWalker::instance = new o2StackWalker();				                 \
o2::MemoryManager* o2::MemoryManager::instance = new MemoryManager();				                 \
template<> o2::Ptr<o2::Debug> o2::Singleton<o2::Debug>::mInstance = mnew o2::Debug();                \
template<> o2::Ptr<o2::FileSystem> o2::Singleton<o2::FileSystem>::mInstance = mnew o2::FileSystem(); \
o2::Ptr<o2::Types> o2::Types::instance = mnew Types()

#define MAIN_O2 					 \
o2::Debug::SetupAsRootObject();		 \
o2::Types::instance.SetupAsRoot();	 \
o2::FileSystem::SetupAsRootObject(); \
o2::Types::InitializeTypes()


