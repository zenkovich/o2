# Based on https://github.com/OGRECave/ogre/blob/master/CMake/Packages/FindOpenGLES2.cmake

# Once done this will define
#
#  OPENGLES2_FOUND        - system has OpenGLES
#  OPENGLES2_INCLUDE_DIR  - the GL include directory
#  OPENGLES2_LIBRARIES    - Link these to use OpenGLES

find_path( OPENGLES2_INCLUDE_DIR GLES2/gl2.h
    /usr/include
)

find_library( OPENGLES2_gl_LIBRARY
    NAMES GLESv2
    PATHS /usr/shlib
          /usr/lib
)

if( OPENGLES2_gl_LIBRARY AND OPENGLES2_INCLUDE_DIR )
    set( OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} )
    set( OPENGLES2_FOUND TRUE )
endif()

mark_as_advanced(
    OPENGLES2_INCLUDE_DIR
    OPENGLES2_gl_LIBRARY
)

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args(
    OpenGLES2
    REQUIRED_VARS OPENGLES2_INCLUDE_DIR OPENGLES2_gl_LIBRARY
    FAIL_MESSAGE "OpenGLES2 is not found."
)

if( OPENGLES2_FOUND )
    if( NOT TARGET OpenGLES2::OpenGLES2 )
        add_library( OpenGLES2::OpenGLES2 SHARED IMPORTED )
        set_target_properties(
            OpenGLES2::OpenGLES2
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${OPENGLES2_INCLUDE_DIR}"
                IMPORTED_LOCATION "${OPENGLES2_LIBRARIES}"
        )
    endif()
endif()
