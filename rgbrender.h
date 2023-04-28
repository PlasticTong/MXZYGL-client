#ifndef RGBRENDER_H
#define RGBRENDER_H

#include "datarender.h"
#include "rgbwidget.h"
#include <opencv2/opencv.hpp>
#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QResizeEvent>

class RgbRender : public DataRender
{
    Q_OBJECT
public:
    RgbRender(QWidget *parent = nullptr);

    void renderData(void *strdata);
    void renderFile(QString filename);
    void clear();

    void resizeEvent(QResizeEvent* size);

signals:
    void sgRenderFile(QString filepath);

public slots:
    void doRenderFile(QString filepath);

private:
    RgbWidget   *_imgwidget     = nullptr;
    QGridLayout *_gridlayout    = nullptr;
    QLabel      *_imglabel      = nullptr;
    QImage      *_img           = nullptr;
    cv::Mat     _cvimg;
};

#endif // RGBRENDER_H
