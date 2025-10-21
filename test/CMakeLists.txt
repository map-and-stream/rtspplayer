# Add GoogleTest
add_subdirectory(googletest)

# Enable testing
enable_testing()

# Create your test target
add_executable(rtspplayer_test
    test_rtspplayer.cpp
)

# Link against GoogleTest and pthread
target_link_libraries(rtspplayer_test
PRIVATE
    ${LIB_ALIAS}
    GTest::gtest
    GTest::gtest_main
    pthread
)
