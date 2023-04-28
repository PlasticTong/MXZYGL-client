QT       += core gui opengl widgets
QT += datavisualization
QT  += network
QT += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fbxrender.cpp \
    form.cpp \
    main.cpp \
    modelclient.cpp \
    modelsearch.cpp \
    modelsearchbylabel.cpp \
    modelsearchbytype.cpp \
    modeltable.cpp \
    modeltablelabel.cpp \
    modelupload.cpp \
    modelviewer.cpp \
    myutils.cpp \
    datarender.cpp \
    objrender.cpp \
    offrender.cpp \
    rgbfunction.cpp \
    rgbrender.cpp \
    rgbwidget.cpp \
    simplepanel.cpp \
    multipanel.cpp \
    pointrender.cpp \
    threadbase.cpp \
    camera.cpp \
    glwidget.cpp \
    light.cpp \
    material.cpp \
    trackball.cpp \
#    tiny_obj_loader.cc


HEADERS += \
    fbxrender.h \
    form.h \
    modelclient.h \
    modelsearch.h \
    modelsearchbylabel.h \
    modelsearchbytype.h \
    modeltable.h \
    modeltablelabel.h \
    modelupload.h \
    modelviewer.h \
    mydefine.h \
    myutils.h \
    datarender.h \
    objrender.h \
    offrender.h \
    pointrender.h \
    rgbfunction.h \
    rgbrender.h \
    rgbwidget.h \
    simplepanel.h \
    multipanel.h \
    threadbase.h \
    camera.h \
    glwidget.h \
    light.h \
    material.h \
    trackball.h \
    tiny_obj_loader.h \

RESOURCES += \
    resources.qrc


#FORMS += \
#    mainwindow.ui

include(OpenFBX/openfbx.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#include( ../vendor/tinyobjloader/tinyobjloader.pri )

INCLUDEPATH += $$PWD/../inc
#   $$PWD/../vendor/tinyobjloader
DESTDIR = $$PWD/../bin
LIBS += -L$$PWD/../lib
LIBS += -lopengl32 -lopencv_world3415

FORMS += \
    form.ui

