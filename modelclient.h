#ifndef MODELCLIENT_H
#define MODELCLIENT_H
#include <QtWidgets>
#include <QTcpSocket>
#include "mydefine.h"

class ModelClient : public QObject
{
    Q_OBJECT
public:
    explicit ModelClient(QObject *parent=nullptr);
    ~ModelClient();
    int connetTo(const QString &ip="127.0.0.1", quint16 port=12345);
    int disconnet();
    int close();
    int sendData(const QByteArray &data);
    int recvData(QByteArray &data);
    int sendModel(const QByteArray &header, const QByteArray &body);
    int recvModel(QByteArray &header, QByteArray &body);

signals:
    void sgDataRecved(QByteArray, QByteArray);

public slots:
    void doReadyRead();

private:

    //通信服务
    QTcpSocket  *_clientSocket  = nullptr;
    bool        _isLengthRecved = false;
    bool        _isHeadRecved   = false;
    bool        _isBodyRecved   = false;
    uint        _lentorecv = 0;
    uint        _head_len = 0;
    uint        _body_len = 0;
    QByteArray  _head;
    QByteArray  _body;
};

#endif // MODELCLIENT_H
