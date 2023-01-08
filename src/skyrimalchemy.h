#ifndef SKYRIMALCHEMY_H
#define SKYRIMALCHEMY_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class SkyrimAlchemy;
}

class SkyrimAlchemy : public QMainWindow
{
    Q_OBJECT

public:
    explicit SkyrimAlchemy(QWidget *parent = nullptr);
    ~SkyrimAlchemy() override;

private:
    QScopedPointer<Ui::SkyrimAlchemy> m_ui;
};

#endif // SKYRIMALCHEMY_H
