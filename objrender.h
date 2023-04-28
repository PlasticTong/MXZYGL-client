#ifndef OBJRENDER_H
#define OBJRENDER_H

#include <QWidget>
#include <QGridLayout>
#include "datarender.h"
#include "glwidget.h"

class ObjRender : public DataRender
{
    Q_OBJECT
public:
    ObjRender(QWidget *parent = nullptr);

    void renderData(void *strdata);
    void renderFile(QString filename);
    void clear();

signals:
    void sgRenderFile(QString filepath);

public slots:
    void doRenderFile(QString filepath);

private:
    GLWidget    *_glwiget       = nullptr;
    QGridLayout *_gridlayout    = nullptr;
};

#endif // OBJRENDER_H
