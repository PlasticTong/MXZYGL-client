#ifndef OFFRENDER_H
#define OFFRENDER_H

#include <QWidget>
#include <QGridLayout>
#include "datarender.h"
#include "glwidget.h"

class OffRender : public DataRender
{
    Q_OBJECT
public:
    OffRender(QWidget *parent = nullptr);
    ~OffRender();

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

#endif // OFFRENDER_H
