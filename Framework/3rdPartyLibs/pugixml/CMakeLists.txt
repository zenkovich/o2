add_library(pugixml STATIC)
file(GLOB_RECURSE pugixml_SOURCES "*.cpp" "*.hpp")
target_sources(pugixml PRIVATE ${pugixml_SOURCES})
