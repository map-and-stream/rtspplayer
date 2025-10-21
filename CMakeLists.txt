cmake_minimum_required(VERSION 3.10)
project(rtspplayer)

set(CMAKE_CXX_STANDARD 17)
set(LIB_ALIAS isiran::rtspplayer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

cmake_policy(SET CMP0079 NEW)


set(CMAKE_PREFIX_PATH "/home/javad/Qt/6.9.3/gcc_64")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 QuickWidgets)

# Build the example
add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(test)
