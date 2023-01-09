// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#ifndef ALCHEMY_MATERIAL_MODEL_H
#define ALCHEMY_MATERIAL_MODEL_H

#include <qabstractitemmodel.h>
#include "alchemy_material.h"

/**
 * 炼金材料Model 用来展示炼金材料.
 */
class AlchemyMaterialModel : public QAbstractItemModel {
    Q_OBJECT

private:
    AlchemyMaterialList materialList;
    const static int columnSize = 7;
    QStringList const header =
                    {"编号", "名称", "翻译", "效果1", "效果2", "效果3", "效果4"};

public:
    enum Roles {
        DisplayRole = Qt::DisplayRole,
        TextAlignmentRole = Qt::TextAlignmentRole,
        CustomRole = Qt::UserRole,
    };

public:
    AlchemyMaterialModel(const AlchemyMaterialList& materials);
    AlchemyMaterialModel(const QList<AlchemyMaterial*>& materials);

    virtual QVariant data(const QModelIndex& index, int role) const override;

    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual QModelIndex parent(const QModelIndex& child) const override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;

    virtual void sort(int column, Qt::SortOrder order) override;

    /**
     * 通过行号来获取一个炼金材料实例.
     */
    const AlchemyMaterial* getAlchemyMaterialByRowNumber(int row);
};

#endif // ALCHEMY_MATERIAL_MODEL_H
