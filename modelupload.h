#ifndef MODELUPLOAD_H
#define MODELUPLOAD_H

#include <QWidget>
#include <QtWidgets>
#include "multipanel.h"
#include "modeltable.h"
#include "modelclient.h"
#include "threadbase.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
class ModelUpload : public QWidget
{
    Q_OBJECT
public:
    explicit ModelUpload(QWidget *parent = nullptr);
    ~ModelUpload();

signals:

public slots:
    void on_files_clicked();
    void on_upload_clicked();
    void doDataRecved(QByteArray header, QByteArray data);
    

public:
    QPushButton *_pushbtn_filedir   = nullptr;
    QPushButton *_pushbtn_upload    = nullptr;
    ModelTable  *_modeltable        = nullptr;
    ModelClient *_modelclient       = nullptr;
    ThreadBase  *_thread_upload     = nullptr;
};

#endif // MODELUPLOAD_H
