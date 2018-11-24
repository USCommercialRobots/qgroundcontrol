#include "Parameters.h"
#include "ui_Parameters.h"
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


const unsigned int Parameters::updateInterval = 1000U; // 1 second

Parameters::Parameters(const QString& title, QAction* action, QWidget *parent) : 
    QGCDockWidget(title, action, parent),
    ui(new Ui::Parameters)
{
    _count = 0;
    ui->setupUi(this);
    _multiVehicleManager = qgcApp()->toolbox()->multiVehicleManager();

    connect(_multiVehicleManager, &MultiVehicleManager::activeVehicleChanged, this, &Parameters::_activeVehicleChanged);
    connect(_multiVehicleManager, &MultiVehicleManager::vehicleAdded, this, &Parameters::_vehicleAdded);
    connect(_multiVehicleManager, &MultiVehicleManager::vehicleRemoved, this, &Parameters::_vehicleRemoved);

    vehiclesList = new QWidget;
    vehiclesList->setObjectName("techarea");
    vehiclesList->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    vehiclesList->setLayout(new QVBoxLayout(vehiclesList));
    ui->scrollArea->setWidget(vehiclesList);

    init();

    connect(&updateTimer, &QTimer::timeout, this, &Parameters::refresh);
    updateTimer.start(updateInterval);
}

Parameters::~Parameters()
{
    delete ui;
}

void Parameters::init(void)
{
    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();
    for (int i=0; i<vehicles->count(); i++) {
//        _ids[i] = qobject_cast<Vehicle*>(vehicles->get(i))->id();
        _vehicleAdded(qobject_cast<Vehicle*>(vehicles->get(i)));
    }
    _count = vehicles->count();
}

void Parameters::_setArmed(int k)
{
    qDebug() << "--- arm slot:  k = " << k;

    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();

    for (int i=0; i<vehicles->count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles->get(i));
        if (vehicle->id() == k){
            qDebug() << "--- arm slot:  arming k = " << k;
            vehicle->setArmed(true);
        }
    }
}

void Parameters::_setDisarmed(int k)
{
    qDebug() << "--- disarm slot:  k = " << k;

    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();

    for (int i=0; i<vehicles->count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles->get(i));
        if (vehicle->id() == k){
            qDebug() << "--- disarm slot:  arming k = " << k;
            vehicle->setArmed(false);
        }
    }
}

void Parameters::_setAuto(int k)
{
    qDebug() << "--- disarm slot:  k = " << k;

    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();

    for (int i=0; i<vehicles->count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles->get(i));
        if (vehicle->id() == k){
            qDebug() << "--- disarm slot:  arming k = " << k;
            vehicle->setFlightMode("Auto");
        }
    }
}

void Parameters::_setHold(int k)
{
    qDebug() << "--- disarm slot:  k = " << k;

    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();

    for (int i=0; i<vehicles->count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles->get(i));
        if (vehicle->id() == k){
            qDebug() << "--- disarm slot:  arming k = " << k;
            vehicle->setFlightMode("Hold");
        }
    }
}

void Parameters::_vehicleRemoved(Vehicle* vehicle)
{
    int id = vehicle->id();
    std::string str = std::to_string(id);
    qDebug() << "--- vehicle removed: ";
}

void Parameters::_vehicleAdded(Vehicle* vehicle)
{
    qDebug() << "--- vehicle added: (new row for " << vehicle->id() << ")";

    QLayout *vehiclesLayout = vehiclesList->layout();

    int id = vehicle->id();
    char name [20];
    sprintf(name, "%s%d", "Vehicle ", id);
    QGroupBox *row = new QGroupBox(tr(name));


    // 1. job time
    QString jobTime = vehicle->getFact("flightTime")->rawValueString();
    QString jobTimeUnits = vehicle->getFact("flightTime")->rawUnits();
    QLabel *jobTimeLabel = new QLabel(this);
    jobTimeLabel->setText("Job time: " + jobTime + " " + jobTimeUnits);

    // 2. ground speed
    QString groundSpeed = vehicle->getFact("groundSpeed")->rawValueString();
    QString groundSpeedUnits = vehicle->getFact("groundSpeed")->rawUnits();
    QLabel *speedLabel = new QLabel(this);
    speedLabel->setText("Ground speed: " + groundSpeed + " " + groundSpeedUnits);

    // 3. mode
    QString mode = vehicle->flightMode();
    QLabel *modeLabel = new QLabel(this);
    modeLabel->setText("Mode: " + mode);


    // 4. button Arm
    QSignalMapper *signalMapperArm = new QSignalMapper(this);
    connect(signalMapperArm, SIGNAL(mapped(int)), this, SLOT(_setArmed(int)));

    QPushButton *buttonArm = new QPushButton("Arm");
    signalMapperArm->setMapping(buttonArm, id);
    connect(buttonArm, SIGNAL(clicked()), signalMapperArm, SLOT(map()));


    // 5. button Disarm
    QSignalMapper *signalMapperDisarm = new QSignalMapper(this);
    connect(signalMapperDisarm, SIGNAL(mapped(int)), this, SLOT(_setDisarmed(int)));

    QPushButton *buttonDisarm = new QPushButton("Disarm");
    signalMapperDisarm->setMapping(buttonDisarm, id);
    connect(buttonDisarm, SIGNAL(clicked()), signalMapperDisarm, SLOT(map()));


    // 6. button Auto mode
    QSignalMapper *signalMapperAuto = new QSignalMapper(this);
    connect(signalMapperAuto, SIGNAL(mapped(int)), this, SLOT(_setAuto(int)));

    QPushButton *buttonAuto = new QPushButton("Auto");
    signalMapperAuto->setMapping(buttonAuto, id);
    connect(buttonAuto, SIGNAL(clicked()), signalMapperAuto, SLOT(map()));


    // 7. button Hold mode
    QSignalMapper *signalMapperHold = new QSignalMapper(this);
    connect(signalMapperHold, SIGNAL(mapped(int)), this, SLOT(_setHold(int)));

    QPushButton *buttonHold = new QPushButton("Hold");
    signalMapperHold->setMapping(buttonHold, id);
    connect(buttonHold, SIGNAL(clicked()), signalMapperHold, SLOT(map()));


    // 8. button Lidar on
    QPushButton *buttonLidarOn = new QPushButton("Lidar On");

    // 9. button Lidar off
    QPushButton *buttonLidarOff = new QPushButton("Lidar Off");

    // 10. button Geofence switch
    QPushButton *buttonGeofenceSwitch = new QPushButton("Geofence On/Off");

    // 11. geofence status
    QLabel *geofenceStatusLabel = new QLabel(this);
    geofenceStatusLabel->setText("Geofence Status: ");

    // 12. arm/disarm status
    QLabel *statusLabel = new QLabel(this);
    QString status = "Disarmed";
    if (vehicle->armed()) {
        status = "Armed";
    }
    statusLabel->setText("Status: " + status);

    // 13. roll
    QString roll = vehicle->getFact("roll")->rawValueString();
    QString rollUnits = vehicle->getFact("roll")->rawUnits();
    QLabel *rollLabel = new QLabel(this);
    rollLabel->setText("Roll: " + roll + " " + rollUnits);

    // 13. heading
    int heading = vehicle->getFact("heading")->rawValueString().toInt();
    QLabel *headingLabel = new QLabel(this);

    QString headingStr;
    if (heading < 30 || heading >= 330) {
        headingStr = "N";
    } else
    if (heading >= 30 && heading < 65) {
        headingStr = "NE";
    } else
    if (heading >= 65 && heading < 115) {
        headingStr = "E";
    } else
    if (heading >= 115 && heading < 145) {
        headingStr = "SE";
    } else
    if (heading >= 145 && heading < 205) {
        headingStr = "S";
    } else
    if (heading >= 205 && heading < 245) {
        headingStr = "SW";
    } else
    if (heading >= 245 && heading < 295) {
        headingStr = "W";
    } else
    if (heading >= 295 && heading < 330) {
        headingStr = "NW";
    }

    headingLabel->setText("Heading: " + headingStr);


    QGridLayout *layout = new QGridLayout;

    layout->addWidget(rollLabel, 0, 0);
    layout->addWidget(headingLabel, 0, 1);
    layout->addWidget(jobTimeLabel, 0, 2);
    layout->addWidget(statusLabel, 0, 3);
    layout->addWidget(speedLabel, 1, 0);
    layout->addWidget(modeLabel, 1, 1);
    layout->addWidget(buttonArm, 1, 2);
    layout->addWidget(buttonDisarm, 1, 3);
    layout->addWidget(buttonAuto, 2, 0);
    layout->addWidget(buttonHold, 2, 1);
    layout->addWidget(buttonLidarOn, 2, 2);
    layout->addWidget(buttonLidarOff, 2, 3);
    layout->addWidget(buttonGeofenceSwitch, 3, 0);
    layout->addWidget(geofenceStatusLabel, 3, 1);

    row->setLayout(layout);
    row->show();
    vehiclesLayout->addWidget(row);

    _ids[_count] = id;
    _count++;
}


void Parameters::refresh(){
    QmlObjectListModel* vehicles = qgcApp()->toolbox()->multiVehicleManager()->vehicles();
    QLayout *vehiclesLayout = vehiclesList->layout();

    qDebug() << "--- refresh: _count = " << _count;

    for (int i = 0; i<vehicles->count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles->get(i));
        int index = -1;
        for (int j = 0; j<_count; j++) {
            if (_ids[j] == vehicle->id()){
                index = j;
            }
        }

        qDebug() << "--- refresh: _ids[" << i << "] = " << _ids[index];

        if (index == -1){
            qDebug() << "--- refresh: Vehicle not found  id = " << vehicle->id();

        } else {
            qDebug() << "--- refresh: Edit row for  id = " << vehicle->id();

            QGroupBox *row = qobject_cast<QGroupBox*>(vehiclesLayout->itemAt(index)->widget());

            // 1. job time
            QString jobTime = vehicle->getFact("flightTime")->rawValueString();
            QString jobTimeUnits = vehicle->getFact("flightTime")->rawUnits();
            qobject_cast<QLabel*>(row->layout()->itemAt(2)->widget())->setText("Job time: " + jobTime + " " + jobTimeUnits);

            // 2. ground speed
            QString groundSpeed = vehicle->getFact("groundSpeed")->rawValueString();
            QString groundSpeedUnits = vehicle->getFact("groundSpeed")->rawUnits();
            qobject_cast<QLabel*>(row->layout()->itemAt(4)->widget())->setText("Ground speed: " + groundSpeed + " " + groundSpeedUnits);

            // 3. mode
            QString mode = vehicle->flightMode();
            qobject_cast<QLabel*>(row->layout()->itemAt(5)->widget())->setText("Mode: " + mode);


            // 11. geofence status

            // 12. arm/disarm status
            QString status = "Disarmed";
            if (vehicle->armed()) {
                status = "Armed";
            }
            qobject_cast<QLabel*>(row->layout()->itemAt(3)->widget())->setText("Status: " + status);


            // 13. roll
            QString roll = vehicle->getFact("roll")->rawValueString();
            QString rollUnits = vehicle->getFact("roll")->rawUnits();
            qobject_cast<QLabel*>(row->layout()->itemAt(0)->widget())->setText("Roll: " + roll + " " + rollUnits);

            // 13. heading
            int heading = vehicle->getFact("heading")->rawValueString().toInt();

            QString headingStr;
            if (heading < 30 || heading >= 330) {
                headingStr = "N";
            } else
            if (heading >= 30 && heading < 65) {
                headingStr = "NE";
            } else
            if (heading >= 65 && heading < 115) {
                headingStr = "E";
            } else
            if (heading >= 115 && heading < 145) {
                headingStr = "SE";
            } else
            if (heading >= 145 && heading < 205) {
                headingStr = "S";
            } else
            if (heading >= 205 && heading < 245) {
                headingStr = "SW";
            } else
            if (heading >= 245 && heading < 295) {
                headingStr = "W";
            } else
            if (heading >= 295 && heading < 330) {
                headingStr = "NW";
            }

            qobject_cast<QLabel*>(row->layout()->itemAt(1)->widget())->setText("Heading: " + headingStr);
        }

    }

}


void Parameters::_activeVehicleChanged(Vehicle* vehicle)
{
    qDebug() << "--- active vehicle changed ";
}


