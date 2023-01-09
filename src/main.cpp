#include "skyrimalchemy.h"
#include <QApplication>

#define String std::string

int main(int argc, char *argv[]) {
    AlchemyEffect::loadAlchemyEffects();
    AlchemyMaterial::loadAlchemyMaterials();
    QApplication app(argc, argv);
    QIcon ico("./resources/skyrimalchemy.ico");
    app.setWindowIcon(ico);
    SkyrimAlchemy w;
    w.setWindowIcon(ico);
    w.show();
    int code = app.exec();
    AlchemyEffect::clearAlchemyEffects();
    AlchemyMaterial::clearAlchemyMaterials();
    return code;
}

