#ifndef MULTIPOINTPANEL_H
#define MULTIPOINTPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include "simplepanel.h"

class MultiPanel : public QWidget
{
//    Q_OBJECT
public:
    explicit MultiPanel(uint rows=3, uint cols=3, QWidget *parent = nullptr);
    ~MultiPanel();

    void showModels(QVector<ModelInfo> &models);
    void renderFile(QString &filepath, uint row, uint col);
    void clearAt(uint row, uint col);
    void clear();
    void setInfoAt(const QString &text, uint row, uint col);


    int rows();
    int cols();
    int size();

signals:


private:
    QVector<QVector<SimplePanel*>>  _panels;
    QVector<ModelInfo>              _models;
};

#endif // MULTIPOINTPANEL_H
