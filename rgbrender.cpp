#include "rgbrender.h"


RgbRender::RgbRender(QWidget *parent) : DataRender(parent)
{
    _gridlayout = new QGridLayout(this);
//    _imgwidget  = new RgbWidget(this);
//    _gridlayout->addWidget(_imgwidget);

    _img = new QImage();
    _imglabel   = new QLabel(this);
    _gridlayout->addWidget(_imglabel);
    _gridlayout->setMargin(0);
    this->setLayout(_gridlayout);
    QObject::connect(this, SIGNAL(sgRenderFile(QString)), this, SLOT(doRenderFile(QString)));
}

void RgbRender::renderData(void *strdata)
{

}

void RgbRender::renderFile(QString filename)
{
    QString filepath = filename.replace(".txt", ".png");
    qDebug() <<"11"<<filepath;
    emit sgRenderFile(filepath);
}

void RgbRender::clear()
{

}

void RgbRender::resizeEvent(QResizeEvent *size)
{
    if(_img->width() != 0 && 0 != _img->height())
    {
        float w2h = float(_img->width())/float(_img->height());
        int h = this->width() / w2h;
        int w = this->height() * w2h;
        if(h <= this->height())
        {
            w = this->width();
        }
        else
        {
            h = this->height();
        }
//        int w = this->width();
//        int h = this->height();
        QPoint center = this->geometry().center();
        _imglabel->setGeometry(center.x()-w/2, center.y()-h/2, w, h);
        _imglabel->setScaledContents(true);
    }
}

cv::Mat BGRToRGB(cv::Mat img)
{
    cv::Mat image(img.rows, img.cols, CV_8UC3);
    for (int i = 0; i < img.rows; ++i)
    {
        //获取第i行首像素指针
        cv::Vec3b *p1 = img.ptr<cv::Vec3b>(i);
        cv::Vec3b *p2 = image.ptr<cv::Vec3b>(i);
        for(int j=0; j<img.cols; ++j)
        {
            //将img的bgr转为image的rgb
            p2[j][2] = p1[j][0];
            p2[j][1] = p1[j][1];
            p2[j][0] = p1[j][2];
        }
    }
    return image;
}

void RgbRender::doRenderFile(QString filepath)
{
    _img->load(filepath);
    _img->scaled(_imglabel->width(), _imglabel->height(), Qt::KeepAspectRatio);
    _imglabel->setPixmap(QPixmap::fromImage(*_img));

//    _cvimg = cv::imread(filepath.toStdString(), 1);
//    cv::cvtColor(_cvimg, _cvimg, cv::COLOR_BGR2RGB);
//    _imgwidget->updateImg(_cvimg.data, _cvimg.cols, _cvimg.rows);
}
