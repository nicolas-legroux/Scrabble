cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

set(PROJECT_NAME Scrabble)

project(${PROJECT_NAME} CXX)

# Find QT5Widgets
find_package(Qt5Widgets REQUIRED)

# Tell CMake to run moc when necessary
set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(INCLUDE_DIR ${ROOT_DIR}/src/includes)

set(CMAKE_CXX_FLAGS "-Wall -O3 -std=c++14 -fopenmp")

file(GLOB SRC_FILES
	"${ROOT_DIR}/src/Scrabble.cpp"
	"${ROOT_DIR}/src/Trie.hpp"
	"${ROOT_DIR}/src/Trie.cpp"
	"${ROOT_DIR}/src/ScrabbleConstants.hpp"
	"${ROOT_DIR}/src/ScrabbleRack.hpp"
	"${ROOT_DIR}/src/ScrabbleRack.cpp"
	"${ROOT_DIR}/src/ScrabbleGrid.hpp"
	"${ROOT_DIR}/src/ScrabbleGrid.cpp"
	"${ROOT_DIR}/src/ScrabblePlayer.hpp"
	"${ROOT_DIR}/src/ScrabblePlayer.cpp"
	"${ROOT_DIR}/src/ScrabbleGridElement.hpp"
	"${ROOT_DIR}/src/ScrabbleGridElement.cpp"
	"${ROOT_DIR}/src/ScrabbleGridWidget.hpp"
	"${ROOT_DIR}/src/ScrabbleGridWidget.cpp"
	"${ROOT_DIR}/src/ScrabbleRackWidget.cpp"
	"${ROOT_DIR}/src/ScrabbleRackWidget.hpp"
	"${ROOT_DIR}/src/ScrabbleMain.hpp"
	"${ROOT_DIR}/src/ScrabbleMain.cpp"
)

include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

