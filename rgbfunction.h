#ifndef RGBFUNCTION_H
#define RGBFUNCTION_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class RgbFunction: public QOpenGLFunctions
{
public:
    RgbFunction();

    void initialize();

    void render(unsigned char* ptr,int width,int height,int type);

private:

    QOpenGLShaderProgram m_program;

    GLuint m_textureUniform;

    GLuint m_id;
};

#endif // RGBFUNCTION_H
