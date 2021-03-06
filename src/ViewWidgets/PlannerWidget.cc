/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include "PlannerWidget.h"

PlannerWidget::PlannerWidget(const QString& title, QAction* action, QWidget *parent) :
    QGCQmlWidgetHolder(title, action, parent)
{
    Q_UNUSED(title);
    Q_UNUSED(action);
	setSource(QUrl::fromUserInput("qrc:/qml/PlannerWidget.qml"));
    
    loadSettings();
}
