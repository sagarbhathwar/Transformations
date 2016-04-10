#include <QGuiApplication>
#include "window.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    //Size of color buffers RGB
    //Size of alpha buffers and stencil buffers
    //Number of samples per pixel for multisampling
    //OpenGL profile and version for rendering
    //Enable/Disable stereo buffers and swap behaviour
    //http://doc.qt.io/qt-5/qsurfaceformat.html

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4, 4);

    Window window;
    window.setFormat(format);
    window.resize(QSize(800, 600));
    window.show();

    return a.exec();
}
