#include "fbxrender.h"

FbxRender::FbxRender(QWidget *parent) : DataRender(parent)
{
    _gridlayout = new QGridLayout(this);
    _glwiget      = new GLWidget();
    _gridlayout->addWidget(_glwiget);
    _gridlayout->setMargin(0);
    QObject::connect(this, SIGNAL(sgRenderFile(QString)), this, SLOT(doRenderFile(QString)));
}

void FbxRender::renderData(void *strdata)
{

}
void FbxRender::renderFile(QString filename)
{
//    QString filepath = filename.replace(".txt", ".obj");
    QString filepath = filename;
    qDebug() <<"FbxRender::renderFile"<<filepath;
    emit sgRenderFile(filepath);
}
void FbxRender::clear()
{

}

void FbxRender::doRenderFile(QString filepath)
{
    _glwiget->renderFbxFile(filepath);
}
