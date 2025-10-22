#include <gtest/gtest.h>
#include "GstFrameGrabber.h"
#include "VideoSurfaceGL.h"

// To execute these tests:
// 1. Ensure GoogleTest is included and built as shown in your CMakeLists.txt.
// 2. Build the project so that the `rtspplayer_test` executable is generated.
// 3. Run the test binary from your build or output directory:
//      ./rtspplayer_test
//    Or from your Makefile-created output directory:
//      ./output/rtspplayer_test
// 4. Test results will be shown in the terminal.
//
// Example commands (from project root):
//      make build
//      ./output/rtspplayer_test
//
// ------------------ Unit Tests ------------------
