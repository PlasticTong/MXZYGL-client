#ifndef FBXRENDER_H
#define FBXRENDER_H

#include <QWidget>
#include <QGridLayout>
#include "datarender.h"
#include "glwidget.h"

class FbxRender : public DataRender
{
    Q_OBJECT
public:
    FbxRender(QWidget *parent = nullptr);

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

#endif // FBXRENDER_H
