#ifndef SIMPLEPANEL_H
#define SIMPLEPANEL_H

#include <QWidget>
#include <QLabel>
#include <QStatusBar>
#include <QSplitter>
#include "myutils.h"
#include "pointrender.h"
#include "offrender.h"
#include "objrender.h"
#include "rgbrender.h"
#include "fbxrender.h"

class SimplePanel : public QWidget
{
    Q_OBJECT
public:
    explicit SimplePanel(QWidget *parent = nullptr);
    ~SimplePanel();

    void renderData(void *data);
    void renderFile(QString filepath);
    void setInfo(QString info);
    void showModel(ModelInfo &model);
    void clear();
signals:


private:
//    DataRender *_rgbrender      = nullptr;
//    DataRender *_offrender      = nullptr;
    DataRender *_objrender      = nullptr;
    DataRender *_fbxrender      = nullptr;
    DataRender *_ptsrender      = nullptr;
    QLabel      *_label_info    = nullptr;
    QGridLayout *_gridlayout    = nullptr;
    QSplitter   *_vsplitter     = nullptr;
};

#endif // SIMPLEPANEL_H
