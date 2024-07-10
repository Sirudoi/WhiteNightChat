#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // load qss file setting
    QFile qss(":/style/stylesheer.qss");
    if (qss.open(QFile::ReadOnly)) {
        // convert read content to QString type
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();

        // TODO:debug log

    } else {
        // TODO:debug log
    }


    MainWindow w;
    w.show();
    return a.exec();
}
