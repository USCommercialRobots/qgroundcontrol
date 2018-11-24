#pragma once

#include "QGCDockWidget.h"
#include "Vehicle.h"
#include "QGCApplication.h"

namespace Ui {
    class Parameters;
}

class Parameters : public QGCDockWidget
{
    Q_OBJECT

public:
    explicit Parameters(const QString& title, QAction* action, QWidget *parent = 0);
    ~Parameters();

    void init(void);

public slots:
    void refresh();

protected:
    QTimer updateTimer;
    QWidget* vehiclesList;
    MultiVehicleManager* _multiVehicleManager;
    int _ids[1000];
    int _count;
    static const unsigned int updateInterval;

private slots:
    void _vehicleAdded(Vehicle* vehicle);
    void _vehicleRemoved(Vehicle* vehicle);
    void _activeVehicleChanged(Vehicle* vehicle);
    void _setArmed(int k);
    void _setDisarmed(int k);
    void _setAuto(int k);
    void _setHold(int k);

private:
    Ui::Parameters *ui;

};


