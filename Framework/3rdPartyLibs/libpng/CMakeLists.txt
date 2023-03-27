add_library(libpng STATIC)
file(GLOB_RECURSE libpng_SOURCES "*.c" "*.h")
target_include_directories(libpng PUBLIC ../../)
target_sources(libpng PRIVATE ${libpng_SOURCES})
