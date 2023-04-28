#include "offrender.h"

OffRender::OffRender(QWidget *parent) : DataRender(parent)
{
    _gridlayout = new QGridLayout(this);
    _glwiget      = new GLWidget();
    _gridlayout->addWidget(_glwiget);
    _gridlayout->setMargin(0);
    QObject::connect(this, SIGNAL(sgRenderFile(QString)), this, SLOT(doRenderFile(QString)));
}

OffRender::~OffRender()
{

}

void OffRender::renderData(void *strdata)
{

}

void OffRender::renderFile(QString filename)
{
    QString filepath = filename.replace(".txt", ".off");
    qDebug() <<"OffRender::renderFile"<<filepath;
    emit sgRenderFile(filepath);
}

void OffRender::clear()
{

}


void OffRender::doRenderFile(QString filepath)
{
    _glwiget->renderOffFile(filepath);
}




