/****************************************************************************
 *
 *   (c) 2009-2018 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#pragma once

#include "MultiVehicleDockWidget.h"

class Parameters : public MultiVehicleDockWidget
{
    Q_OBJECT

public:
    explicit Parameters(const QString& title, QAction* action, QWidget *parent = 0);

protected:
    // Override from MultiVehicleDockWidget
    virtual QWidget* _newVehicleWidget(Vehicle* vehicle, QWidget* parent);
};

