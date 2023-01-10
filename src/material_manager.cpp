// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: MIT

#include "material_manager.h"
#include <QDebug>
#include <QSet>

MaterialManager::~MaterialManager() {
    // 清除材料及配方
    clearMaterial();

    // 清除其余配方.
    for (auto& recipe : containsRecipes) {
        delete recipe;
    }
    containsRecipes.clear();
}

bool MaterialManager::addMaterial(const AlchemyMaterial* material) {
    if (selectedMaterials.contains(material)) {
        return false;
    }

    AlchemyRecipeList newRecipes;
    // 新添加的材料与原有的材料二元组合成二元配方.
    for (const AlchemyMaterial* m : selectedMaterials) {
        AlchemyRecipe* recipe = new AlchemyRecipe(material, m);
        if (!recipe->isRedundant()) {
            newRecipes << recipe;
        } else {
            delete recipe;
        }
    }

    // 新添加的材料与已有配方中的材料组合组成三元配方.
    for (const auto* r : recipes) {
        AlchemyMaterialList materialList(r->getMaterials());
        materialList << material;
        AlchemyRecipe* recipe = new AlchemyRecipe(materialList);
        if (!recipe->isRedundant()) {
            newRecipes << recipe;
        } else {
            delete recipe;
        }
    }

    // 添加新加入的配方.
    recipes << newRecipes;
    selectedMaterials.append(material);
    newRecipes.clear();
    return true;
}

bool MaterialManager::addMaterial(const QString& id) {
    return addMaterial(AlchemyMaterial::getAlchemyMaterialById(id));
}

void MaterialManager::removeMaterial(const AlchemyMaterial* material) {
    if (!selectedMaterials.contains(material)) {
        return;
    }

    // 在配方中寻找出包含要移除的材料的配方并删除.
    AlchemyRecipeList needRemove;
    for (auto* recipe : recipes) {
        if (recipe->getMaterials().contains(material)) {
            needRemove.append(recipe);
        }
    }

    for (auto* recipe : needRemove) {
        if (recipes.removeOne(recipe)) {
            delete recipe;
        }
    }
    selectedMaterials.removeOne(material);
}

void MaterialManager::removeMaterial(const QString& id) {
    removeMaterial(AlchemyMaterial::getAlchemyMaterialById(id));
}

void MaterialManager::addAllMaterial() {
    auto all = AlchemyMaterial::getAlchemyMaterials().values();
    for (auto& m : all) {
        addMaterial(m);
    }
}

void MaterialManager::clearMaterial() {
    for (auto* recipe : recipes) {
        delete recipe;
    }
    recipes.clear();
    selectedMaterials.clear();
}

const MaterialManager::AlchemyRecipeList& MaterialManager::getAlchemyRecipeList() {
    return recipes;
}

const MaterialManager::AlchemyRecipeList & MaterialManager::getContainsRecipes() {
    return containsRecipes;
}


const AlchemyMaterialList & MaterialManager::getSelectedMaterials() {
    return selectedMaterials;
}

void MaterialManager::calculateContainsRecipes() {
    // 清除之前计算的配方.
    for (auto& recipe : containsRecipes) {
        if (recipe) {
            delete recipe;
        }
    }
    containsRecipes.clear();
    if (selectedMaterials.count() <= 0 && selectedMaterials.count() > 3) {
        return;
    }
    if (selectedMaterials.count() == 3) {
        AlchemyRecipe* recipe = new AlchemyRecipe(selectedMaterials);
        if (!recipe->isRedundant()) {
            containsRecipes << recipe;
        } else {
            delete recipe;
        }
        return;
    }

    auto allMaterials = AlchemyMaterial::getAlchemyMaterials().values();
    // 搜寻两个材料组成的配方.
    if (selectedMaterials.count() == 2) {
        QSet<QString> includeEffects;
        for (auto& m : selectedMaterials) {
            for (auto& id : m->includeEffectId) {
                includeEffects << id;
            }
        }

        for (auto& m : allMaterials) {
            bool flag = false;
            for (auto& id : m->includeEffectId) {
                if (includeEffects.contains(id)) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                AlchemyRecipe* recipe = new AlchemyRecipe(AlchemyMaterialList(selectedMaterials) << m);
                if (!recipe->isRedundant()) {
                    containsRecipes << recipe;
                } else {
                    delete recipe;
                }
            }
        }
        return;
    }

    if (selectedMaterials.count() == 1) {
        QSet<QString> recipesId;
        qDebug() << "[MaterialManager] selectedMaterials.size() == 1";
        // 如果已选择的材料仅仅只有一个, 则遍历所有可能组成二元与三元配方.
        auto m = selectedMaterials[0];
        for (int i = 0; i < allMaterials.count(); ++i) {
            if (allMaterials[i] == m) {
                continue;
            }
            AlchemyMaterialList list = {m, allMaterials[i]};
            AlchemyRecipe* recipe = new AlchemyRecipe(list);
            if (!recipe->isRedundant() && !recipesId.contains(recipe->id)) {
                containsRecipes << recipe;
                recipesId << recipe->id;
            } else {
                delete recipe;
            }
            for (int j = 0; j < allMaterials.count(); ++j) {
                if (allMaterials[j] == m) {
                    continue;
                }
                list << allMaterials[j];

                recipe = new AlchemyRecipe(list);
                if (!recipe->isRedundant()) {
                    containsRecipes << recipe;
                } else {
                    delete recipe;
                }
            }
        }
        recipesId.clear();
    }
}
