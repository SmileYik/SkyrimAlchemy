#include "skyrimalchemy.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SkyrimAlchemy w;
    w.show();

    return app.exec();
}

