add_library(StackWalker STATIC)
file(GLOB_RECURSE StackWalker_SOURCES "*.cpp" "*.h")
target_sources(StackWalker PRIVATE ${StackWalker_SOURCES})
