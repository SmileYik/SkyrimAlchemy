// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#ifndef ALCHEMY_RECIPE_MODEL_H
#define ALCHEMY_RECIPE_MODEL_H

#include <qabstractitemmodel.h>
#include "alchemy_recipe.h"

/**
 * 炼金配方Model.
 */
class AlchemyRecipeModel : public QAbstractItemModel {
    Q_OBJECT

private:
    typedef QList<AlchemyRecipe*> AlchemyRecipeList;
    AlchemyRecipeList recipeList;
    // 标头后面为 (效果1 重复次数1) ~ n
    QStringList header = {"编号组合", "原材料1", "原材料2", "原材料3", "价格1", "价格2"};

public:
    enum Roles {
        DisplayRole = Qt::DisplayRole,
        TextAlignmentRole = Qt::TextAlignmentRole,
        CustomRole = Qt::UserRole,
    };

public:
    AlchemyRecipeModel(const AlchemyRecipeList& recipes);

    virtual QVariant data(const QModelIndex& index, int role) const override;

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual QModelIndex parent(const QModelIndex& child) const override;


    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    virtual void sort(int column, Qt::SortOrder order) override;

    /**
     * 通过行号来获取对应的配方.
     */
    const AlchemyRecipe* getAlchemyRecipeByRowNumber(int row);
};

#endif // ALCHEMY_RECIPE_MODEL_H
