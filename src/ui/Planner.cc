/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include "Planner.h"
#include "PlannerConfiguration.h"

Planner::Planner(const QString& title, QAction* action, QWidget *parent)
    : MultiVehicleDockWidget(title, action, parent)
{
    init();
    
    loadSettings();
}

QWidget* Planner::_newVehicleWidget(Vehicle* vehicle, QWidget* parent)
{
    return new PlannerConfiguration(vehicle, parent);
}
