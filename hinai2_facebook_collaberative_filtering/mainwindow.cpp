#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "collaberativefiltering.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CollaberativeFiltering cf;
    cf.addToTable(male,ipad_air);
    cf.addToTable(male,assasins_creed_4);
    cf.addToTable(male,ipad_mini);
    cf.addToTable(male,ipad_mini);
    cf.addToTable(male,ipad_mini);
    cf.addToTable(age30_40,microsoft_surface_rt_64_gb);

    cf.makeCalculations();

    cf.writeToDebug();

}

MainWindow::~MainWindow()
{
    delete ui;
}
