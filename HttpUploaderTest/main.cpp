#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "httpuploader.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterUncreatableType<HttpPostField>("HttpUp", 1, 0, "HttpPostField", "Can't touch this");
    qmlRegisterType<HttpPostFieldValue>("HttpUp", 1, 0, "HttpPostFieldValue");
    qmlRegisterType<HttpPostFieldFile>("HttpUp", 1, 0, "HttpPostFieldFile");
    qmlRegisterType<HttpUploader>("HttpUp", 1, 0, "HttpUploader");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/HttpUploaderTest/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
