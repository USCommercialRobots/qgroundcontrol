#include "Planner.h"
#include "ui_Planner.h"

Planner::Planner(const QString& title, QAction* action, QWidget *parent) : 
    QGCDockWidget(title, action, parent),
    ui(new Ui::Planner)
{
    ui->setupUi(this);    
    
}

Planner::~Planner()
{
    delete ui;
}
