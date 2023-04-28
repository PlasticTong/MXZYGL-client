#ifndef POINTRENDER_H
#define POINTRENDER_H

#include <QWidget>
#include <QtDataVisualization>
#include <QGridLayout>
#include "datarender.h"

using namespace QtDataVisualization;

class PointRender : public DataRender
{
    Q_OBJECT
public:
    explicit PointRender(QWidget *parent = nullptr);
    ~PointRender();

    void refresh();

    void renderData(void *strdata);
    void renderFile(QString filename);
    void clear();

    QString getData();



signals:

private:
    QString             _data_str = "";
    QScatterDataArray *_data_array = nullptr;

    QGridLayout *_gridlayout    = nullptr;
    Q3DScatter  *_graph         = nullptr;
    QWidget     *_container     = nullptr;

};

#endif // POINTRENDER_H
