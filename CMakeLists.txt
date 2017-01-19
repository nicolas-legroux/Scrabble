cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

set(PROJECT_NAME Scrabble)

project(${PROJECT_NAME} CXX)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(INCLUDE_DIR ${ROOT_DIR}/src/includes)

set(CMAKE_CXX_FLAGS "-Wall -O3 -std=c++14 -fopenmp")

file(GLOB SRC_FILES
	"${ROOT_DIR}/src/Scrabble.cpp"
	"${ROOT_DIR}/src/Trie.hpp"
	"${ROOT_DIR}/src/Trie.cpp"
	"${ROOT_DIR}/src/ScrabbleRack.hpp"
	"${ROOT_DIR}/src/ScrabbleRack.cpp"
	"${ROOT_DIR}/src/ScrabbleGrid.hpp"
	"${ROOT_DIR}/src/ScrabbleGrid.cpp"
)

include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SRC_FILES})

