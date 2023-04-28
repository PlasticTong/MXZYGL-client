#include "rgbfunction.h"

#define ATTRIB_VERTEX 0
#define ATTRIB_TEXTURE 1


RgbFunction::RgbFunction()
{

}

void RgbFunction::initialize()
{
    //初始化opengl （QOpenGLFunctions继承）函数
    initializeOpenGLFunctions();

    //顶点shader
    const char *vString =
            "attribute vec4 vertexPosition;\
            attribute vec2 textureCoordinate;\
    varying vec2 texture_Out;\
    void main(void)\
    {\
        gl_Position = vertexPosition;\
        texture_Out = textureCoordinate;\
    }";
    //片元shader
    const char *tString =
            "varying vec2 texture_Out;\
            uniform sampler2D tex;\
    void main(void)\
    {\
        gl_FragColor = texture2D(tex, texture_Out); \
    }";

    //m_program加载shader（顶点和片元）脚本
    //片元（像素）
    qDebug()<<m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, tString);
    //顶点shader
    qDebug() << m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, vString);

    //设置顶点位置
    m_program.bindAttributeLocation("vertexPosition",ATTRIB_VERTEX);
    //设置纹理位置
    m_program.bindAttributeLocation("textureCoordinate",ATTRIB_TEXTURE);

    //编译shader
    qDebug() << "m_program.link() = " << m_program.link();

    qDebug() << "m_program.bind() = " << m_program.bind();

    //传递顶点和纹理坐标
    //顶点
    static const GLfloat ver[] = {
        -1.0f,-1.0f,
        1.0f,-1.0f,
        -1.0f, 1.0f,
        1.0f,1.0f
        //        -1.0f,-1.0f,
        //        0.9f,-1.0f,
        //        -1.0f, 1.0f,
        //        0.9f,1.0f
    };
    //纹理
    static const GLfloat tex[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    //设置顶点,纹理数组并启用
    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, ver);
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, tex);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);

    //从shader获取地址
    m_textureUniform = m_program.uniformLocation("tex");

    //创建纹理
    glGenTextures(1, &m_id);
    //只需要一个texture
    glBindTexture(GL_TEXTURE_2D, m_id);
    //放大过滤，线性插值   GL_NEAREST(效率高，但马赛克严重)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RgbFunction::render(unsigned char *ptr, int width, int height, int type)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_id);
    //修改纹理内容(复制内存内容)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr);
    //与shader 关联
    glUniform1i(m_textureUniform, 0);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}
