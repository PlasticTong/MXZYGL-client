#ifndef MODELTABLE_H
#define MODELTABLE_H

#include <QWidget>
#include <QtWidgets>
#include "mydefine.h"
#include "pointrender.h"
#include "multipanel.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

class ModelTable : public QWidget
{
    Q_OBJECT
public:
    explicit ModelTable(uint panel_rows = 3, uint panel_cols = 3, QWidget *parent = nullptr);

signals:

public slots:
    void doItemSelectionChanged();
    void doVerticalSectionClicked(int idx);
    void doVerticalSectionDoubleClicked(int idx);

public:
    void setModels(QList<ModelInfo> &models);
    void setModels2(ModelsInfo &models);
    void setModels3(const QJsonObject &table_data);
    void setModels4(const QJsonObject &models);
    void readFiles(const QStringList &filenames);
    void refresh();
    TableJson getModels();
    void clear();
private:
    QTableWidget    *_tablewidget   = nullptr;
    MultiPanel      *_multipanel    = nullptr;
    QJsonObject     _models;
    QJsonObject     _table_data;

};

#endif // MODELTABLE_H
