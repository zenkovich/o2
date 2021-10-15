cmake_minimum_required(VERSION 3.10)

project(firework LANGUAGES CXX)

set(SOURCE_FILES
	Src/CodeToolApp.cpp
	Src/CodeTool.cpp
	Src/CppSyntaxParser.cpp
	Src/SyntaxTree.cpp
	Src/pugixml/pugixml.cpp
)
set(HEADER_FILES
	Src/CodeToolApp.h
	Src/CppSyntaxParser.h
	Src/SyntaxTree.h
	Src/pugixml/pugixml.hpp
	Src/pugixml/pugiconfig.hpp
)

set(CMAKE_CXX_STANDARD 17)

add_executable(CodeTool WIN32 ${HEADER_FILES} ${SOURCE_FILES})
