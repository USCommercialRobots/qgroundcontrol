#pragma once

#include "QGCDockWidget.h"
#include "Vehicle.h"

namespace Ui {
    class Video;
}

class Video : public QGCDockWidget
{
    Q_OBJECT

public:
    explicit Video(const QString& title, QAction* action, QWidget *parent = 0);
    ~Video();

private:
    Ui::Video *ui;
};