#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QWidget>
#include <QtWidgets>
#include "glwidget.h"
#include "form.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
class ModelViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ModelViewer(QWidget *parent = nullptr);

signals:

public slots:
    void showOpenFileDialog();
    void showOpenFileDialog1();
    void authorization_replyFinished(QNetworkReply *reply);
    void on_pushbtn_newwin_clicked();

public:
    GLWidget *glwidget = nullptr;
    GLWidget *glwidget1 = nullptr;
    QNetworkAccessManager *network_access_manager_authorization;
    int authorization_code;

};

#endif // MODELVIEWER_H
