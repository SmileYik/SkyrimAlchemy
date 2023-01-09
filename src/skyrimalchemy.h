#ifndef SKYRIMALCHEMY_H
#define SKYRIMALCHEMY_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QPointer>
#include "alchemy_material_model.h"
#include "alchemy_effect_model.h"
#include "alchemy_recipe_model.h"
#include "material_manager.h"

namespace Ui {
class SkyrimAlchemy;
}

class SkyrimAlchemy : public QMainWindow
{
    Q_OBJECT

public:
    explicit SkyrimAlchemy(QWidget *parent = nullptr);
    ~SkyrimAlchemy() override;

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    QScopedPointer<Ui::SkyrimAlchemy> ui;
    QScopedPointer<MaterialManager> materialManager;
    QPointer<AlchemyMaterialModel> allMaterialModel;
    QPointer<AlchemyMaterialModel> selectedMaterialModel;
    QPointer<AlchemyEffectModel> searchEffectModel;
    QPointer<AlchemyRecipeModel> alchemyRecipeModel;


    void refreshAllMaterialTable(const QString& searchName = "",
                                 const QString& searchEffect = "");
    void refreshSelectedMaterialTable();
    void refreshRecipeTable(const bool& containsRecipes = false);

public slots:
    void onMaterialSearchBtnClicked();
    void onAllMaterialTableDoubleClicked(const QModelIndex &index);
    void onSelectedMaterialTableDoubleClicked(const QModelIndex& index);
    void onPushButtonContainsMaterialClicked();
    void onActionAddAllMaterialClicked();
    void onActionRemoveAllSelectedMaterialClicked();
    void onMaterialNameSearchEditReturnPressed();
};

#endif // SKYRIMALCHEMY_H
