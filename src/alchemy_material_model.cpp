// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#include "alchemy_material_model.h"
#include <QDebug>
#include <QMap>
#include <QColor>

AlchemyMaterialModel::AlchemyMaterialModel(
    const AlchemyMaterialList& materials) : materialList(materials) {
    int size = materials.size();
    while (size > 0) {
        displayList.push_front(--size);
    }
}

AlchemyMaterialModel::AlchemyMaterialModel(
    const QList<AlchemyMaterial*>& materials) {
    int idx = 0;
    for (const auto* m : materials) {
        materialList << m;
        displayList << idx++;
    }
}


QVariant AlchemyMaterialModel::data(const QModelIndex& index, int role) const {
    if (index.row() < 0 || index.row() > displayList.count() ||
        index.column() < 0 || index.column() > columnCount()) {
        return QVariant();
    }



    switch (role) {
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignCenter);
        case Qt::DisplayRole:
            break;
        case Qt::BackgroundRole:
            break;
        default:
            return QVariant();

    }
    // qDebug() << "[AlchemyMaterialModel] [data] displayRow = " << index.row();
    int displayIdx = displayList[index.row()];
    // qDebug() << "[AlchemyMaterialModel] [data] displayIdx = " << displayIdx;
    auto material = materialList[displayIdx];
    if (!material) {
        return QVariant();
    }

    switch (role) {
        case Qt::BackgroundRole:
            return highlightMaterials.contains(material) ?
                        QVariant(QColor(0x00AA00)) : QVariant();
    }

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
    return displayList.count();
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
    beginResetModel();
    bool flag = order == Qt::SortOrder::AscendingOrder;
    QMap<const AlchemyMaterial*, int> map;
    AlchemyMaterialList materialList;

    qDebug() << "[AlchemyMaterialModel] [sort] desplayList count = " << displayList.count();
    for (int idx : displayList) {
        qDebug() << "[AlchemyMaterialModel] [sort] desplayList foreach " << idx;
        auto material = this->materialList[idx];
        map[material] = idx;
        materialList << material;
    }

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

    int i = 0;
    for (auto& m : materialList) {
        displayList[i++] = map[m];
    }
    endResetModel();
}

const AlchemyMaterial* AlchemyMaterialModel::getAlchemyMaterialByRowNumber(int row) {
    if (row >= 0 && row < rowCount()) {
        return materialList[displayList[row]];
    }
    return nullptr;
}

void AlchemyMaterialModel::search(const QString& name,
                                  const QString& effectId,
                                  bool displayHighlight) {
    beginResetModel();
    // beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    displayList.clear();
    // endRemoveRows();

    int size = materialList.count();
    for (int i = 0; i < size; ++i) {
        bool picked = true;
        if (!name.isEmpty() && !materialList[i]->name.isLike(name)) {
            picked = false;
        }
        if (!effectId.isEmpty() &&
                !materialList[i]->includeEffectId.contains(effectId)) {
            picked = false;
        }
        if (displayHighlight && !highlightMaterials.contains(materialList[i])) {
            picked = false;
        }
        if (picked) {
            displayList << i;
        }
    }
    // beginInsertRows(QModelIndex(), 0, displayList.count() - 1);
    // endInsertRows();
    endResetModel();
}

void AlchemyMaterialModel::highlight(const AlchemyMaterial *& material) {
    beginResetModel();
    if (!highlightMaterials.contains(material)) {
        highlightMaterials << material;
    }
    endResetModel();
}

void AlchemyMaterialModel::removeHighlight(const AlchemyMaterial *& material) {
    beginResetModel();
    highlightMaterials.removeOne(material);
    endResetModel();
}

void AlchemyMaterialModel::resetHighlight() {
    beginResetModel();
    highlightMaterials.clear();
    endResetModel();
}

void AlchemyMaterialModel::highlightAll() {
    beginResetModel();
    highlightMaterials << materialList;
    endResetModel();
}

int AlchemyMaterialModel::getAlchemyMaterialRow(const AlchemyMaterial* material) {
    int idx = materialList.indexOf(material);
    return displayList.indexOf(idx);
}




