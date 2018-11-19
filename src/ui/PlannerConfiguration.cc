/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include <QSettings>

#include "PlannerConfiguration.h"
#include "ui_PlannerConfiguration.h"

#include "UAS.h"

PlannerConfiguration::PlannerConfiguration(Vehicle* vehicle, QWidget *parent)
    : QWidget(parent)
    , _vehicle(vehicle)
    , ui(new Ui::PlannerConfiguration)
{
    ui->setupUi(this);

    // XXX its quite wrong that this is implicitely a factory
    // class, but this is something to clean up for later.

    QSettings settings;
    settings.beginGroup("QGC_HILCONFIG");
    int i = settings.value("SIMULATOR_INDEX", -1).toInt();

    if (i > 0) {
//        ui->simComboBox->blockSignals(true);
        ui->simComboBox->setCurrentIndex(i);
    }

    settings.endGroup();
}

