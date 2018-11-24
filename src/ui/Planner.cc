#include "Planner.h"
#include "ui_Planner.h"
#include "MultiVehicleManager.h"
#include "QGCApplication.h"
#include "Vehicle.h"
#include <QPushButton>
#include <QScrollArea>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <string>
#include <unistd.h>
#include <QtConcurrent>


const unsigned int Planner::updateInterval = 1000U; // 1 second


Planner::Planner(const QString& title, QAction* action, QWidget *parent) : 
    QGCDockWidget(title, action, parent),
    ui(new Ui::Planner)
{
    _count = 0;
    ui->setupUi(this);
    pmc = new PlanMasterController(this);
    pmc->start(true);

    _multiVehicleManager = qgcApp()->toolbox()->multiVehicleManager();

    connect(_multiVehicleManager, &MultiVehicleManager::activeVehicleChanged, this, &Planner::_activeVehicleChanged);
    connect(_multiVehicleManager, &MultiVehicleManager::vehicleAdded, this, &Planner::_vehicleAdded);
    connect(_multiVehicleManager, &MultiVehicleManager::vehicleRemoved, this, &Planner::_vehicleRemoved);
    connect(this, &Planner::changeActiveSignal, this, &Planner::_changeActive);

    vehiclesList = new QWidget;
    vehiclesList->setObjectName("vehs");
    vehiclesList->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    vehiclesList->setLayout(new QVBoxLayout(vehiclesList));
    ui->scrollArea->setWidget(vehiclesList);

    init();
    connect(&updateTimer, &QTimer::timeout, this, &Planner::refresh);
    updateTimer.start(updateInterval);
}

Planner::~Planner()
{
    delete ui;
}

void Planner::init(void)
{
    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();

    for (int i=0; i<vehicles->count(); i++) {
        _vehicleAdded(qobject_cast<Vehicle*>(vehicles->get(i)));
    }

    _count = vehicles->count();
}

void Planner::_loadMission(int k)
{
    qDebug() << "~~~~~~~ load Mission slot:  k = " << k;

    QLayout *vehiclesLayout = vehiclesList->layout();

    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();

    for (int i=0; i<vehicles->count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles->get(i));
        if (vehicle->id() == k){

//            int index = -1;
//            for (int j = 0; j<_count; j++) {
//                if (_ids[j] == vehicle->id()){
//                    index = j;
//                }
//            }
//            QGroupBox *row = qobject_cast<QGroupBox*>(vehiclesLayout->itemAt(i)->widget());
//            QPushButton *actB = qobject_cast<QPushButton*>(row->layout()->itemAt(2)->widget());
//            actB->setText("Job time: ");
//            QMetaObject::invokeMethod(, "clicked");

            qDebug() << "~~~~~ load k = " << k;

            qDebug() << "2. load from file ";
            pmc->loadFromFile("/home/azat/ground/planqgc.plan");
            sleep(1);
            qDebug() << "3. send to vehicle ";
            pmc->sendToVehicle();
        }
    }
}


void Planner::_changeActive(int k)
{
    qDebug() << "~~~~~~~~~~~ change Active slot:  k = " << k;

    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();
    for (int i=0; i<vehicles->count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles->get(i));
        if (vehicle->id() == k){
            qDebug() << "~~~~~ active k = " << 4;
            qDebug() << "1. set active ";

            _multiVehicleManager->setActiveVehicle(vehicle);
        }
    }


//    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();

//    for (int i=0; i<vehicles->count(); i++) {
//        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles->get(i));
//        if (vehicle->id() == k){
//            qDebug() << "~~~~~ active k = " << k;
//            qDebug() << "pmc offline = " << pmc->offline();
//            sleep(3);
//            qDebug() << "1. set active ";
//            _multiVehicleManager->setActiveVehicle(vehicle);
//        }
//    }
}


void Planner::_vehicleRemoved(Vehicle* vehicle)
{
    int id = vehicle->id();
    std::string str = std::to_string(id);
    qDebug() << "+++ vehicle removed: ";
}

void Planner::_vehicleAdded(Vehicle* vehicle)
{
    qDebug() << "+++ vehicle added: (new row for " << vehicle->id() << ")";

    QLayout *vehiclesLayout = vehiclesList->layout();

    int id = vehicle->id();
    char name [20];
    sprintf(name, "%s%d", "Vehicle ", id);
    QGroupBox *row = new QGroupBox(tr(name));

    // 1. button Open
    QSignalMapper *signalMapperArm = new QSignalMapper(this);
    connect(signalMapperArm, SIGNAL(mapped(int)), this, SLOT(_loadMission(int)));

    QPushButton *buttonArm = new QPushButton("Load Mission");
    signalMapperArm->setMapping(buttonArm, id);
    connect(buttonArm, SIGNAL(clicked()), signalMapperArm, SLOT(map()));



    QSignalMapper *signalMapperArm2 = new QSignalMapper(this);
    connect(signalMapperArm2, SIGNAL(mapped(int)), this, SLOT(_changeActive(int)));

    QPushButton *buttonChangeActive = new QPushButton("Activate");
    signalMapperArm2->setMapping(buttonChangeActive, id);
    connect(buttonChangeActive, SIGNAL(clicked()), signalMapperArm2, SLOT(map()));



    QGridLayout *layout = new QGridLayout;

    layout->addWidget(buttonArm, 0, 0);
    layout->addWidget(buttonChangeActive, 0, 1);

    row->setLayout(layout);
    row->show();
    vehiclesLayout->addWidget(row);

    _ids[_count] = id;
    _count++;
}

void Planner::_activeVehicleChanged(Vehicle* vehicle)
{
    qDebug() << "+++ active vehicle changed " << vehicle->id();
    qDebug() << "+++ pmc offline = " << pmc->offline();
}


void Planner::refresh(){

    qDebug() << "$$$ refr pmc offline = " << pmc->offline();
}
