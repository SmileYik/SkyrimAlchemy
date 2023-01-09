#include "skyrimalchemy.h"
#include <QApplication>

#define String std::string

int main(int argc, char *argv[]) {
    AlchemyEffect::loadAlchemyEffects();
    AlchemyMaterial::loadAlchemyMaterials();
    QApplication app(argc, argv);
    SkyrimAlchemy w;
    w.show();
    int code = app.exec();
    AlchemyEffect::clearAlchemyEffects();
    AlchemyMaterial::clearAlchemyMaterials();
    return code;
}

