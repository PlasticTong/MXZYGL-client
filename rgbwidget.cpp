#include "rgbwidget.h"
#include <QTimerEvent>

RgbWidget::RgbWidget(QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
    _rgbfunc = new RgbFunction();
    _timer_fresh = startTimer(10);
    _image = new QImage("E:/pictures/zhang.jpg");
}

void RgbWidget::updateImg(uchar *data, int w, int h)
{
    _data = data;
    _width = w;
    _height = h;
    this->repaint();
}

void RgbWidget::doFresh()
{

}

void RgbWidget::initializeGL()
{
    _rgbfunc->initialize();
}

void RgbWidget::paintGL()
{
    if(nullptr != _data && _width > 0 && _height > 0)
        _rgbfunc->render(_data, _width, _height, 0);
}

void RgbWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void RgbWidget::timerEvent(QTimerEvent *event)
{
    if(_timer_fresh == event->timerId())
    {
        doFresh();
    }
}
