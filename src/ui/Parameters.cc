#include "Parameters.h"
#include "ui_Parameters.h"

Parameters::Parameters(const QString& title, QAction* action, QWidget *parent) : 
    QGCDockWidget(title, action, parent),
    ui(new Ui::Parameters)
{
    ui->setupUi(this);
    
}

Parameters::~Parameters()
{
    delete ui;
}
