// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#include "alchemy_effect_model.h"

AlchemyEffectModel::AlchemyEffectModel(
    const AlchemyEffectList& effects) : effectList(effects) {
    effectList.push_front(nullptr);
}

AlchemyEffectModel::AlchemyEffectModel(
    const QList<AlchemyEffect*>& effects) {
    // 添加一个nullptr代表未选择状态.
    effectList << nullptr;
    for (const auto* e : effects) {
        effectList << e;
    }
}


QVariant AlchemyEffectModel::data(const QModelIndex& index, int role) const {
    if (index.row() < 0 || index.row() >= rowCount() ||
        index.column() < 0 || index.column() > columnCount() ||
        role != Qt::DisplayRole) {
        return QVariant();
    }

    auto effect = effectList[index.row()];

    switch (index.column()) {
        case 0:
            return effect ? QVariant(effect->name) : QVariant("不选择");
    }
    return QVariant();
}

int AlchemyEffectModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return columnSize;
}

int AlchemyEffectModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return effectList.count();
}

QModelIndex AlchemyEffectModel::parent(const QModelIndex& child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

QModelIndex AlchemyEffectModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column, (void *)& effectList[row]);
}

QVariant AlchemyEffectModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const {
    if (role != Qt::DisplayRole) {
        return QAbstractItemModel::headerData(section, orientation, role);
    } else if (orientation == Qt::Orientation::Horizontal) {
        return QVariant();
    } else {
        return QAbstractItemModel::headerData(section, orientation, role);
    }
}

const AlchemyEffect* AlchemyEffectModel::getAlchemyEffectByRowNumber(int row) {
    if (row >= 0 && row < rowCount()) {
        return effectList[row];
    }
    return nullptr;
}

