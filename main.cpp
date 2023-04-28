#include "modelviewer.h"

#include <QApplication>
#include <QGLFormat>

int main(int argc, char *argv[])
{
    QGLFormat format = QGLFormat :: defaultFormat () ;
    format.setSampleBuffers ( true );
    format.setSamples (8) ;

    if (! format . sampleBuffers () )
        qWarning (" Multisample buffer is not supported .");

    QGLFormat :: setDefaultFormat ( format );

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    ModelViewer w;
//    w.show();
    w.showMaximized();

    return a.exec();
}
