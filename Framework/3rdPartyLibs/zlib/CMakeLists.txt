add_library(zlib STATIC)
file(GLOB_RECURSE zlib_SOURCES "*.c" "*.h")
target_sources(zlib PRIVATE ${zlib_SOURCES})
