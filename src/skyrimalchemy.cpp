#include "skyrimalchemy.h"
#include "ui_skyrimalchemy.h"

SkyrimAlchemy::SkyrimAlchemy(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::SkyrimAlchemy)
{
    m_ui->setupUi(this);
}

SkyrimAlchemy::~SkyrimAlchemy() = default;
