#include "simplepanel.h"

SimplePanel::SimplePanel(QWidget *parent) : QWidget(parent)
{
//    _ptsrender      = new PointRender(this);
//    _offrender      = new OffRender(this);
    _objrender      = new ObjRender(this);
    _fbxrender      = new FbxRender(this);
//    _rgbrender      = new RgbRender(this);
    _label_info     = new QLabel(this);
    _gridlayout     = new QGridLayout(this);
    _gridlayout->setMargin(0);
    _gridlayout->setSpacing(0);
//    _gridlayout->addWidget(_offrender, 0, 0);
    _gridlayout->addWidget(_objrender, 0, 0);
    _gridlayout->addWidget(_fbxrender, 0, 0);
//    _gridlayout->addWidget(_rgbrender, 0, 0);
    _gridlayout->addWidget(_label_info, 1, 0);
    _label_info->setStyleSheet("background-color:white;color:black;");
//    _label_info->setFrameShape(QFrame::Panel);
    _label_info->setAlignment(Qt::AlignHCenter);
    _label_info->setMaximumHeight(16);
    this->setLayout(_gridlayout);

//    _ptsrender->hide();
//    _offrender->hide();
    _objrender->hide();
    _fbxrender->hide();
//    _rgbrender->hide();
}

SimplePanel::~SimplePanel()
{

}

void SimplePanel::renderData(void *data)
{
//    _rgbrender->renderData((QScatterDataArray*)data);
}

void SimplePanel::renderFile(QString filepath)
{
////    qDebug() <<"文件路径是："<< filepath;
//    if(filepath.contains("shapenet"))
//    {
//        _rgbrender->renderFile(filepath);
////        _offrender->hide();
//        _objrender->hide();
//        _rgbrender->show();
//    }
//    else
//    {
////        _offrender->renderFile(filepath);
//        _objrender->renderFile(filepath);
//        _rgbrender->hide();
////        _offrender->show();
//        _objrender->show();
//    }

    if(filepath.contains(".obj"))
    {
        _objrender->renderFile(filepath);
        _fbxrender->hide();
        _objrender->show();
    }
    else if (filepath.contains(".fbx"))
    {
        _fbxrender->renderFile(filepath);
        _objrender->hide();
        _fbxrender->show();
    }
}

void SimplePanel::setInfo(QString info)
{
    _label_info->setText(info);
}

void SimplePanel::showModel(ModelInfo &model)
{
//    _rgbrender->renderFile(model.other);
    _label_info->setText(QString::asprintf("类别：%s,  相似度：%f", model.category.toStdString().c_str(), model.similarity));
}

void SimplePanel::clear()
{
//    _rgbrender->clear();
    _label_info->setText("");
}
