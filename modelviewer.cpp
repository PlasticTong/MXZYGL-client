#include "modelviewer.h"
#include "modelsearch.h"
#include "modelupload.h"
#include "modelsearchbylabel.h"
#include "modelsearchbytype.h"
#include "mydefine.h"

ModelCategory CurrentCategory::category = ModelCategory::OUTFIT;

ModelViewer::ModelViewer(QWidget *parent) : QWidget(parent)
{
//    授權
    network_access_manager_authorization = new QNetworkAccessManager(this);
     connect(network_access_manager_authorization, SIGNAL(finished(QNetworkReply*)), this, SLOT(authorization_replyFinished(QNetworkReply*)));
    QUrl authorization_url("http://localhost:9722/auth?appcode=APP1601&pincode=1234567890");
    QNetworkRequest authorization_request(authorization_url);
    QNetworkReply *authorization_reply = network_access_manager_authorization->get(authorization_request);


    QButtonGroup* bt_group = new QButtonGroup();
    QGridLayout *_gridlayout = new QGridLayout(this);

    auto _pushbtn_newwin   = new QPushButton(tr("索引管理"));
    auto *_hboxlayout_filepath = new QHBoxLayout();
    _hboxlayout_filepath->addWidget(_pushbtn_newwin );
    _gridlayout->addLayout(_hboxlayout_filepath,0,0);

    auto rbt_1 = new QRadioButton(tr("主战装备"));
    auto rbt_2 = new QRadioButton(tr("人员"));
    auto rbt_3 = new QRadioButton(tr("地物"));
    auto rbt_4 = new QRadioButton(tr("建筑"));
    auto _hboxly = new QHBoxLayout();

    _hboxly->addWidget(rbt_1);
    _hboxly->addWidget(rbt_2);
    _hboxly->addWidget(rbt_3);
    _hboxly->addWidget(rbt_4);

    bt_group->addButton(rbt_1, ModelCategory::OUTFIT);
    bt_group->addButton(rbt_2, ModelCategory::STAFF);
    bt_group->addButton(rbt_3, ModelCategory::GROUND);
    bt_group->addButton(rbt_4, ModelCategory::BUILDING);
    _gridlayout->addLayout(_hboxly,1,0);

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(new ModelSearch(this), tr("模型内容搜索"));
    tabWidget->addTab(new ModelUpload(this), tr("模型上传"));
    tabWidget->addTab(new modelsearchbylabel(this), tr("模型标签搜索"));
    tabWidget->addTab(new modelsearchbytype(this), tr("模型类型搜索"));
    _gridlayout->addWidget(tabWidget);

//    QPushButton *btn = new QPushButton(this);
//    glwidget = new GLWidget(this);
//    QObject::connect(btn, SIGNAL(clicked()), this, SLOT(showOpenFileDialog()));

//    QPushButton *btn1 = new QPushButton(this);
//    glwidget1 = new GLWidget(this);
//    QObject::connect(btn1, SIGNAL(clicked()), this, SLOT(showOpenFileDialog1()));
//    _gridlayout->addWidget(btn);
//    _gridlayout->addWidget(glwidget);
//    _gridlayout->addWidget(btn1);
//    _gridlayout->addWidget(glwidget1);

    this->setLayout(_gridlayout);

    connect(bt_group, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
                     [=](QAbstractButton*bt){
        auto id=bt_group->id(bt);
        CurrentCategory::category = static_cast<ModelCategory>(id);
        switch (id) {
        case ModelCategory::OUTFIT:      qDebug() << "zzzb"; break;
        case ModelCategory::STAFF:       qDebug() << "ry"; break;
        case ModelCategory::GROUND:      qDebug() << "dw"; break;
        case ModelCategory::BUILDING:    qDebug() << "jz"; break;
        }
    });

    rbt_1->click();
    QObject::connect(_pushbtn_newwin, SIGNAL(clicked()), this, SLOT(on_pushbtn_newwin_clicked()));


}
void ModelViewer::on_pushbtn_newwin_clicked(){
    
    Form *w = new Form;
    w->show();
    
}
void ModelViewer::authorization_replyFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull()) {
        qDebug() << "Failed to create JSON doc.";
        QMessageBox::about(NULL,"错误","鉴权错误！");
        QCoreApplication::exit(1);
        return;
    }

    if (!doc.isObject()) {
        qDebug() << "JSON is not an object.";
        QMessageBox::about(NULL,"错误","鉴权错误！");
        QCoreApplication::exit(1);
        return;
    }

    QJsonObject obj = doc.object();
    if (obj.isEmpty()) {
        qDebug() << "JSON object is empty.";
        QMessageBox::about(NULL,"错误","鉴权错误！");
        QCoreApplication::exit(1);
        return;
    }

    if (obj.contains("code") && obj["code"] == 0){
        qDebug() << "authorization code is: " << QString::number(obj["code"].toInt());
        authorization_code = 0;
        return;
    }
    else{
        qDebug() << "authorization code is: " << QString::number(obj["code"].toInt());
        QMessageBox::about(NULL,"错误","未获授权！");
        QCoreApplication::exit(1);
        return;
    }
}

void ModelViewer::showOpenFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open File",
        "../opengl-3d-visualizer/off",
        QString("OFF Files (*.off)"),
        nullptr,
        QFileDialog::DontUseNativeDialog
    );

    if (!fileName.isEmpty())
    {
        glwidget->renderOffFile(fileName);
    }
}

void ModelViewer::showOpenFileDialog1()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open File",
        "../opengl-3d-visualizer/off",
        QString("OFF Files (*.off)"),
        nullptr,
        QFileDialog::DontUseNativeDialog
    );

    if (!fileName.isEmpty())
    {
        glwidget1->renderOffFile(fileName);
    }
}
