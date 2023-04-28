#ifndef MYUTILS_H
#define MYUTILS_H

#include <string>
#include <QtDataVisualization>
#include "mydefine.h"


using namespace QtDataVisualization;

void readPlainFile(QString file_path, QString &data_str, QString line_delimiter=" ");
int readPointFile(const QString &file_path, QByteArray &data); // 返回格式化的字符串数据
//void searchModel(QString data_formated, QList<ModelInfo> &models);
void getFeature(QVector<QVector3D> &data, QVector<float> &feature);
QByteArray intToByteArray(const int i);
int byteArrayToInt(const QByteArray &arr,  Endian endian = LittileEndian);

template <typename T>
QByteArray toByteArray(const T &var)
{
    QByteArray array;
    int len_var = sizeof(var);
    array.resize(len_var);
    memcpy(array.data(), &var, len_var);
    return array;
}

template <typename T>
T fromByteArray(const QByteArray &array)
{
    T var;
    memcpy(&var, array.data(), std::min((uint)array.size(), (uint)sizeof(var)));
    return var;
}

#endif // MYUTILS_H
