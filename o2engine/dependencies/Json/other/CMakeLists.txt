cmake_minimum_required(VERSION 2.8.12)

project(JsonBox)

include(GenerateExportHeader)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

set(JSONBOX_SOURCES
  src/JsonWritingError.cpp
  src/Value.cpp
  src/Array.cpp
  src/SolidusEscaper.cpp
  src/Escaper.cpp
  src/Indenter.cpp
  src/IndentCanceller.cpp
  src/JsonParsingError.cpp
  src/Convert.cpp
  src/Object.cpp
)
set(JSONBOX_HEADERS
  include/JsonBox/Array.h
  include/JsonBox/Convert.h
  include/JsonBox/Escaper.h
  include/JsonBox/Grammar.h
  include/JsonBox/IndentCanceller.h
  include/JsonBox/Indenter.h
  include/JsonBox/JsonParsingError.h
  include/JsonBox/JsonWritingError.h
  include/JsonBox/Object.h
  include/JsonBox/OutputFilter.h
  include/JsonBox/SolidusEscaper.h
  include/JsonBox/Value.h
  include/JsonBox.h
  ${CMAKE_CURRENT_BINARY_DIR}/Export.h
)

# build library
set(CMAKE_DEBUG_POSTFIX "_d")
add_library(JsonBox ${JSONBOX_SOURCES} ${JSONBOX_HEADERS})

generate_export_header(JsonBox EXPORT_FILE_NAME Export.h)

target_include_directories(JsonBox PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
)
target_include_directories(JsonBox SYSTEM INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)

export(TARGETS JsonBox
  FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
)

# install
install(TARGETS JsonBox
  COMPONENT lib
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib${LIB_SUFFIX}
  LIBRARY DESTINATION lib${LIB_SUFFIX}
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(FILES ${JSONBOX_HEADERS}
  COMPONENT dev
  DESTINATION include
)
install(EXPORT ${PROJECT_NAME}
  DESTINATION lib${LIB_SUFFIX}/cmake
  FILE ${PROJECT_NAME}Config.cmake
)


# examples
add_executable(example1 "${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp")
add_dependencies(example1 JsonBox)
target_link_libraries(example1 JsonBox)
