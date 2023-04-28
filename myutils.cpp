#include "myutils.h"
//#include "pymodule.h"
#include "QTcpSocket"

void readPlainFile(QString file_path, QString &data_str, QString line_delimiter)
{
    qDebug() << "reading file: " << file_path;
    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Can't open the file: "<< file_path;
        return;
    }

    data_str = "";
    QTextStream in(&file);
    if(!in.atEnd()) data_str = in.readLine();
    while (!in.atEnd())
    {
        QString line = in.readLine();
        data_str += (line_delimiter+line);
    }
    qDebug() << "reading file: " << file_path << " done";
}

int readPointFile(const QString &file_path, QByteArray &data)
{
    int len_old = data.size();
    QString filename = file_path;
    filename = filename.replace(".off", ".txt");
    filename = filename.replace(".png", ".txt");
    QFile file(filename);
    qDebug() << "reading file: " << file.fileName();

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Can't open the file: "<< file.fileName();
        return -1;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList str_list= line.split(",");
        if(str_list.length() >= 3)
        {
            data.append(toByteArray(str_list[0].toFloat()));
            data.append(toByteArray(str_list[1].toFloat()));
            data.append(toByteArray(str_list[2].toFloat()));
        }
    }
    qDebug() << "reading file: " << file.fileName() << " done";
    return data.size() - len_old;
}

//void searchModel(QString data_formated, QList<ModelInfo> &models)
//{
//    PyModule myapi("E:/coding/retrieval-3d", "myapi");
//    PyObject* args = Py_BuildValue("s", data_formated.toStdString().c_str());
//    PyObject* pymodels = myapi.call("sayHello", args);
//    int models_count = PyList_Size(pymodels);
//    for(int i = 0; i < models_count; ++i)
//    {
//        PyObject *pymodel = PyList_GetItem(pymodels, i);
//        if(PyDict_Check(pymodel))
//        {
//            char* cstr;
//            ModelInfo model;
//            PyArg_Parse(PyDict_GetItemString(pymodel, "id"),            "i", &model.id);
//            PyArg_Parse(PyDict_GetItemString(pymodel, "category"),      "s", &cstr); model.category = cstr;
//            PyArg_Parse(PyDict_GetItemString(pymodel, "tags"),          "s", &cstr); model.tags = cstr;
//            PyArg_Parse(PyDict_GetItemString(pymodel, "feature"),       "s", &cstr); model.feature = cstr;
//            PyArg_Parse(PyDict_GetItemString(pymodel, "description"),   "s", &cstr); model.description = cstr;
//            PyArg_Parse(PyDict_GetItemString(pymodel, "filepath"),      "s", &cstr); model.filepath = cstr;
//            PyArg_Parse(PyDict_GetItemString(pymodel, "similarity"),    "f", &model.similarity);
//            models.push_back(model);
//        }
//    }
//}

QByteArray intToByteArray(const int i)
{
    QByteArray ba;
    int len_intVar = sizeof(i);
    ba.resize(len_intVar);
    memcpy(ba.data(), &i, len_intVar);
//    ba.resize(4);
//    ba[0] = (uchar)(0x000000ff & i);
//    ba[1] = (uchar)((0x0000ff00&i)>>8);
//    ba[2] = (uchar)((0x00ff0000&i)>>16);
//    ba[3] = (uchar)((0xff000000&i)>>24);
    return ba;
}

int byteArrayToInt(const QByteArray &arr,  Endian endian)
{
    if (arr.size() < 4)
        return 0;

    int res = 0;

    // 小端模式
    if (endian == LittileEndian)
    {
        res = arr.at(0) & 0x000000FF;
        res |= (arr.at(1) << 8) & 0x0000FF00;
        res |= (arr.at(2) << 16) & 0x00FF0000;
        res |= (arr.at(3) << 24) & 0xFF000000;
    }

    // 大端模式
    else if (endian == BigEndian)
    {
        res = (arr.at(0) << 24) & 0xFF000000;
        res |= (arr.at(1) << 16) & 0x00FF0000;
        res |= arr.at(2) << 8 & 0x0000FF00;
        res |= arr.at(3) & 0x000000FF;
    }
    return res;
}
