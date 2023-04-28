#ifndef MODELSEARCH_H
#define MODELSEARCH_H

#include <QWidget>
#include <QtWidgets>
#include "pointrender.h"
#include "multipanel.h"
#include "QTcpSocket"
#include "modeltable.h"
#include "modelclient.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
class ModelSearch : public QWidget
{
    Q_OBJECT
public:
    explicit ModelSearch(QWidget *parent = nullptr);

signals:

private slots:

    void on_pushbtn_filepath_clicked();

    void on_pushbtn_search_clicked();

    void on_lineedit_filepath_editingFinished();

    void on_pushbtn_prev_clicked();

    void on_pushbtn_next_clicked();

    void doDataRecved(QByteArray head, QByteArray body);

private:
    QString _filepath;

    QTabWidget *tabWidget;

    QLineEdit   *_lineedit_filepath = nullptr;
    QPushButton *_pushbtn_filepath  = nullptr;
    QPushButton *_pushbtn_search    = nullptr;
    MultiPanel  *_multipanel_model  = nullptr;
    ModelTable  *_modeltable_model  = nullptr;
    ModelTable  *_modeltable_models = nullptr;
    ModelClient *_modelclient       = nullptr;

    PointRender *_pointrender_model = nullptr;

    //通信服务
    QTcpSocket      *_clientSocket    = nullptr;
    bool            isTcpRecvHeadOk = false;
    QByteArray      tcpRecvBlock;
    unsigned int    _lentorecv = 0;
    QString         _current_category = "Unkown";

};

#endif // MODELSEARCH_H
