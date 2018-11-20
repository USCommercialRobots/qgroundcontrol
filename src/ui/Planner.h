#pragma once

#include "QGCDockWidget.h"
#include "Vehicle.h"

namespace Ui {
    class Planner;
}

class Planner : public QGCDockWidget
{
    Q_OBJECT

public:
    explicit Planner(const QString& title, QAction* action, QWidget *parent = 0);
    ~Planner();

private:
    Ui::Planner *ui;
};