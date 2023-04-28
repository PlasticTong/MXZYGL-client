#include "modeltable.h"

ModelTable::ModelTable(uint panel_rows, uint panel_cols, QWidget *parent) : QWidget(parent)
{
    _tablewidget = new QTableWidget();
    _multipanel  = new MultiPanel(panel_rows, panel_cols);

    QSplitter *_vsplitter = new QSplitter(Qt::Vertical);
    _vsplitter->addWidget(_multipanel);
    _vsplitter->addWidget(_tablewidget);
//    设置大小
    QList<int> sizes;
    sizes <<_vsplitter->height() / 2 << _vsplitter->height() / 2;
    _vsplitter->setSizes(sizes);
//    _vsplitter->setStretchFactor(0,2);
//    _vsplitter->setStretchFactor(1,2);
    QGridLayout *_gridlayout = new QGridLayout();
    _gridlayout->addWidget(_vsplitter);
    this->setLayout(_gridlayout);



//    void cellActivated(int row, int column);
//    void cellEntered(int row, int column);
//    void cellChanged(int row, int column);

//    void currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

//    void itemPressed(QTableWidgetItem *item);
//    void itemClicked(QTableWidgetItem *item);
//    void itemDoubleClicked(QTableWidgetItem *item);

//    void itemActivated(QTableWidgetItem *item);
//    void itemEntered(QTableWidgetItem *item);


//    connect(_tablewidget, &QTableWidget::currentCellChanged,
//            [](int cr,int cc, int pr, int pc)
//            {
//                qDebug() << "previous cell:" <<pr<<", "<<pc<<"; current cell: "<<cr<<", "<<cc;
//            });

    connect(_tablewidget, &QTableWidget::itemChanged,
            [this](QTableWidgetItem* cur)
            {
                auto newText = cur->text();
                auto R = cur->row();
                auto C = cur->column();
                if (C <= 0 )
                    return;
                auto vals = this->_models["values"].toArray();
                auto row = vals[R].toArray();
                auto preText = row[C-1].toString();
                row[C-1] = newText;
                vals[R] = row;
                this->_models["values"]=vals;
                qDebug() << "QTableWidget::itemChanged, "
                         << "from" << preText
                         << "to" << newText
                         << ", in table[" << R <<", " << C << "]";
            });

    connect(_tablewidget, SIGNAL(itemSelectionChanged()), this, SLOT(doItemSelectionChanged()));
    connect(_tablewidget->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(doVerticalSectionClicked(int)));
    connect(_tablewidget->verticalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(doVerticalSectionDoubleClicked(int)));
}

void ModelTable::doItemSelectionChanged()
{
    qDebug() << "doitemSelectionChanged" << _tablewidget->selectedItems();

    for(auto item: _tablewidget->selectedItems())
    {
        if(0 == _tablewidget->column(item))
        {
            QJsonArray titles_ = _models["titles"].toArray();
            QStringList titles;
            for(auto t: titles_)
                titles << t.toString();
            int j = titles.indexOf("filepath");
            if(-1 == j) j = titles.indexOf("文件路径");            if(-1 == j) j = titles.indexOf("文件路径");
            if(-1 == j) j = titles.indexOf("hash");
            if(-1 == j) j = titles.indexOf("哈希值");
            if(-1 == j) return;
            ++j; // 跳过第一列（用于渲染选择）

            int s = titles.indexOf("similarity");
            if(-1 == s) s = titles.indexOf("相似度");            if(-1 == j) j = titles.indexOf("文件路径");

            //渲染模型
            int i = _tablewidget->row(item);
            int span = _tablewidget->rowSpan(i, 0);
            int num_rows = _multipanel->rows();
            int num_cols = _multipanel->cols();

            for(int r = 0, k = 0; r < num_rows; ++r)
            {
                for(int c = 0; c < num_cols; ++c, ++k, ++i)
                {
                    if(k < span)
                    {
                        QString filepath = _tablewidget->item(i, j)->text();
                        _multipanel->renderFile(filepath, r, c);
                        QString txtInfo = QString::asprintf("%d", i+1);
                        txtInfo += " 类别:" + _tablewidget->item(i, 3)->text();
                        if(-1 != s) txtInfo += " 相似度:" + _tablewidget->item(i, s+1)->text();
                        _multipanel->setInfoAt(txtInfo, r, c);
                    }
                    else
                    {
                        _multipanel->clearAt(r, c);
                    }
                }
            }

            break;
        }
    }
}

void ModelTable::doVerticalSectionClicked(int idx)
{
    qDebug() << "doVerticalSectionClicked " << idx;
    // 获取模型
    // 渲染模型
}

void ModelTable::doVerticalSectionDoubleClicked(int idx)
{
    qDebug() << "doVerticalSectionDoubleClicked " << idx;
}

void ModelTable::setModels(QList<ModelInfo> &models)
{
    //_tablewidget->clearContents(); //只清除工作区，不清除表头
    int i = 0;
    for(; i < models.size(); ++i)
    {
        // 插入表格
        if(i >= _tablewidget->rowCount())
        {
            _tablewidget->insertRow(i);
        }

        //_tablewidget->setCellWidget(i, 0, _renders[i]); _renders[i]->renderData(&models[i].data);
        _tablewidget->setItem(i, 1, new QTableWidgetItem(models[i].name));
        _tablewidget->setItem(i, 2, new QTableWidgetItem(models[i].category));
        _tablewidget->setItem(i, 3, new QTableWidgetItem(models[i].tags));
        _tablewidget->setItem(i, 4, new QTableWidgetItem(models[i].description));
        _tablewidget->setItem(i, 5, new QTableWidgetItem(models[i].other));

        _tablewidget->setRowHidden(i, false);
    }

    for(; i < _tablewidget->rowCount(); ++i)
    {
        _tablewidget->setRowHidden(i, true);
    }

    for(int i = 0; i < models.size(); i += _multipanel->size())
    {
        _tablewidget->setItem(i, 0, new QTableWidgetItem("模型预览"));
        int row_span = std::min(_multipanel->size(), models.size()-i);
        _tablewidget->setSpan(i, 0, row_span, 1);
    }

    // 默认渲染第一个模型
    if(models.size() > 0)
        _tablewidget->setItemSelected(_tablewidget->item(0, 0), true);
}

void ModelTable::setModels3(const QJsonObject &table_data)
{
    _tablewidget->clear();
    _table_data = table_data;
    QStringList keys = _table_data.keys();
    if(0 == keys.size()) return;
    // 设置表头
    QStringList titles;
    titles << "preview" << keys;
    int num_cols = titles.size();
    if(0 == num_cols) return;
    _tablewidget->setColumnCount(num_cols);
    _tablewidget->setHorizontalHeaderLabels(titles);
    _tablewidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _tablewidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _tablewidget->horizontalHeader()->setSectionResizeMode(num_cols-1, QHeaderView::Stretch);

    int num_rows = table_data[keys[0]].toArray().size();
    _tablewidget->setRowCount(num_rows);
    for(int j = 1; j < num_cols; ++j)
    {
        QString key = titles[j];
        QJsonArray val = _table_data[key].toArray();
//        if("id" == key.toLower() || "编号" == key)
//            for(int i = 0; i < num_rows; ++i)
//                _tablewidget->setItem(i, j, new QTableWidgetItem(val[i].toInt()));
//        else if("similarity" == titles[j] || "相似度" == titles[j])
//            for(int i = 0; i < num_rows; ++i)
//                _tablewidget->setItem(i, j, new QTableWidgetItem(val[i].toDouble()));
//        else
        for(int i = 0; i < num_rows; ++i)
            _tablewidget->setItem(i, j, new QTableWidgetItem(val[i].toString()));
    }

    for(int i = 0; i < num_rows; i += _multipanel->size())
    {
        _tablewidget->setItem(i, 0, new QTableWidgetItem("模型预览"));
        int row_span = std::min(_multipanel->size(), num_rows-i);
        _tablewidget->setSpan(i, 0, row_span, 1);
    }

    // 默认渲染第一组模型
    if(num_rows > 0)
        _tablewidget->setItemSelected(_tablewidget->item(0, 0), true);
}

void ModelTable::setModels4(const QJsonObject &models)
{
    _models = models;
    this->refresh();
}

void ModelTable::readFiles(const QStringList &filenames)
{
    // 读取文件并显示
    _models["titles"] = QJsonArray({"name", "type", "tags", "description", "hash"});
    QJsonArray values = QJsonArray();

    QByteArray data;
    for(auto filename: filenames)
    {
        QString basename = QFileInfo(filename).baseName();
        QString genre = basename.split("_")[0];
        QJsonArray model;
        model.append(basename);
        model.append(genre);
        model.append(genre);
        model.append(genre);
        model.append(filename);
        //readPointFile(filename, data);
        values.append(model);
    }

    _models["values"] = values;

    this->refresh();
}

void ModelTable::refresh()
{
    _tablewidget->clear();
    QStringList titles;
    for(auto title: _models["titles"].toArray())
    {
        titles.append(title.toString());
    }
    int num_cols = titles.size();
    if(0 == num_cols) return;
    // 设置表头
    QStringList titles_;
    titles_ << "preview" << titles;
    qDebug() << "titles"<< titles.size() << "titles_" << titles_.size();
    _tablewidget->setColumnCount(titles_.size());
    _tablewidget->setHorizontalHeaderLabels(titles_);
    _tablewidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _tablewidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _tablewidget->horizontalHeader()->setSectionResizeMode(titles_.size()-1, QHeaderView::Stretch);

    QJsonArray rows = _models["values"].toArray();
    int num_rows = rows.size();
    if(0 == num_rows) return;
    // 设置表内容
    _tablewidget->setRowCount(num_rows);
    for(int i = 0; i < num_rows; ++i)
    {
        QJsonArray row = rows[i].toArray();
        for(int j = 0; j < num_cols; ++j)
        {
//            if("id" == titles[j].toLower() || "编号" == titles[j])
//                _tablewidget->setItem(i, 1+j, new QTableWidgetItem(row[j].toString()));
//            else if("similarity" == titles[j].toLower() || "相似度" == titles[j])
//                _tablewidget->setItem(i, 1+j, new QTableWidgetItem(row[j].toString()));
//            else
            _tablewidget->setItem(i, 1+j, new QTableWidgetItem(row[j].toString()));
        }
    }

    if(_multipanel->size() > 0)
    {
        for(int i = 0; i < num_rows; i += _multipanel->size())
        {
            _tablewidget->setItem(i, 0, new QTableWidgetItem("模型预览"));
            int row_span = std::min(_multipanel->size(), num_rows-i);
            _tablewidget->setSpan(i, 0, row_span, 1);
        }

        // 默认渲染第一组模型
        if(num_rows > 0)
            _tablewidget->setItemSelected(_tablewidget->item(0, 0), true);
    }
}

TableJson ModelTable::getModels()
{
//    QJsonObject models;
//    // 获取表格内容
//    QList<QJsonArray> col_contents;
//    for(int j = 1; j < _tablewidget->horizontalHeader()->count(); ++j)
//    {
//        col_contents.append(QJsonArray());
//    }
//    for(int j = 1; j < _tablewidget->horizontalHeader()->count(); ++j)
//    {
//        QJsonArray val_col;
//        for(int i = 0; i < _tablewidget->rowCount(); ++i)
//        {
//            QString text = (nullptr != _tablewidget->item(i, j)) ? _tablewidget->horizontalHeaderItem(j)->text() : "";
//            val_col.append(text);
//        }
//        QString title = _tablewidget->horizontalHeaderItem(j)->text();
//        models[title] = val_col;
//    }

    return _models;
}

void ModelTable::clear()
{
    while(_tablewidget->columnCount() > 0)
    {
        _tablewidget->removeColumn(0);
    }
//    for(int i = 0; i < _tablewidget->rowCount(); ++i)
//    {
//        _tablewidget->setRowHidden(i, true);
//    }
    _table_data = QJsonObject();
    _multipanel->clear();
}
