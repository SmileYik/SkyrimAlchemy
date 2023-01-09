// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#ifndef ALCHEMY_RECIPE_H
#define ALCHEMY_RECIPE_H

#include <qobject.h>
#include "alchemy_effect.h"
#include "alchemy_material.h"

/**
 * 炼金配方.
 */
class AlchemyRecipe : public QObject
{
    Q_OBJECT
private:
    AlchemyMaterialList materials;               // 使用的材料
    QMap<const AlchemyEffect*, quint16> effects; // 配方应该得到的效果.
    qint32 price1; // 第一种价格估测, 达成效果的效果基础价格之和
    qint32 price2; // 第二种价格估测, 达成三种效果时该效果基础价格 * 1.5

    /**
     * 是否包含某个材料.
     */
    bool isContainsMaterialById(const QString& id) const;
public:
    QString mixedId; // 配方中使用到的原材料 id 混合

    /**
     * 通过材料列表来计算一个配方.
     * 仅有材料为2个或3个时才会可能生产有用的炼金配方.
     * 判断生产的炼金配方是否有用需要使用isValidRecipe()方法判断.
     */
    AlchemyRecipe(const AlchemyMaterialList&);

    /**
     * 通过材料列表来计算一个配方.
     * 仅有材料为2个或3个时才会可能生产有用的炼金配方.
     * 判断生产的炼金配方是否有用需要使用isValidRecipe()方法判断.
     */
    AlchemyRecipe(const AlchemyMaterial* material1,
                  const AlchemyMaterial* material2);

    /**
     * 通过材料列表来计算一个配方.
     * 仅有材料为2个或3个时才会可能生产有用的炼金配方.
     * 判断生产的炼金配方是否有用需要使用isValidRecipe()方法判断.
     */
    AlchemyRecipe(const AlchemyMaterial* material1,
                  const AlchemyMaterial* material2,
                  const AlchemyMaterial* material3);

    /**
     * 获取第一种预测的价格.
     */
    qint32 getPrice1() const;

    /**
     * 获取第二种预测的价格.
     */
    qint32 getPrice2() const;

    /**
     * 获取该配方所使用到的炼金材料.
     */
    AlchemyMaterialList getMaterials() const;

    /**
     * 获取该配方混合后得到的药水应该能够拥有的效果.
     */
    QMap<const AlchemyEffect*, quint16> getEffects() const;

    /**
     * 判断当前配方是否为一个有效配方
     */
    bool isValidRecipe() const;
};

#endif // ALCHEMY_RECIPE_H
