#include "VideoSurfaceGL.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QElapsedTimer>
#include <QOpenGLFramebufferObject>


class VideoRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:
    uchar *rawData = nullptr;
    int width = 0;
    int height = 0;

    GLuint textureId = 0;
    QOpenGLShaderProgram program;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

    int frameCount = 0;
    QElapsedTimer fpsTimer;

    const char *vertexShaderSrc = R"(
        attribute vec2 position;
        attribute vec2 texCoord;
        varying vec2 vTexCoord;
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
            vTexCoord = texCoord;
        }
    )";

    const char *fragmentShaderSrc = R"(
        uniform sampler2D texture;
        varying vec2 vTexCoord;
        void main() {
            gl_FragColor = texture2D(texture, vTexCoord);
        }
    )";

    void synchronize(QQuickFramebufferObject *item) override {
        auto surface = static_cast<VideoSurfaceGL *>(item);
        rawData = surface->rawData;
        width = surface->rawWidth;
        height = surface->rawHeight;
    }

    void render() override {
        initializeOpenGLFunctions();

        if (!fpsTimer.isValid())
            fpsTimer.start();

        glViewport(0, 0, framebufferObject()->width(), framebufferObject()->height());
        glClear(GL_COLOR_BUFFER_BIT);

        if (!rawData || width == 0 || height == 0)
            return;

        if (textureId == 0)
            glGenTextures(1, &textureId);

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (!program.isLinked()) {
            program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
            program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc);
            program.link();

            GLfloat vertices[] = {
                -1.0f, -1.0f,  0.0f, 0.0f,
                1.0f, -1.0f,  1.0f, 0.0f,
                1.0f,  1.0f,  1.0f, 1.0f,
                -1.0f,  1.0f,  0.0f, 1.0f,
            };

            vao.create();
            vao.bind();

            vbo.create();
            vbo.bind();
            vbo.allocate(vertices, sizeof(vertices));

            program.enableAttributeArray("position");
            program.setAttributeBuffer("position", GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));

            program.enableAttributeArray("texCoord");
            program.setAttributeBuffer("texCoord", GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));

            vbo.release();
            vao.release();
        }

        program.bind();
        vao.bind();
        glBindTexture(GL_TEXTURE_2D, textureId);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        vao.release();
        program.release();

        frameCount++;
        if (fpsTimer.elapsed() >= 1000) {
            //qDebug() << "Rendered FPS:" << frameCount;
            frameCount = 0;
            fpsTimer.restart();
        }
    }

    ~VideoRenderer() override {
        if (textureId)
            glDeleteTextures(1, &textureId);
        vbo.destroy();
        vao.destroy();
    }
};

VideoSurfaceGL::VideoSurfaceGL(QQuickItem *parent)
    : QQuickFramebufferObject(parent) {}

QQuickFramebufferObject::Renderer *VideoSurfaceGL::createRenderer() const {
    return new VideoRenderer();
}

void VideoSurfaceGL::setRawFrame(uchar *data, int w, int h) {
    rawData = data;
    rawWidth = w;
    rawHeight = h;
    update();
}
