set(LIBRARY_NAME rtspplayer)

set(RTSPPLAYER_SOURCES
    FrameGrabber.cpp
    VideoSurface.cpp
)

set(RTSPPLAYER_HEADERS
    FrameGrabber.h
    VideoSurface.h
)



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


add_library(${LIBRARY_NAME} STATIC
    ${RTSPPLAYER_SOURCES}
    ${RTSPPLAYER_HEADERS}  # ← include headers in target (for IDEs)
)

target_link_libraries(${LIBRARY_NAME}
    PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::QuickWidgets
    ${GSTREAMER_LIBRARIES}
)


target_include_directories(${LIBRARY_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GSTREAMER_INCLUDE_DIRS}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_options(${LIBRARY_NAME}
    PRIVATE
    ${GSTREAMER_CFLAGS_OTHER}
)

# Convenient namespaced alias for downstream targets
add_library(${LIB_ALIAS} ALIAS ${LIBRARY_NAME})
