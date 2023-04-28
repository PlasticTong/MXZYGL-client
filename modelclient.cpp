#include "myutils.h"
#include "modelclient.h"

ModelClient::ModelClient(QObject *parent) : QObject(parent)
{
    _clientSocket = new QTcpSocket();
    connect(_clientSocket, SIGNAL(readyRead()), this, SLOT(doReadyRead()));
}

ModelClient::~ModelClient()
{
    delete _clientSocket;
}

int ModelClient::connetTo(const QString &ip, quint16 port)
{
    // 建立连接
    if(QTcpSocket::UnconnectedState != _clientSocket->state())
    {
        _clientSocket->close();
        //_clientSocket->waitForDisconnected(3000);
    }
    _clientSocket->connectToHost(ip, port);
    if(!_clientSocket->waitForConnected(3000))
    {
        qDebug() << "连接服务端失败！";
        return -1;
    }
    return 0;
}

int ModelClient::disconnet()
{
    return close();
}

int ModelClient::close()
{
    if(QTcpSocket::UnconnectedState != _clientSocket->state())
    {
        _clientSocket->close();
        _clientSocket->waitForDisconnected(3000);
    }
    return 0;
}

int ModelClient::sendData(const QByteArray &data)
{
    // 先发送数据长度
    uint len = data.size();
    _clientSocket->write(toByteArray(len));
    // 再发送真正的数据
    qDebug() << "length to send " << len;
    int len_sended = 0;
    while(len_sended < len)
    {
        int ret = _clientSocket->write(data);
        if(-1 == ret)
        {
            qDebug() << "向服务端发送数据失败！";
            return -1;
        }
        len_sended += ret;
        _clientSocket->flush();
    }
    qDebug() << "length sended " << len_sended;

    return 0;
}

int ModelClient::recvData(QByteArray &data)
{
    qDebug() << "available bytes" << _clientSocket->bytesAvailable();
    if (!_isLengthRecved) // 首先读取数据长度信息
    {
        if (_clientSocket->bytesAvailable() >= 8) // 不能小于头字节数
        {
            // 读取头信息
            QByteArray head_len_byte, body_len_byte;
            head_len_byte = _clientSocket->read(4);
            body_len_byte = _clientSocket->read(4);
            _head_len = fromByteArray<uint>(head_len_byte);
            _body_len = fromByteArray<uint>(body_len_byte);
            qDebug() << "length to receive " << _head_len << _body_len;
            _head.clear(); // 清空缓存，准备接收数据
            _body.clear(); // 清空缓存，准备接收数据
            if(0 == _head_len && 0 == _body_len) return 0; // 没有后续数据

            // 读取头信息并标注头信息已经被读取
            _isLengthRecved = true;
        }

        if(!_isLengthRecved) return -1;
    }

    // 读取实际的数据
    if(_head.size() < _head_len)
    {
        _head.append(_clientSocket->read(_head_len-_head.size()));
    }
    else if(_body.size() < _body_len)
    {
        _body.append(_clientSocket->read(_body_len-_body.size()));
    }

    qDebug() << "length received " << _head.size() << _body.size();
    if (_head.size() < _head_len || _body.size() < _body_len) return -1;  // 如果没有读完的话，还会再次进入槽函数

    // 复位
    _isLengthRecved = false;
    _head_len = 0;
    _body_len = 0;
    return 0;
}

int ModelClient::sendModel(const QByteArray &head, const QByteArray &body)
{
    // 先发送数据长度
    uint head_len = head.size(), body_len = body.size();
    _clientSocket->write(toByteArray(head_len) + toByteArray(body_len));
    // 再发送真正的数据

    uint len_sended = 0, len = head_len + body_len;
    qDebug() << "length to send " << len;
    while(len_sended < len)
    {
        int ret = -1;
        if(len_sended < head_len)
        {
            ret = _clientSocket->write(head.data()+len_sended, head_len-len_sended);
        }
        else
        {
            ret = _clientSocket->write(body.data()+len_sended-head_len, len - len_sended);
        }

        if(-1 == ret)
        {
            qDebug() << "向服务端发送数据失败！";
            return -1;
        }
        len_sended += ret;
        _clientSocket->flush();
        qDebug() << "length sended " << len_sended;
    }

    return 0;

//    // 发送header
//    int ret = this->sendData(header);
//    if(0 != ret) return ret;
//    // 发送body
//    ret = this->sendData(body);
//    if(0 != ret) return ret;
//    return 0;
}

int ModelClient::recvModel(QByteArray &header, QByteArray &body)
{
    qDebug() << "available bytes" << _clientSocket->bytesAvailable();
    if (!_isLengthRecved) // 首先读取数据长度信息
    {
        if (_clientSocket->bytesAvailable() >= 8) // 不能小于头字节数
        {
            // 读取头信息
            QByteArray head_len_byte, body_len_byte;
            head_len_byte = _clientSocket->read(4);
            body_len_byte = _clientSocket->read(4);
            _head_len = fromByteArray<uint>(head_len_byte);
            _body_len = fromByteArray<uint>(body_len_byte);
            qDebug() << "length to receive " << _head_len << _body_len;
            _head.clear(); // 清空缓存，准备接收数据
            _body.clear(); // 清空缓存，准备接收数据
            if(0 == _head_len && 0 == _body_len) return 0; // 没有后续数据

            // 读取头信息并标注头信息已经被读取
            _isLengthRecved = true;
        }

        if(!_isLengthRecved) return -1;
    }

    // 读取实际的数据
    if(_head.size() < _head_len)
    {
        _head.append(_clientSocket->read(_head_len-_head.size()));
    }
    else if(_body.size() < _body_len)
    {
        _body.append(_clientSocket->read(_body_len-_body.size()));
    }

    qDebug() << "length received " << _head.size() << _body.size();
    if (_head.size() < _head_len || _body.size() < _body_len) return -1;  // 如果没有读完的话，还会再次进入槽函数

    // 复位
    _isLengthRecved = false;
    _head_len = 0;
    _body_len = 0;
    return 0;
}

void ModelClient::doReadyRead()
{
    // 读取返回结果
    if(0 != recvModel(_head, _body)) return;
    // 通知数据准备好了
    emit sgDataRecved(_head, _body);
    _head.clear();
    _body.clear();
}
