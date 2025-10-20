cmake_minimum_required(VERSION 3.16)
project(StreamViewPro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 QuickWidgets)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED
    gstreamer-1.0
    gstreamer-app-1.0
    gstreamer-video-1.0
    gstreamer-plugins-base-1.0
)

if(NOT GSTREAMER_FOUND)
    message(FATAL_ERROR "GStreamer not found")
endif()

qt_add_executable(${PROJECT_NAME}
    main.cpp
    FrameGrabber.cpp
    FrameGrabber.h
    VideoSurface.cpp
    VideoSurface.h
    qml.qrc
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${GSTREAMER_INCLUDE_DIRS}
)

link_directories(${GSTREAMER_LIBRARY_DIRS})

target_compile_options(${PROJECT_NAME}
    PRIVATE
    ${GSTREAMER_CFLAGS_OTHER}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::QuickWidgets
    ${GSTREAMER_LIBRARIES}
)
