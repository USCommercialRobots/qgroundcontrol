#pragma once

#include "QGCDockWidget.h"
#include "Vehicle.h"
#include "QGCApplication.h"
#include "PlanMasterController.h"

namespace Ui {
    class Planner;
}

class Planner : public QGCDockWidget
{
    Q_OBJECT
    QThread thread;

public:
    explicit Planner(const QString& title, QAction* action, QWidget *parent = 0);
    ~Planner();

    void init(void);

public slots:
    void refresh();

signals:
    void changeActiveSignal (int k);

protected:
    QTimer updateTimer;
    static const unsigned int updateInterval;
    int _ids[1000];
    int _count;
    QWidget* vehiclesList;
    MultiVehicleManager* _multiVehicleManager;
    PlanMasterController *pmc;

private slots:
    void _vehicleAdded(Vehicle* vehicle);
    void _vehicleRemoved(Vehicle* vehicle);
    void _activeVehicleChanged(Vehicle* vehicle);
    void _loadMission(int k);
    void _changeActive(int k);

private:
    Ui::Planner *ui;
};


