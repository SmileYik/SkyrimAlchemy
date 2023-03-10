// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#include "alchemy_recipe.h"
#include <QSet>

AlchemyRecipe::AlchemyRecipe(const AlchemyMaterial* material1,
                             const AlchemyMaterial* material2) :
                             AlchemyRecipe(AlchemyMaterialList({
                                 material1, material2
                             })) {
}

AlchemyRecipe::AlchemyRecipe(const AlchemyMaterial* material1,
                             const AlchemyMaterial* material2,
                             const AlchemyMaterial* material3) :
                             AlchemyRecipe(AlchemyMaterialList({
                                 material1, material2, material3
                             })) {
}

AlchemyRecipe::AlchemyRecipe(const AlchemyMaterialList& materials) {
    int size = materials.size();
    if ((size > 3 || size <= 1) ||
        QSet<const AlchemyMaterial*>(materials.begin(), materials.end()).size() != size) {
        return;
    }

    this->materials << materials;
    QStringList ids;
    // 混合材料, 获取材料中的效果.
    for (const auto* material : this->materials) {
        const AlchemyEffectList list = material->getIncludeEffects();
        for (const auto* effect : list) {
            if (effects.contains(effect)) {
                effects[effect] += 1;
            } else {
                effects[effect] = 1;
            }
        }
        ids << material->id;
    }
    mixedId = ids.join("\n");
    ids.sort();
    id = ids.join("+");

    AlchemyEffectList needDelete;
    price1 = price2 = 0;
    // 遍历效果, 留下重复2~3次的效果. 并计算价格.
    for (auto begin = effects.constBegin(),
                end = effects.constEnd(); begin != end; begin++) {
        if (begin.value() >= 2) {
            qint32 price = begin.key()->price;
            for (const auto& key : begin.key()->priceIfContainsMaterial.keys()) {
                if (isContainsMaterialById(key)) {
                    price = begin.key()->priceIfContainsMaterial[key];
                    break;
                }
            }
            price1 += price;
            price2 += (qint32) (price * (1.0 + ((begin.value() - 2.0) / 2.0)));
        } else {
            needDelete << begin.key();
        }
    }
    // 清除无用效果.
    for (const auto* effect : needDelete) {
        effects.remove(effect);
    }
    needDelete.clear();

}

bool AlchemyRecipe::isContainsMaterialById(const QString& id) const {
    for (const auto* material : materials) {
        if (material->id == id) {
            return true;
        }
    }
    return false;
}

qint32 AlchemyRecipe::getPrice1() const {
    return price1;
}

qint32 AlchemyRecipe::getPrice2() const {
    return price2;
}

QMap<const AlchemyEffect *, quint16> AlchemyRecipe::getEffects() const {
    return effects;
}

AlchemyMaterialList AlchemyRecipe::getMaterials() const {
    return materials;
}

bool AlchemyRecipe::isValidRecipe() const {
    return effects.count() > 0;
}

bool AlchemyRecipe::isRedundant() const {
    bool isValid = isValidRecipe();
    if (isValid && materials.count() == 2) {
        return false;
    } else if (!(isValid && materials.count() == 3)) {
        return true;
    }

    bool flag = false;
    for (int i = 0; i < materials.count(); ++i) {
        for (int j = i + 1; j < materials.count(); ++j) {
            AlchemyRecipe recipe(materials[i], materials[j]);
            if (recipe.isValidRecipe()) {
                if (recipe.getPrice1() == price1) {
                    flag = true;
                }
            }
            recipe.deleteLater();
        }
    }
    return flag;
}
