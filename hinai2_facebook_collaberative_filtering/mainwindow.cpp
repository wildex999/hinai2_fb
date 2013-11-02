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

    net->setToken("CAACEdEose0cBAD7HYYUYUZAzQuG6ge26PENi5Q0OE1YcpGdZAAUkX9g2XljwwZBZCWOxWtyMsEeF39IjJklcHPFkBhUz4FvcRwFsMvX86o8BaihFA0Lz7wbuYvEWed21aZBEiTwRBAa8ZA7hBr195xDq8oDk9DukN1QsoZCfbpO7jhwQZAId3Y3qnMo24egIfcMZD");
    net->addGetGraphJob("&fields=posts", "expertnorge");

    net->addGetFacebookAboutPersonPage("aleksanderpedersen");

    
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
