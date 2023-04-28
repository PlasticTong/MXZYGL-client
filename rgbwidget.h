#ifndef RGBWIDGET_H
#define RGBWIDGET_H

#include "rgbfunction.h"

#include <QImage>
#include <QOpenGLWidget>

class RgbWidget: public QOpenGLWidget
{
    Q_OBJECT
public:
    RgbWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void updateImg(uchar *data, int w, int h);

public slots:
    void doFresh();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void timerEvent(QTimerEvent *event);

public:
    RgbFunction *_rgbfunc = nullptr;
    int         _timer_fresh = 0;
    uchar       *_data = nullptr;
    int         _width = 0;
    int         _height = 0;
    QImage      *_image = nullptr;
};

#endif // RGBWIDGET_H
