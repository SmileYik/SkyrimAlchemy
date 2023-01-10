// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#include "alchemy_effect.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#define ALCHEMY_EFFECT_PRICE_FILE ":/data/alchemy_effect_price"

AlchemyEffect::AlchemyEffect() = default;

bool AlchemyEffect::addAlchemyEffect(AlchemyEffect* alchemyEffect) {
    if (alchemyEffect->id.isEmpty()) {
        return false;
    }
    if (loadedAlchemyEffects.constFind(alchemyEffect->id) ==
        loadedAlchemyEffects.end()) {
        loadedAlchemyEffects[alchemyEffect->id] = alchemyEffect;
        alchemyEffectNameIdMap[alchemyEffect->name] = alchemyEffect->id;
        return true;
    }
    return false;
}

void AlchemyEffect::clearAlchemyEffects() {
    auto all = loadedAlchemyEffects.values();
    for (auto* effect : all) {
        if (effect) {
            delete effect;
        }
    }
    loadedAlchemyEffects.clear();
}

int AlchemyEffect::loadAlchemyEffects() {
    int count = 0;
    QFile file(ALCHEMY_EFFECT_PRICE_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        return -1;
    }
    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        if (line.isEmpty()) {
            break;
        }
        QStringList list = line.split(" ");
        AlchemyEffect* effect = new AlchemyEffect();
        effect->id = list[0];
        effect->name = list[1];
        effect->price = list[2].toInt();
        for (int i = 4; i < list.size(); ++i) {
            effect->priceIfContainsMaterial[list[i - 1].mid(1)] = list[i].toInt();
        }
        if (addAlchemyEffect(effect)) {
            ++count;
        }
    }
    file.close();
    return count;
}

const QMap<QString, AlchemyEffect*>& AlchemyEffect::getAlchemyEffects() {
    return loadedAlchemyEffects;
}

const AlchemyEffect* AlchemyEffect::getAlchemyEffectById(const QString& id) {
    if (loadedAlchemyEffects.constFind(id) == loadedAlchemyEffects.end()) {
        return nullptr;
    }
    return loadedAlchemyEffects[id];
}

const AlchemyEffect * AlchemyEffect::getAlchemyEffectByName(const QString& name) {
    if (alchemyEffectNameIdMap.constFind(name) == alchemyEffectNameIdMap.end()) {
        return nullptr;
    }
    return getAlchemyEffectById(alchemyEffectNameIdMap[name]);
}
