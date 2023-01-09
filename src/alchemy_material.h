// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#ifndef ALCHEMY_MATERIAL_H
#define ALCHEMY_MATERIAL_H

#include <qobject.h>
#include <QStringList>
#include <QMap>
#include "alchemy_effect.h"

class AlchemyMaterialName;
class AlchemyMaterial;

/**
 * 炼金材料名称及其翻译, 以游戏内的名称为主.
 */
class AlchemyMaterialName {
    friend class AlchemyMaterial;
private:
    QString translatedName;    // 翻译名
public:
    /*
     * 通过游戏名初始化
     */
    AlchemyMaterialName(const QString& aName);

    QString gameName;            // 游戏名
    QStringList translatedNames; // 翻译名

    /**
     * 获取翻译名字符串.
     */
    const QString& getTranslatedName() const;
    /**
     *
     * 材料的名称是否像某个字符串.
     * 会在游戏名和翻译名中进行检索.
     */
    bool isLike(const QString& name) const;
};

/**
 * 炼金材料. 一个炼金材料包含4种效果.
 */
class AlchemyMaterial : public QObject {
    Q_OBJECT
public:
    AlchemyMaterial();

    QString id;                  // 游戏里该物品材料代码
    AlchemyMaterialName name;    // 该材料名
    QStringList includeEffectId; // 包含的效果.

    /**
     * 获取包含的效果列表.
     */
    AlchemyEffectList getIncludeEffects() const;

    /**
     * 从文件中读取所有的炼金材料.
     */
    static int loadAlchemyMaterials();

    /**
     * 添加一个炼金材料到加载列表中
     */
    static bool addAlchemyMaterial(AlchemyMaterial*);

    /**
     * 删除所有炼金材料数据.
     */
    static void clearAlchemyMaterials();

    /**
     * 获取已经加载的所有炼金材料
     */
    static const QMap<QString, AlchemyMaterial*>& getAlchemyMaterials();

    /**
     * 通过炼金材料的id来获取一个炼金材料的实例
     */
    static const AlchemyMaterial* getAlchemyMaterialById(const QString& id);
};

static QMap<QString, AlchemyMaterial*> loadedAlchemyMaterials;  // 已加载炼金材料
typedef QList<const AlchemyMaterial*> AlchemyMaterialList;

#endif // ALCHEMY_MATERIAL_H
