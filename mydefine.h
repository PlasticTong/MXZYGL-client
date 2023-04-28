#ifndef MYDEFINE_H
#define MYDEFINE_H

#include <QString>
#include <QtWidgets>

typedef enum ModelCategory_{
    OUTFIT      = 0x0100,   // 主战装备
    STAFF       = 0x0200,   // 人员
    GROUND      = 0x0300,   // 地物
    BUILDING    = 0x0400,   // 建筑
}ModelCategory;

struct CurrentCategory {
    static ModelCategory category;
};

typedef enum CmdType_{
    REQUEST_UNKOWN          = 0x01111111, // 发生错误
    RESPONSE_ERROR          = 0x11111111, // 发生错误
    REQUEST_GETFEATURE      = 0x00000001, // 计算特征
    RESPONSE_GETFEATURE     = 0x10000001, // 计算特征
    REQUEST_SEARCHMODEL     = 0x00000002, // 搜索模型
    RESPONSE_SEARCHMODEL    = 0x10000002, // 搜索模型
    REQUEST_UPLOADMODEL     = 0x00000003, // 上传模型
    RESPONSE_UPLOADMODEL    = 0x10000003, // 上传模型
    REQUEST_SEARCHLABEL     = 0x00000004, // 搜索模型标签
    RESPONSE_SEARCHLABEL    = 0x10000004, // 搜索模型标签
    REQUEST_SEARCHTYPE      = 0x00000005, // 搜索类型标签
    RESPONSE_SEARCHTYPE     = 0x10000005, // 搜索类型标签
} CmdType;

typedef struct ModelInfo_{
    int     id          = 0;
    QString name        = "";
    QString category    = "";
    QString tags        = "";
    QString feature     = "";
    QString description = "";
    QString filepath    = "";
    float   similarity  = 0.;
    QString data        = 0;
    QString other       = "";
}ModelInfo;

// header len; header; body


typedef struct ModelHeader_{
    CmdType type            = REQUEST_UNKOWN;
    int     id = 0;
    char    name[64]        = "";
    char    genre[64]       = "";
    char    tags[128]       = "";
    char    hash[128]       = "";
    char    description[128]= "";
    float   similarity      = 0.;
    uint    datalen         = 0;
    char    other[128]      = "";
}ModelHeader;

typedef struct ModelsInfo_{
    QList<QString> titles;
    QJsonObject jsModels; // {"k0":{v00, v01, ...}, "k1":{v10, v11, ...}, ...}
}ModelsInfo;

typedef QHash<QString, QList<QVariant>> TableColsValue;
typedef QHash<QString, QList<QVariant>> TableRowsValue;
typedef QJsonObject TableJson;


struct CmdInfo
{
    int  info_type; // 发送者ID
    char info_to[20];   // 接收者ID
    int  info_length; //发送的消息主体的长度
    char info_content[1024]; //消息主体
};

enum Endian
{
    LittileEndian,
    BigEndian
};

#endif // MYDEFINE_H
