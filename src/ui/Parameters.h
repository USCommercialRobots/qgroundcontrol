#pragma once

#include "QGCDockWidget.h"
#include "Vehicle.h"

namespace Ui {
    class Parameters;
}

class Parameters : public QGCDockWidget
{
    Q_OBJECT

public:
    explicit Parameters(const QString& title, QAction* action, QWidget *parent = 0);
    ~Parameters();

private:
    Ui::Parameters *ui;
};