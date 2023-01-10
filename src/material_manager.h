// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <qobject.h>
#include "alchemy_effect.h"
#include "alchemy_material.h"
#include "alchemy_recipe.h"

/**
 * 炼金材料管理.
 */
class MaterialManager : public QObject {
    Q_OBJECT
private:
    typedef QList<AlchemyRecipe*> AlchemyRecipeList;
    AlchemyMaterialList selectedMaterials;  // 已选择的材料
    AlchemyRecipeList recipes;              // 当前已选择的材料能够组成的配方
    AlchemyRecipeList containsRecipes;      // 所有包含当前已选择的材料的配方
public:
    ~MaterialManager();

    /**
     * 添加一个材料, 如果已有这个材料则返回false.
     */
    bool addMaterial(const QString& id);

    /**
     * 添加一个材料, 如果已有这个材料则返回false.
     */
    bool addMaterial(const AlchemyMaterial* material);

    /**
     * 通过材料id移除一个材料
     */
    void removeMaterial(const QString& id);

    /**
     * 移除一个材料.
     */
    void removeMaterial(const AlchemyMaterial* material);

    /**
     * 清除当前已选择的所有材料
     */
    void clearMaterial();

    /**
     * 添加当前程序已经加载的所有材料
     */
    void addAllMaterial();

    /**
     * 计算包含当前材料的所有配方, 当前材料可能的个数只有1~3(包含两端)个.
     */
    void calculateContainsRecipes();

    /**
     * 返回已经选择的所有材料
     */
    const AlchemyMaterialList& getSelectedMaterials();

    /**
     * 返回当前材料可以组成的所有配方
     */
    const AlchemyRecipeList& getAlchemyRecipeList();

    /**
     * 返回包含当前材料的所有配方
     */
    const AlchemyRecipeList& getContainsRecipes();
};

#endif // MATERIAL_MANAGER_H
