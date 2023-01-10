// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#include "alchemy_material.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#define ALCHEMY_MATERIAL_FILE ":/data/alchemy_material"
#define ALCHEMY_MATERIAL_EN_FILE ":/data/alchemy_material_en"
#define ALCHEMY_MATERIAL_TR_FILE ":/data/alchemy_material_tr"

const QString& AlchemyMaterialName::getTranslatedName() const {
    return translatedName;
}

bool AlchemyMaterialName::isLike(const QString& name) const {
    QString tmp = name.toLower();
    return gameName.toLower().contains(tmp) || translatedName.contains(tmp);
}


AlchemyMaterialName::AlchemyMaterialName(const QString& aName) : gameName(aName) {
}


AlchemyMaterial::AlchemyMaterial()
                    : QObject(), id(""), name("None"), includeEffectId() {

}


int AlchemyMaterial::loadAlchemyMaterials() {
    QMap<QString, QString> enName;
    QMap<QString, QStringList> trName;
    QFile materialFile(ALCHEMY_MATERIAL_FILE);
    QFile enFile(ALCHEMY_MATERIAL_EN_FILE);
    QFile trFile(ALCHEMY_MATERIAL_TR_FILE);
    if (!materialFile.open(QIODevice::ReadOnly)) {
        return -1;
    }
    // load material en name
    if (enFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&enFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.isEmpty()) {
                continue;
            }
            int i = line.indexOf(" ");
            enName.insert(
                line.mid(0, i),
                line.mid(i + 1)
            );
        }
        enFile.close();
    }
    // load material translated name
    if (trFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&trFile);
        while (!in.atEnd()) {
            QStringList list;
            QString line = in.readLine();
            if (line.isEmpty()) {
                continue;
            }
            int i = line.indexOf(" ");
            QString id = line.mid(0, i);
            if (trName.constFind(id) != trName.end()) {
                trName[id] << line.mid(i + 1).split(" ");
            } else {
                trName.insert(
                    id,
                    line.mid(i + 1).split(" ")
                );
            }
        }
        trFile.close();
    }
    // load material from file.
    QTextStream in(&materialFile);
    int count = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) {
            continue;
        }
        AlchemyMaterial* material = new AlchemyMaterial();
        int i = line.indexOf(" ");
        material->id = line.mid(0, i);
        material->includeEffectId << line.mid(i + 1).split(" ");
        material->includeEffectId.sort();
        if (enName.constFind(material->id) != enName.end()) {
            material->name.gameName = enName[material->id];
        }
        if (trName.constFind(material->id) != trName.end()) {
            material->name.translatedNames << trName[material->id];
        }
        if (addAlchemyMaterial(material)) {
            ++count;
        }
    }

    materialFile.close();
    return count;
}

bool AlchemyMaterial::addAlchemyMaterial(
    AlchemyMaterial* material) {
    if (material->id.isEmpty()) {
        return false;
    }
    if (loadedAlchemyMaterials.constFind(material->id) ==
                                            loadedAlchemyMaterials.end()) {
        material->name.translatedNames.removeDuplicates();
        loadedAlchemyMaterials[material->id] = material;
        material->name.translatedName = material->name.translatedNames.join("\n");
        return true;
    }
    return false;
}

void AlchemyMaterial::clearAlchemyMaterials() {
    auto all = loadedAlchemyMaterials.values();
    for (auto* material : all) {
        if (material) {
            material->deleteLater();
        }
    }
    loadedAlchemyMaterials.clear();
}

const QMap<QString, AlchemyMaterial*>& AlchemyMaterial::getAlchemyMaterials() {
    return loadedAlchemyMaterials;
}

const AlchemyMaterial* AlchemyMaterial::getAlchemyMaterialById(const QString& id) {
    if (loadedAlchemyMaterials.constFind(id) == loadedAlchemyMaterials.end()) {
        return nullptr;
    }
    return loadedAlchemyMaterials[id];
}

AlchemyEffectList AlchemyMaterial::getIncludeEffects() const {
    AlchemyEffectList list;
    for (const auto& id : includeEffectId) {
        list << AlchemyEffect::getAlchemyEffectById(id);
    }
    return list;
}
