#include <gtest/gtest.h>
#include "FrameGrabber.h"
#include "VideoSurface.h"

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

TEST(FrameGrabberTest, ConstructionAndProcessingEnabled) {
    FrameGrabber grabber;
    // By default, processing should be disabled
    EXPECT_FALSE(grabber.isProcessingEnabled());

    // Enable processing and check
    grabber.setProcessingEnabled(true);
    EXPECT_TRUE(grabber.isProcessingEnabled());

    // Disable again and check
    grabber.setProcessingEnabled(false);
    EXPECT_FALSE(grabber.isProcessingEnabled());
}

TEST(VideoSurfaceTest, FrameSetAndGet) {
    VideoSurface surface;
    QImage image(100, 100, QImage::Format_RGB32);
    image.fill(Qt::red);
    surface.setFrame(image);

    QImage result = surface.frame();
    // The image should not be null and should be equal to the one set
    EXPECT_FALSE(result.isNull());
    EXPECT_EQ(result.size(), QSize(100, 100));
    // Check that the pixel at (0,0) is red
    EXPECT_EQ(result.pixelColor(0, 0), Qt::red);
}

TEST(FrameGrabberTest, FrameReadySignalExists) {
    FrameGrabber grabber;
    bool fired = false;
    QObject::connect(&grabber, &FrameGrabber::frameReady, [&](const QImage &) {
        fired = true;
    });
    // We can't actually cause a frame to be ready without real RTSP,
    // but this ensures the signal exists and can be connected.
    EXPECT_FALSE(fired);
}
