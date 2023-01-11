#include "skyrimalchemy.h"
#include <QApplication>
#include <QFontDatabase>
#include <QFont>

#define String std::string

int main(int argc, char *argv[]) {
    AlchemyEffect::loadAlchemyEffects();
    AlchemyMaterial::loadAlchemyMaterials();
    QApplication app(argc, argv);
    int fontId = QFontDatabase::addApplicationFont(":/font/NotoSansCJK-Regular.ttc");
    if (fontId != -1) {
        QFont font(QFontDatabase::applicationFontFamilies(fontId)[0]);
        app.setFont(font);
    }
    // QIcon ico("./resources/skyrimalchemy.ico");
    // app.setWindowIcon(ico);
    SkyrimAlchemy w;
    // w.setWindowIcon(ico);
    w.show();
    int code = app.exec();
    AlchemyEffect::clearAlchemyEffects();
    AlchemyMaterial::clearAlchemyMaterials();
    return code;
}

