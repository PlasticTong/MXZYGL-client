#include "modelupload.h"
#include <thread>



void threadUpload(void *userdata)
{
    ModelUpload *p = (ModelUpload*)userdata;

    // 获取模型
    QJsonObject models = p->_modeltable->getModels();
    QJsonArray titles = models["titles"].toArray();
    QJsonArray values = models["values"].toArray();
    if(0 == values.size())
    {
        QMessageBox::information(p, "模型搜索", "请先打开要搜索的模型文件！");
        return;
    }
    int j = -1;
    for(int k = 0; k < titles.size(); ++k)
    {
        QString title = titles[k].toString();
        if("filepath" == title || "文件路径" == title || "hash" == title || "哈希值" == title)
        {
            j = k;
            break;
        }
    }
    if(-1 == j)
    {
       QMessageBox::information(p, "模型搜索", "无法获取文件数据");
       return;
    }

    // 根据hash值（此处为文件路径）读取文件数据
    QByteArray body;
    QJsonArray block_size;
    for(int i = 0; i < values.size(); ++i)
    {
        QJsonArray value = values[i].toArray();
        QString filename = value[j].toString();
        int len = readPointFile(filename, body);
        if(-1 != len)
            block_size.append(len);
    }

    p->_pushbtn_upload->setEnabled(false);
    p->_pushbtn_upload->setText("模型上传中...");

    models["block_size"] = block_size;
    models["type"] = REQUEST_UPLOADMODEL;     // 请求类型

    // 建立连接
    if(0 != p->_modelclient->connetTo("127.0.0.1", 12345))
    {
        QMessageBox::information(p, "模型上传", "连接服务端失败！");

        p->_pushbtn_upload->setText("上传模型");
        p->_pushbtn_upload->setEnabled(true);

        return;
    }

    // 发送数据
    // 序列化head
    QByteArray head = QJsonDocument(models).toJson();
    p->_modelclient->sendModel(head, body);

    //在槽函数中处理接收数据
}



ModelUpload::ModelUpload(QWidget *parent) : QWidget(parent)
{
    _pushbtn_filedir    = new QPushButton(tr("模型文件"));
    _pushbtn_upload     = new QPushButton(tr("上传模型"));
    _modeltable         = new ModelTable(3, 3);

    _modelclient        = new ModelClient();

//    QHBoxLayout *_hboxlayout = new QHBoxLayout();
//    _hboxlayout->addWidget(_pushbtn_filedir);
//    _hboxlayout->addWidget(_pushbtn_upload);

    QGridLayout *_gridlayout = new QGridLayout();
//    _gridlayout->addLayout(_hboxlayout, 0, 0);
    _gridlayout->addWidget(_pushbtn_filedir);
    _gridlayout->addWidget(_modeltable);
    _gridlayout->addWidget(_pushbtn_upload);
    this->setLayout(_gridlayout);


    connect(_pushbtn_filedir, SIGNAL(clicked()), this, SLOT(on_files_clicked()));
    connect(_pushbtn_upload, SIGNAL(clicked()), this, SLOT(on_upload_clicked()));
    connect(_modelclient, SIGNAL(sgDataRecved(QByteArray, QByteArray)), this, SLOT(doDataRecved(QByteArray, QByteArray)));
}

ModelUpload::~ModelUpload()
{
    delete _modelclient;
    delete _thread_upload;
}

void ModelUpload::on_files_clicked()
{
//    QString filename = QFileDialog::getOpenFileName(this, tr("选择模型文件"), "./off", tr("三维模型文件(*.txt *.off)"));
    QStringList filenames = QFileDialog::getOpenFileNames(this,tr("选择模型文件"),
                                                          tr("./model"),
                                                          tr("模型文件(*.obj;*.fbx;*.txt)"));
    if(filenames.isEmpty()) return;
    qDebug() << filenames;

//    filenames.clear();
//    filenames << "E:/datasets/pairs_10000_2048/1024_piano_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/1024_cup_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/1024_toilet_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/1024_sofa_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/1024_chair_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/1024_airplane_0001.txt";

//    filenames << "E:/datasets/pairs_10000_2048/10000_piano_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/10000_cup_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/10000_toilet_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/10000_sofa_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/10000_chair_0001.txt";
//    filenames << "E:/datasets/pairs_10000_2048/10000_airplane_0001.txt";

    _modeltable->readFiles(filenames);

//    // 读取文件并显示
//    QJsonObject table_data;
//    QJsonArray name, category, tags, description, filepath;
//    QByteArray data;
//    for(auto filename: filenames)
//    {
//        QJsonArray jsModel;
//        QString basename = QFileInfo(filename).baseName();
//        QString genre = basename.split("_")[0];

//        name.append(basename);
//        category.append(genre);
//        tags.append(genre);
//        description.append(genre);
//        filepath.append(filename);
//        //readPointFile(filename, data);
//    }

//    table_data["name"] = name;
//    table_data["category"] = category;
//    table_data["tags"] = tags;
//    table_data["description"] = description;
//    table_data["hash"] = filepath;

//    _modeltable->setModels3(table_data);
}

void ModelUpload::on_upload_clicked()
{
    _pushbtn_upload->setText("模型上传中...");
    _pushbtn_upload->setEnabled(false);
    _pushbtn_filedir->setEnabled(false);
//    if(nullptr == _thread_upload)
//        _thread_upload = new ThreadBase(threadUpload, this);
//    _thread_upload->start();
    // 获取模型
    QJsonObject models = _modeltable->getModels();
    QJsonArray titles = models["titles"].toArray();
    QJsonArray values = models["values"].toArray();
    if(0 == values.size())
    {
        QMessageBox::information(this, "模型搜索", "请先打开要搜索的模型文件！");
        _pushbtn_upload->setText("上传模型");
        _pushbtn_upload->setEnabled(true);
        _pushbtn_filedir->setEnabled(true);
        return;
    }
    int j = -1;
    for(int k = 0; k < titles.size(); ++k)
    {
        QString title = titles[k].toString();
        if("filepath" == title || "文件路径" == title || "hash" == title || "哈希值" == title)
        {
            j = k;
            break;
        }
    }
    if(-1 == j)
    {
       QMessageBox::information(this, "模型搜索", "无法获取文件数据");
       _pushbtn_upload->setText("上传模型");
       _pushbtn_upload->setEnabled(true);
       _pushbtn_filedir->setEnabled(true);
       return;
    }

    // 根据hash值（此处为文件路径）读取文件数据
    QByteArray body;
    QJsonArray block_size;
    for(int i = 0; i < values.size(); ++i)
    {
        QJsonArray value = values[i].toArray();
        QString filename = value[j].toString();
        int len = readPointFile(filename, body);
        if(-1 != len)
            block_size.append(len);
    }

    models["block_size"] = block_size;
    models["type"] = REQUEST_UPLOADMODEL;     // 请求类型
    models["category"] = static_cast<int>(CurrentCategory::category);     // 模型大类

    // 建立连接
    if(0 != _modelclient->connetTo("127.0.0.1", 12345))
    {
        QMessageBox::information(this, "模型上传", "连接服务端失败！");
        _pushbtn_upload->setText("上传模型");
        _pushbtn_upload->setEnabled(true);
        _pushbtn_filedir->setEnabled(true);
        return;
    }

    // 发送数据
    // 序列化head
    QByteArray head = QJsonDocument(models).toJson();
    _modelclient->sendModel(head, body);

    //在槽函数中处理接收数据
}

void ModelUpload::doDataRecved(QByteArray header, QByteArray data)
{
    // 反序列化数据
    QJsonObject jsheader = QJsonDocument::fromJson(header).object();
    if(RESPONSE_ERROR == jsheader["type"].toInt())
    {
        QMessageBox::information(this, "模型上传", "模型上传失败! " + jsheader["status"].toString());
    }
    else
    {
        QMessageBox::information(this, "模型上传", "模型上传成功! ");
        _modeltable->clear(); // 上传成功后清空表格
    }

    // 断开连接
    _modelclient->close();

    _pushbtn_upload->setEnabled(true);
    _pushbtn_filedir->setEnabled(true);
    _pushbtn_upload->setText("上传模型");
}
