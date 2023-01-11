#include "skyrimalchemy.h"
#include "ui_skyrimalchemy.h"
#include <QDebug>
#include <QMessageBox>

SkyrimAlchemy::SkyrimAlchemy(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SkyrimAlchemy),
    materialManager(new MaterialManager) {
    ui->setupUi(this);

    materialSearchMode = ui->actionMaterialSearchMode->isChecked();

    connect(ui->materialNameSearchEdit, SIGNAL(returnPressed()),
            this, SLOT(onMaterialNameSearchEditReturnPressed()));
    connect(ui->materialSearchBtn, SIGNAL(clicked()),
            this, SLOT(onMaterialSearchBtnClicked()));
    connect(ui->pushButtonContainsMaterial, SIGNAL(clicked()),
            this, SLOT(onPushButtonContainsMaterialClicked()));
    connect(ui->actionAddAllMaterial, SIGNAL(triggered()),
            this, SLOT(onActionAddAllMaterialClicked()));
    connect(ui->actionRemoveAllSelectedMaterial, SIGNAL(triggered()),
            this, SLOT(onActionRemoveAllSelectedMaterialClicked()));
    connect(ui->actionHelp, SIGNAL(triggered()),
            this, SLOT(onActionHelpClicked()));
    connect(ui->actionMaterialSearchMode, SIGNAL(triggered()),
            this, SLOT(onActionMaterialSearchModeClicked()));
    connect(ui->checkBoxSelectedMaterial, SIGNAL(stateChanged(int)),
            this, SLOT(onCheckBoxSelectedMaterialStateChanged(int)));
    connect(ui->tableAllMaterial, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(onAllMaterialTableDoubleClicked(const QModelIndex&)));
    connect(ui->tableSelectedMaterial, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(onSelectedMaterialTableDoubleClicked(const QModelIndex&)));

    searchEffectModel = new AlchemyEffectModel(
                AlchemyEffect::getAlchemyEffects().values());
    ui->materialEffectSearchEdit->setModel(searchEffectModel);
    refreshAllMaterialTable();
}

SkyrimAlchemy::~SkyrimAlchemy() {

    if (alchemyRecipeModel) {
        alchemyRecipeModel->deleteLater();
    }
    if (selectedMaterialModel) {
        selectedMaterialModel->deleteLater();
    }
    if (allMaterialModel) {
        allMaterialModel->deleteLater();
    }
    if (materialManager) {
        materialManager->deleteLater();
    }
}

void SkyrimAlchemy::refreshAllMaterialTable(const QString& searchName,
                                            const QString& searchEffectId) {

    if (!allMaterialModel) {
        allMaterialModel = new AlchemyMaterialModel(
            AlchemyMaterial::getAlchemyMaterials().values()
        );
        ui->tableAllMaterial->setModel(allMaterialModel);
        ui->tableAllMaterial->resizeColumnsToContents();
        ui->tableAllMaterial->resizeRowsToContents();
    }
    // ui->tableAllMaterial->setModel(nullptr);
    allMaterialModel->search(searchName, searchEffectId,
                             ui->checkBoxSelectedMaterial->isChecked());
    ui->tableAllMaterial->resizeRowsToContents();
    ui->tableAllMaterial->sortByColumn(0, Qt::SortOrder::AscendingOrder);
    // ui->tableAllMaterial->setModel(allMaterialModel);
    // if (allMaterialModel) {
    //     ui->tableAllMaterial->model()->deleteLater();
    //     ui->tableAllMaterial->setModel(nullptr);
    //     delete allMaterialModel;
    // }

    // if (searchName.isEmpty() && searchEffectId.isEmpty()) {
    //     allMaterialModel = new AlchemyMaterialModel(
    //         AlchemyMaterial::getAlchemyMaterials().values()
    //     );
    // } else {
    //     auto all = AlchemyMaterial::getAlchemyMaterials().values();
    //     AlchemyMaterialList pickedOut;
    //     for (const auto& m : all) {
    //         bool picked = true;
    //         if (!searchName.isEmpty() && !m->name.isLike(searchName)) {
    //             picked = false;
    //         }
    //         if (!searchEffectId.isEmpty() &&
    //             !m->includeEffectId.contains(searchEffectId)) {
    //             picked = false;
    //         }
    //         if (picked) {
    //             pickedOut << m;
    //         }
    //     }
    //     allMaterialModel = new AlchemyMaterialModel(pickedOut);
    // }
    // ui->tableAllMaterial->setModel(allMaterialModel);
    // ui->tableAllMaterial->resizeColumnsToContents();
    // ui->tableAllMaterial->resizeRowsToContents();
}

void SkyrimAlchemy::refreshSelectedMaterialTable() {
    if (materialSearchMode) {
        refreshRecipeTable(true);
        ui->tableSelectedMaterial->setModel(alchemyRecipeModel);
        ui->tableSelectedMaterial->resizeColumnsToContents();
        ui->tableSelectedMaterial->resizeRowsToContents();
        ui->tableSelectedMaterial->sortByColumn(4, Qt::SortOrder::DescendingOrder);
        return;
    }
    int currentRow = -1;
    if (selectedMaterialModel) {
        if (ui->tableSelectedMaterial->currentIndex().isValid()) {
            currentRow = ui->tableSelectedMaterial->currentIndex().row();
        }
        ui->tableSelectedMaterial->model()->deleteLater();
        ui->tableSelectedMaterial->setModel(nullptr);
        delete selectedMaterialModel;
    }

    selectedMaterialModel =
            new AlchemyMaterialModel(materialManager->getSelectedMaterials());
    ui->tableSelectedMaterial->setModel(selectedMaterialModel);
    if (currentRow != -1) {
        ui->tableSelectedMaterial->selectRow(currentRow);
    } else {
        ui->tableSelectedMaterial->scrollToBottom();
    }

    ui->tableSelectedMaterial->resizeColumnsToContents();
    ui->tableSelectedMaterial->resizeRowsToContents();
    refreshRecipeTable();
}

void SkyrimAlchemy::refreshRecipeTable(const bool& containsRecipes) {
    if (alchemyRecipeModel) {
        ui->tableAlchemyResult->model()->deleteLater();
        ui->tableAlchemyResult->setModel(nullptr);
        delete alchemyRecipeModel;
    }
    if (containsRecipes) {
        materialManager->calculateContainsRecipes();
        alchemyRecipeModel = new AlchemyRecipeModel(
            materialManager->getContainsRecipes()
        );
    } else {
        alchemyRecipeModel = new AlchemyRecipeModel(
            materialManager->getAlchemyRecipeList()
        );
    }

    ui->tableAlchemyResult->setModel(alchemyRecipeModel);

    ui->tableAlchemyResult->resizeColumnsToContents();
    ui->tableAlchemyResult->resizeRowsToContents();
}



void SkyrimAlchemy::onMaterialSearchBtnClicked() {
    QString name = ui->materialNameSearchEdit->text();
    auto effect = this->searchEffectModel->getAlchemyEffectByRowNumber(
        ui->materialEffectSearchEdit->currentIndex()
    );
    QString effectId = effect ? effect->id : "";
    refreshAllMaterialTable(name, effectId);
}

void SkyrimAlchemy::onAllMaterialTableDoubleClicked(const QModelIndex& index) {
    if (index.isValid()) {
        const AlchemyMaterial* material =
                allMaterialModel->getAlchemyMaterialByRowNumber(index.row());
        if (material) {
            if (materialManager->addMaterial(material)) {
                allMaterialModel->highlight(material);
            } else {
                materialManager->removeMaterial(material);
                allMaterialModel->removeHighlight(material);
            }
            refreshSelectedMaterialTable();
        }
    }
}

void SkyrimAlchemy::onSelectedMaterialTableDoubleClicked(const QModelIndex& index) {
    if (materialSearchMode) {
        auto recipe = alchemyRecipeModel->getAlchemyRecipeByRowNumber(index.row());
        int idx = index.column() - 1;
        if (recipe->getMaterials().size() > idx) {
            auto material = recipe->getMaterials()[idx];
            idx = allMaterialModel->getAlchemyMaterialRow(material);
            ui->tableAllMaterial->selectRow(idx);
        }
        return;
    }
    if (index.isValid()) {
        const AlchemyMaterial* material =
                selectedMaterialModel->getAlchemyMaterialByRowNumber(index.row());
        if (material) {
            materialManager->removeMaterial(material);
            allMaterialModel->removeHighlight(material);
            refreshSelectedMaterialTable();
        }
    }
}

void SkyrimAlchemy::onPushButtonContainsMaterialClicked() {
    refreshRecipeTable(true);
}

void SkyrimAlchemy::onActionAddAllMaterialClicked() {
    materialManager->addAllMaterial();
    allMaterialModel->highlightAll();
    refreshSelectedMaterialTable();
}

void SkyrimAlchemy::onActionRemoveAllSelectedMaterialClicked() {
    materialManager->clearMaterial();
    allMaterialModel->resetHighlight();
    refreshSelectedMaterialTable();
}

void SkyrimAlchemy::resizeEvent(QResizeEvent* event) {
    ui->tabAlchemy->tabBar()->setFixedWidth(width());
}

void SkyrimAlchemy::onMaterialNameSearchEditReturnPressed() {
    onMaterialSearchBtnClicked();
}

void SkyrimAlchemy::onActionMaterialSearchModeClicked() {
    materialSearchMode = ui->actionMaterialSearchMode->isChecked();
    refreshSelectedMaterialTable();
}

void SkyrimAlchemy::onCheckBoxSelectedMaterialStateChanged(int state) {
    refreshAllMaterialTable();
}


void SkyrimAlchemy::onActionHelpClicked() {
    QMessageBox::about(this, "帮助 & 关于",
        "作者: SmileYik\n"
        "项目主页: https://github.com/SmileYik/SkyrimAlchemy\n"
        "使用说明: \n"
        "在Material面板中会显示出游戏内的所有原料, 双击即可选择它作为"
        "材料, 并且显示在下方表格中, 同时将会根据下方表格中的原料来计算"
        "所有可能组成的炼金配方, 这些配方将在Alchemy面板中显示.\n"
        "如果你想取消掉一些材料, 那么你可以双击下方表格中的材料即可取消"
        "选择此材料作为原料.\n"
        "如果你仅想查看包含你所选择的材料的炼金配方, 你可以在Alchemy"
        "面板中点击此面板中的计算按钮即可."
    );
}
