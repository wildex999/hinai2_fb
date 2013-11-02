#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "networkmanager.h"
#include "collaberativefiltering.h"
#include "fbgraph_parser.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    FBGraph_Parser* parser = new FBGraph_Parser();
    NetworkManager* net = new NetworkManager(this, parser);
    parser->setNetworkManager(net);

    QString oculuskeywords[] = {"oculus rift","oculus", "lol", 0};
    parser->addProduct("Oculus Rift", Product::WearableElectornics, oculuskeywords);

    net->setToken("CAACEdEose0cBAMiMv6pFcLXlCjwa4GDmDS08CnU2wHtpaxtRchxLW3o7262kqtNZB6mSQPWFYIwv9WcXZCZCCgm65HPMrTf1fDdvZAn058UNNHBrgOpHogXW5aslkgM1tZAZA3a93gWG3xryEzI63K9byVSmrTWZBUnStvfQZBKLY5r9Y1XyZAAoYbgdPbYuGMpcZD");
    net->addGetGraphJob("&fields=posts", "expertnorge");

    //net->addGetFacebookAboutPersonPage("birgitte.haavardsholm");

    
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
