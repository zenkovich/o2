cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wno-unused-variable -Wno-overloaded-virtual -Wno-reorder -Wno-unused-local-typedef -Wno-unknown-pragmas")

file(GLOB_RECURSE APP_SRC
    "src/main/cpp/JavaBridge.cpp"
    "../../../Sources/*.cpp"
    "../../../../o2Engine/Sources/*.cpp"
    "../../../../o2Engine/dependencies/libpng/*.c"
    "../../../../o2Engine/dependencies/pugixml/*.cpp"
    "../../../../o2Engine/dependencies/Zlib/*.c" )

add_library( app
             SHARED
             ${APP_SRC} )

add_library( freetype2
             SHARED IMPORTED )

set_target_properties( freetype2 PROPERTIES IMPORTED_LOCATION
			  ../../../../src/main/jniLibs/${ANDROID_ABI}/libfreetype2.so )

target_link_libraries( app
                       android
                       log
                       EGL
                       GLESv2
					   freetype2
					   )

include_directories(../../../../o2Engine/Sources)
include_directories(../../../../o2Engine/dependencies/FreeTypeAndroid)
include_directories(../../../../o2Engine/dependencies/FreeTypeAndroid/freetype2)
include_directories(../../../../o2Engine/dependencies)
include_directories(../../../../o2Engine/)
include_directories(../../../Sources)