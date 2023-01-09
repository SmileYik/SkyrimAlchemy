// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#ifndef ALCHEMY_EFFECT_H
#define ALCHEMY_EFFECT_H

#include <qobject.h>
#include <QMap>

/**
 * 炼金效果.
 */
class AlchemyEffect : public QObject {
    Q_OBJECT

public:
    QString id;    // 炼金效果id
    QString name;  // 炼金效果名
    qint32 price;  // 基础价格
    QMap<QString, qint32> priceIfContainsMaterial; // 特殊价格, 炼金材料id-价格 map.
                                                   // 代表包含该材料则基础价格为此特殊价格

    /**
     * Default constructor
     */
    AlchemyEffect();

    /**
     * 通过文件加载炼金效果清单.
     */
    static int loadAlchemyEffects();

    /**
     * 向已加载的炼金效果中添加一个新的炼金效果.
     */
    static bool addAlchemyEffect(AlchemyEffect*);

    /**
     * 清除所有已经加载的炼金效果
     */
    static void clearAlchemyEffects();

    /**
     * 获取所有已经加载的炼金效果
     */
    static const QMap<QString, AlchemyEffect*>& getAlchemyEffects();

    /**
     * 通过炼金效果id来获取一个实例
     */
    static const AlchemyEffect* getAlchemyEffectById(const QString& id);

    /**
     * 通过炼金效果名称来获取一个实例
     */
    static const AlchemyEffect* getAlchemyEffectByName(const QString& name);
};

static QMap<QString, AlchemyEffect*> loadedAlchemyEffects; // 已经加载的炼金效果
static QMap<QString, QString> alchemyEffectNameIdMap;      // 炼金效果名称与id映射

typedef QList<const AlchemyEffect*> AlchemyEffectList;

#endif // ALCHEMY_EFFECT_H
