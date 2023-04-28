#include "pointrender.h"
#include "myutils.h"

PointRender::PointRender(QWidget *parent) : DataRender(parent)
{
    _gridlayout = new QGridLayout(this);
    _graph      = new Q3DScatter();
    _container  = QWidget::createWindowContainer(_graph);
    _gridlayout->addWidget(_container);
    _gridlayout->setMargin(0);

    _graph->axisX()->setTitle("X");
    _graph->axisY()->setTitle("Y");
    _graph->axisZ()->setTitle("Z");

//    _graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
    QFont font = _graph->activeTheme()->font();
    font.setPointSize(40);
    _graph->activeTheme()->setFont(font);
    _graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    _graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
//    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(true);
    _graph->addSeries(series);
    _graph->activeTheme()->setGridEnabled(false);
    _graph->activeTheme()->setBackgroundEnabled(false);
    _graph->activeTheme()->setBackgroundColor(Qt::white);
    _graph->activeTheme()->setLabelBorderEnabled(false);
    _graph->activeTheme()->setLabelTextColor(Qt::white);
    _graph->axisX()->setLabelFormat("");
    _graph->axisY()->setLabelFormat("");
    _graph->axisZ()->setLabelFormat("");



    _data_array = new QScatterDataArray();
}

PointRender::~PointRender()
{

}

void PointRender::refresh()
{
    _data_array->clear();

    if(!_data_str.isEmpty())
    {
        QStringList points = _data_str.split(" ");
        for(auto point: points)
        {
            QStringList xyz= point.split(",");
            if(xyz.length() >= 3)
            {
                _data_array->append(QVector3D(xyz[0].toFloat(), xyz[1].toFloat(), xyz[2].toFloat()));
            }
        }
    }

    _graph->seriesList().at(0)->dataProxy()->resetArray(_data_array);
}

void PointRender::renderData(void *strdata)
{
    _data_str = (nullptr != strdata) ? *((QString*)strdata) : "";
    this->refresh();
}

void PointRender::renderFile(QString filename)
{
    readPlainFile(filename, _data_str, " ");
    this->refresh();
}

void PointRender::clear()
{
    _data_str = "";
    this->refresh();
}

QString PointRender::getData()
{

    return _data_str;
}

