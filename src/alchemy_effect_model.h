// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#ifndef ALCHEMY_EFFECT_MODEL_H
#define ALCHEMY_EFFECT_MODEL_H

#include <qabstractitemmodel.h>
#include "alchemy_effect.h"

/**
 * 炼金效果Model. 仅用来展示.
 */
class AlchemyEffectModel : public QAbstractItemModel {
    Q_OBJECT

private:
    AlchemyEffectList effectList;    // 当前展示的炼金效果.
    const static int columnSize = 1; // 列的大小

public:
    enum Roles {
        CustomRole = Qt::UserRole
    };

public:
    /**
     * 通过一个炼金效果列表来初始化一个Model.
     */
    AlchemyEffectModel(const AlchemyEffectList& effects);

    /**
     * 通过一个炼金效果列表来初始化一个Model.
     */
    AlchemyEffectModel(const QList<AlchemyEffect*>& effects);

    virtual QVariant data(const QModelIndex& index, int role) const override;

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual QModelIndex parent(const QModelIndex& child) const override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;

    /**
     * 通过行号来获取对应的炼金效果.
     */
    const AlchemyEffect* getAlchemyEffectByRowNumber(int row);
};

#endif // ALCHEMY_EFFECT_MODEL_H
