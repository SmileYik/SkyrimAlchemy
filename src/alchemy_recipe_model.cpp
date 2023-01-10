// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#include "alchemy_recipe_model.h"
#include <QDebug>
#include <cmath>

AlchemyRecipeModel::AlchemyRecipeModel(
    const AlchemyRecipeList& recipes) : recipeList(recipes) {
    int maxEffectCount = -1;
    for (const auto& recipe : recipeList) {
        maxEffectCount = qMax(maxEffectCount, recipe->getEffects().size());
    }
    for (int i = 0; i < maxEffectCount; ++i) {
        header << QString("效果 %1").arg(i + 1)
               << QString("叠加 %1").arg(i + 1);
    }
}


QVariant AlchemyRecipeModel::data(const QModelIndex& index, int role) const {
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

    auto recipe = recipeList[index.row()];
    int idx;
    switch (index.column()) {
        case 0:
            return QVariant(recipe->mixedId);
        case 1:
        case 2:
        case 3:
            idx = index.column() - 1;
            return recipe->getMaterials().size() > idx ?
                        QVariant(recipe->getMaterials()[idx]->name.getTranslatedName()) :
                        QVariant("-");
        case 4:
            return QVariant(recipe->getPrice1());
        case 5:
            return QVariant(recipe->getPrice2());
        default:
            idx = index.column() - 6;
            if ((idx & 0x1) == 0) {
                idx >>= 1;
                if (recipe->getEffects().size() > idx) {
                    return QVariant(
                        recipe->getEffects().keys()[idx]->name
                    );
                }
            } else {
                idx >>= 1;
                if (recipe->getEffects().size() > idx) {
                    return QVariant(
                        recipe->getEffects().values()[idx]
                    );
                }
            }
    }
    return QVariant("-");
}

int AlchemyRecipeModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return header.size();
}

int AlchemyRecipeModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return recipeList.count();
}

QModelIndex AlchemyRecipeModel::parent(const QModelIndex& child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

QModelIndex AlchemyRecipeModel::index(int row, int column,
                                      const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column, (void *)& recipeList[row]);
}

QVariant AlchemyRecipeModel::headerData(int section,
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

void AlchemyRecipeModel::sort(int column, Qt::SortOrder order) {
    beginResetModel();
    bool flag = order == Qt::SortOrder::AscendingOrder;
    int idx;
    bool isKey;
    switch (column) {
        case 0:
            std::sort(recipeList.begin(), recipeList.end(),
                      [&] (const AlchemyRecipe* a, const AlchemyRecipe* b) {
                          return flag ? a->mixedId > b->mixedId :
                                        a->mixedId < b->mixedId;
                      });
            break;
        case 1:
        case 2:
        case 3:
            idx = column - 1;
            std::sort(recipeList.begin(), recipeList.end(),
                      [&] (const AlchemyRecipe* a, const AlchemyRecipe* b) {
                          QString p, q;
                          if (idx < a->getMaterials().size()) {
                              p = a->getMaterials()[idx]->name.getTranslatedName();
                          }
                          if (idx < b->getMaterials().size()) {
                              q = b->getMaterials()[idx]->name.getTranslatedName();
                          }
                          return flag ? p > q : p < q;
                      });
            break;
        case 4:
            std::sort(recipeList.begin(), recipeList.end(),
                      [&] (const AlchemyRecipe* a, const AlchemyRecipe* b) {
                          return flag ? a->getPrice1() > b->getPrice1() :
                                        a->getPrice1() < b->getPrice1();
                      });
            break;
        case 5:
            std::sort(recipeList.begin(), recipeList.end(),
                      [&] (const AlchemyRecipe* a, const AlchemyRecipe* b) {
                          return flag ? a->getPrice2() > b->getPrice2() :
                                        a->getPrice2() < b->getPrice2();
                      });
            break;
        default:
            idx = column - 6;
            isKey = (idx & 0x1) == 0;
            idx >>= 1;
            std::sort(recipeList.begin(), recipeList.end(),
                      [&] (const AlchemyRecipe* a, const AlchemyRecipe* b) {
                          if (isKey) {
                              QString p, q;
                              if (a->getEffects().size() > idx) {
                                  p = a->getEffects().keys()[idx]->name;
                              }
                              if (b->getEffects().size() > idx) {
                                  q = b->getEffects().keys()[idx]->name;
                              }
                              return flag ? p > q : p < q;
                          } else {
                              int p, q;
                              if (a->getEffects().size() > idx) {
                                  p = a->getEffects().values()[idx];
                              }
                              if (b->getEffects().size() > idx) {
                                  q = b->getEffects().values()[idx];
                              }
                              return flag ? p > q : p < q;
                          }
                      });
            break;
    }
    endResetModel();
}

const AlchemyRecipe * AlchemyRecipeModel::getAlchemyRecipeByRowNumber(int row) {
    if (row >= 0 && row < rowCount()) {
        return recipeList[row];
    }
    return nullptr;
}
