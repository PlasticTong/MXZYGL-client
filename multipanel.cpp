#include "multipanel.h"

MultiPanel::MultiPanel(uint rows, uint cols, QWidget *parent) : QWidget(parent)
{
    QGridLayout *_gridlayout = new QGridLayout(this);
    _gridlayout->setMargin(0);
    _gridlayout->setSpacing(1);
    for(int i = 0; i < rows; ++i)
    {
        _panels.append(QVector<SimplePanel*>());
        for(int j = 0; j < cols; ++j)
        {
            SimplePanel *panel = new SimplePanel();
            _panels[i].append(panel);
            _gridlayout->addWidget(panel, i, j, 1, 1);
        }
    }
    this->setLayout(_gridlayout);
}

MultiPanel::~MultiPanel()
{

}

void MultiPanel::showModels(QVector<ModelInfo> &models)
{
    _models = models;
}

void MultiPanel::renderFile(QString &filepath, uint row, uint col)
{
    if(row >= _panels.size() || col >= _panels[0].size())
    {
        qDebug() << "position error";
        return;
    }
    _panels[row][col]->renderFile(filepath);
}

void MultiPanel::clearAt(uint row, uint col)
{
    if(row >= _panels.size() || col >= _panels[0].size())
    {
        qDebug() << "position error";
        return;
    }
    _panels[row][col]->clear();
}

void MultiPanel::clear()
{
    int rows = _panels.size();
    if(0 == rows) return;
    int cols = _panels[0].size();
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < rows; ++j)
        {
            clearAt(i, j);
        }
    }
}

void MultiPanel::setInfoAt(const QString &text, uint row, uint col)
{
    if(row >= _panels.size() || col >= _panels[0].size())
    {
        qDebug() << "position error";
        return;
    }
    _panels[row][col]->setInfo(text);
}

int MultiPanel::rows()
{
    return _panels.size();
}

int MultiPanel::cols()
{
    return (_panels.size() > 0) ? _panels[0].size() : 0;
}

int MultiPanel::size()
{
    return this->rows() * this->cols();
}
