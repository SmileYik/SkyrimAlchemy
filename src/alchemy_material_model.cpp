// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#include "alchemy_material_model.h"
#include <QDebug>

AlchemyMaterialModel::AlchemyMaterialModel(
    const AlchemyMaterialList& materials) : materialList(materials) {

}

AlchemyMaterialModel::AlchemyMaterialModel(
    const QList<AlchemyMaterial*>& materials) {
    for (const auto* m : materials) {
        materialList << m;
    }
}


QVariant AlchemyMaterialModel::data(const QModelIndex& index, int role) const {
    if (index.row() < 0 || index.row() > rowCount() ||
        index.column() < 0 || index.column() > columnCount()) {
        return QVariant();
    }

    switch (role) {
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignCenter);
        case Qt::DisplayRole:
            break;
        default:
            return QVariant();

    }

    auto material = materialList[index.row()];
    switch (index.column()) {
        case 0:
            return QVariant(material->id);
        case 1:
            return QVariant(material->name.gameName);
        case 2:
            return QVariant(material->name.getTranslatedName());
        case 3:
        case 4:
        case 5:
        case 6:
            auto effects = material->getIncludeEffects();
            return QVariant(effects[index.column() - 3]->name);
    }
    return QVariant();
}

int AlchemyMaterialModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return columnSize;
}

int AlchemyMaterialModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return materialList.count();
}

QModelIndex AlchemyMaterialModel::parent(const QModelIndex& child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

QModelIndex AlchemyMaterialModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column, (void *)& materialList[row]);
}

QVariant AlchemyMaterialModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const {
    if (role != Qt::DisplayRole) {
        return QAbstractItemModel::headerData(section, orientation, role);
    } else if (orientation == Qt::Orientation::Horizontal) {
        return QVariant(header[section]);
    } else {
        return QAbstractItemModel::headerData(section, orientation, role);
    }
}

void AlchemyMaterialModel::sort(int column, Qt::SortOrder order) {
    bool flag = order == Qt::SortOrder::AscendingOrder;
    switch (column) {
        case 0:
            std::sort(materialList.begin(), materialList.end(),
                      [&] (const AlchemyMaterial*& a, const AlchemyMaterial*& b) {
                          return flag ? a->id > b->id : a->id < b->id;
                      });
            break;
        case 1:
            std::sort(materialList.begin(), materialList.end(),
                      [&] (const AlchemyMaterial*& a, const AlchemyMaterial*& b) {
                          return flag ? a->name.gameName > b->name.gameName :
                                        a->name.gameName < b->name.gameName;
                      });
            break;
        case 2:
            std::sort(materialList.begin(), materialList.end(),
                      [&] (const AlchemyMaterial*& a, const AlchemyMaterial*& b) {
                          return flag ? a->name.getTranslatedName() > b->name.getTranslatedName() :
                                        a->name.getTranslatedName() < b->name.getTranslatedName();
                      });
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            int idx = column - 3;
            std::sort(materialList.begin(), materialList.end(),
                      [&] (const AlchemyMaterial*& a, const AlchemyMaterial*& b) {
                          return flag ? a->includeEffectId[idx] > b->includeEffectId[idx] :
                                        a->includeEffectId[idx] < b->includeEffectId[idx];
                      });
            break;
    }
}

const AlchemyMaterial* AlchemyMaterialModel::getAlchemyMaterialByRowNumber(int row) {
    if (row >= 0 && row < rowCount()) {
        return materialList[row];
    }
    return nullptr;
}
