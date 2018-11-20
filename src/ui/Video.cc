#include "Video.h"
#include "ui_Video.h"

Video::Video(const QString& title, QAction* action, QWidget *parent) : 
    QGCDockWidget(title, action, parent),
    ui(new Ui::Video)
{
    ui->setupUi(this);    
    
}

Video::~Video()
{
    delete ui;
}
