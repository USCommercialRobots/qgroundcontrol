/****************************************************************************
 *
 *   (c) 2009-2018 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#pragma once

#include <QWidget>

#include "Vehicle.h"

namespace Ui {
class PlannerConfiguration;
}

class PlannerConfiguration : public QWidget
{
    Q_OBJECT
    
public:
    PlannerConfiguration(Vehicle* vehicle, QWidget *parent = 0);

public slots:
    /** @brief Receive status message */
//    void receiveStatusMessage(const QString& message);
//    void setVersion(QString version);

private slots:
//    void on_simComboBox_currentIndexChanged(int index);

private:
    Vehicle* _vehicle;
    
    Ui::PlannerConfiguration *ui;
};

