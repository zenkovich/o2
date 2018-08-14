cmake_minimum_required(VERSION 3.4.1)

file(GLOB_RECURSE O2ENGINE_SRC
    "../../../../o2Engine/Sources/*.cpp" )

file(GLOB APP_SRC
    "../../../Sources/*.cpp"
    "src/main/cpp/native-lib.cpp" )

add_library( o2Engine
             SHARED
             ${O2ENGINE_SRC} )

add_library( app
             SHARED
             ${APP_SRC} )

find_library( log-lib
              log )

target_link_libraries( o2Engine
                       app
                       ${log-lib} )