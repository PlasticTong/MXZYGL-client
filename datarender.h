#ifndef DATARENDER_H
#define DATARENDER_H

#include <QWidget>

class DataRender : public QWidget
{
    Q_OBJECT
public:
    explicit DataRender(QWidget *parent = nullptr);
    virtual ~DataRender();

    virtual void renderData(void  *data);
    virtual void renderFile(QString filepath);
    virtual void setTitle(QString title);
    virtual void setInfo(QString info);
    virtual void clear();

signals:

};

#endif // DATARENDER_H
