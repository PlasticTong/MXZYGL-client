#include "modelsearchbytype.h"

modelsearchbytype::modelsearchbytype(QWidget *parent) : QWidget(parent)
{
    _lineedit_filepath = new QLineEdit();
//    _lineedit_filepath->setText("请输入模型标签");
//    _pushbtn_filepath   = new QPushButton(tr("模型文件"));
//    _pushbtn_label   = new QPushButton(tr("模型标签"));
    _pushbtn_search     = new QPushButton(tr("搜索"));
//    _modeltable_model   = new ModelTable(1, 1);
    _modeltable_models  = new ModelTable(3, 3);
    _modelclient        = new ModelClient();


    QHBoxLayout *_hboxlayout_filepath = new QHBoxLayout();
//    _hboxlayout_filepath->addWidget(_pushbtn_filepath);
    _hboxlayout_filepath->addWidget(_lineedit_filepath);
    // _hboxlayout_filepath->addWidget(_pushbtn_search);

    QWidget *_widget_filepath = new QWidget();
    _widget_filepath->setLayout(_hboxlayout_filepath);
    _widget_filepath->setMaximumHeight(64);

    // _pointrender_model = new PointRender();
    QHBoxLayout *vboxlayout_modeltosearch = new QHBoxLayout();
    vboxlayout_modeltosearch->addWidget(_widget_filepath);
//    vboxlayout_modeltosearch->addWidget(_modeltable_model);
    vboxlayout_modeltosearch->addWidget(_pushbtn_search);

    QGroupBox *_groupbox_modeltosearch = new QGroupBox(tr("搜索模型"));
//    _groupbox_modeltosearch->setGeometry(100, 100, 200, 50);
    _groupbox_modeltosearch->setLayout(vboxlayout_modeltosearch);

    QVBoxLayout *_vboxlayout_models  = new QVBoxLayout();
    _vboxlayout_models->addWidget(_modeltable_models);

    QGroupBox *_groupbox_modelssearched = new QGroupBox(tr("搜索结果"));
    _groupbox_modelssearched->setLayout(_vboxlayout_models);

    QSplitter *hsplitter_all = new QSplitter(Qt::Vertical);
    hsplitter_all->addWidget(_groupbox_modeltosearch);
    hsplitter_all->addWidget(_groupbox_modelssearched);

    QGridLayout *_gridlayout_all = new QGridLayout();
    _gridlayout_all->addWidget(hsplitter_all);

    this->setLayout(_gridlayout_all);

    // QTableWidgetItem    *headerItem;
    // for (int i = 0;i < _table_models->columnCount(); ++i)//列编号从0开始
    // {
    //     //cellItem=ui->tableInfo->horizontalHeaderItem(i);
    //     headerItem=new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
    //     QFont font=headerItem->font();//获取原有字体设置
    //     font.setBold(true);//设置为粗体
    //     font.setPointSize(12);//字体大小
    //     headerItem->setTextColor(Qt::red);//字体颜色
    //     headerItem->setFont(font);//设置字体
    //     _table_models->setHorizontalHeaderItem(i, headerItem); //设置表头单元格的Item
    // }

    this->setWindowTitle(tr("三维模型检索"));

//    QObject::connect(_lineedit_filepath, SIGNAL(editingFinished()), this, SLOT(on_lineedit_filepath_editingFinished()));
//    QObject::connect(_pushbtn_filepath, SIGNAL(clicked()), this, SLOT(on_pushbtn_filepath_clicked()));
    QObject::connect(_pushbtn_search, SIGNAL(clicked()), this, SLOT(on_pushbtn_search_clicked()));

    // socket通信
    _clientSocket = new QTcpSocket();
    connect(_modelclient, SIGNAL(sgDataRecved(QByteArray, QByteArray)), this, SLOT(doDataRecved(QByteArray, QByteArray)));

}

//void modelsearchbytype::on_pushbtn_filepath_clicked()
//{
////    QString root_dir = QFileDialog::getExistingDirectory(this, tr("Select the directory including 3D data files."));
////    QString filepath = QFileDialog::getOpenFileName(this, tr("打开模型文件"), "./off", tr("三维模型文件(*.*)"));
//    QString text = QInputDialog::getText(this, tr("输入文本"), tr("请输入文本："));
////    QString filepath = QFileDialog::getOpenFileName(this, tr("打开模型文件"), "./off", tr("三维模型文件(*.*)"), nullptr, QFileDialog::DontUseNativeDialog);
//    if(text.isEmpty()) return;
//    _lineedit_filepath->setText(text);
//    qDebug() << text;
//    _lineedit_filepath->editingFinished();
//}

//void modelsearchbytype::on_lineedit_filepath_editingFinished()
//{
//    QString text = QInputDialog::getText(this, tr("输入文本"), tr("请输入文本："));
//    if(_filepath != _lineedit_filepath->text())
//    {
//        QStringList _filenames(_lineedit_filepath->text());
//        _modeltable_model->readFiles(_filenames);

//        QString basename = QFileInfo(_filenames[0]).baseName();
//        _current_category = basename.split("_")[0];
//    }
//}

void modelsearchbytype::on_pushbtn_search_clicked()
{
    qDebug() << _lineedit_filepath->text();
//    // 获取模型
//    QJsonObject models = _modeltable_model->getModels();
//    QJsonArray titles = models["titles"].toArray();
//    QJsonArray values = models["values"].toArray();
//    if(0 == values.size())
//    {
//        QMessageBox::information(this, "模型搜索", "请先打开要搜索的模型文件！");
//        return;
//    }
//    int j = -1;
//    for(int k = 0; k < titles.size(); ++k)
//    {
//        QString title = titles[k].toString();
//        if("filepath" == title || "文件路径" == title || "hash" == title || "哈希值" == title)
//        {
//            j = k;
//            break;
//        }
//    }
//    if(-1 == j)
//    {
//       QMessageBox::information(this, "模型搜索", "无法获取文件数据");
//       return;
//    }
//    // 根据hash值（此处为文件路径）读取文件数据
//    QByteArray body;
//    QJsonArray block_size;
//    for(int i = 0; i < values.size(); ++i)
//    {
//        QJsonArray value = values[i].toArray();
//        QString filename = value[j].toString();

//        int len = readPointFile(filename, body);
//        if(-1 != len)
//            block_size.append(len);
//    }

    QJsonObject models;
    QByteArray body = _lineedit_filepath->text().toUtf8(); // 使用toUtf8()函数
    models["block_size"] = _lineedit_filepath->text();
    models["type"] = REQUEST_SEARCHTYPE;     // 请求类型
    models["category"] = static_cast<int>(CurrentCategory::category);     // 模型大类

    // 建立连接
    if(0 != _modelclient->connetTo("127.0.0.1", 12345))
    {
        QMessageBox::information(this, "模型搜索", "连接服务端失败！");
        return;
    }

    // 发送数据
    // 序列化head
    QByteArray data_header = QJsonDocument(models).toJson();
    _modelclient->sendModel(data_header, body);

//    // 记录当前检索的模型
//    int col_category = -1;
//    int c = 0;
//    for(auto title: models["titles"].toArray())
//    {
//        ++c;
//        if("category" == title.toString() || "类别" == title.toString())
//        {
//            col_category = c;
//            break;
//        }
//    }
//    if(-1 != col_category)
//        _current_category = models["values"].toArray()[0].toArray()[col_category].toString();
//    else _current_category = "Unkown";

    //在槽函数中处理接收数据
}

void modelsearchbytype::doDataRecved(QByteArray head, QByteArray body)
{
    // 反序列化数据
    QJsonObject jshead = QJsonDocument::fromJson(head).object();
    if(RESPONSE_ERROR == jshead["type"].toInt())
    {
        QMessageBox::information(this, "模型搜索", "模型搜索失败! " + jshead["status"].toString());
    }
    else
    {
        // 断开连接
        _modelclient->close();

        // 显示结果
        QJsonObject _models = jshead["models"].toObject();
        _modeltable_models->setModels4(jshead["models"].toObject());

        // 计算top3 和 top10 准确度

        int col_category = -1;
        int j = 0;
        for(auto title: _models["titles"].toArray())
        {
            ++j;
            if("category" == title.toString() || "类别" == title.toString())
            {
                col_category = j;
                break;
            }
        }
        if(-1 != col_category)
        {
            float accuracy_top3 = .0, accuracy_top10 = .0;
            uint correct_top3 = 0, correct_top10 = 0;
            QJsonArray values = _models["values"].toArray();
            for(int i = 0; i < 10 && i < values.size(); ++i)
            {
                if(_current_category == values[i].toArray()[col_category].toString())
                {
                    if(i < 3) ++correct_top3;
                    if(i < 10) ++correct_top10;
                }
            }
            accuracy_top3 = correct_top3/3.0;
            accuracy_top10 = correct_top10/10.0;
            qDebug() << "accuracy_top3" << accuracy_top3 << "(" << correct_top3 << ")";
            qDebug() << "accuracy_top10" << accuracy_top10 << "(" << correct_top10 << ")";
        }

        QMessageBox::information(this, "模型搜索", "模型搜索成功! ");
    }
}

void modelsearchbytype::on_pushbtn_prev_clicked()
{
    qDebug() << "to prev page.";
}

void modelsearchbytype::on_pushbtn_next_clicked()
{
}
