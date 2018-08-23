cmake_minimum_required(VERSION 3.4.1)

file(GLOB_RECURSE O2ENGINE_SRC
    "../../../../o2Engine/Sources/*.cpp" )

file(GLOB_RECURSE APP_SRC
    "../../../Sourcess/*.cpp"
    "src/main/cpp/native-lib.cpp" )

file(GLOB_RECURSE FREE_TYPE_SRC
    "../../../../o2Engine/dependencies/FreeTypeAndroid/*.c" )

add_library( FreeType
             SHARED
             ${FREE_TYPE_SRC} )

add_library( o2Engine
             SHARED
             ${O2ENGINE_SRC} )

add_library( app
             SHARED
             ${APP_SRC} )

find_library( log-lib
              log )

target_link_libraries( FreeType
                       o2Engine
                       app
                       ${log-lib} )

include_directories(../../../../o2Engine/Sources)
include_directories(../../../../o2Engine/dependencies/FreeTypeAndroid/include)
include_directories(../../../Sources)