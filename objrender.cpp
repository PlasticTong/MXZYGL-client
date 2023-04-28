#include "objrender.h"

ObjRender::ObjRender(QWidget *parent) : DataRender(parent)
{
    _gridlayout = new QGridLayout(this);
    _glwiget      = new GLWidget();
    _gridlayout->addWidget(_glwiget);
    _gridlayout->setMargin(0);
    QObject::connect(this, SIGNAL(sgRenderFile(QString)), this, SLOT(doRenderFile(QString)));
}


void ObjRender::renderData(void *strdata)
{

}
void ObjRender::renderFile(QString filename)
{
//    QString filepath = filename.replace(".txt", ".obj");
    QString filepath = filename;
    qDebug() <<"ObjRender::renderFile"<<filepath;
    emit sgRenderFile(filepath);
}
void ObjRender::clear()
{

}

void ObjRender::doRenderFile(QString filepath)
{
    _glwiget->renderObjFile(filepath);
}
