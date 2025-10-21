cmake_minimum_required(VERSION 3.10)
project(rtspplayer_example)


# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

add_executable(example main.cpp qml.qrc )
target_link_libraries(example PRIVATE ${LIB_ALIAS} Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)

