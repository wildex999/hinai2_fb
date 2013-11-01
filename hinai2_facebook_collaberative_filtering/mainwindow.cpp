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
    parser->addProduct("Oculus Rift", oculuskeywords);

    net->setToken("CAACEdEose0cBAHoWNSG4ZABmyKmaZBbKId3LT4KuaB2SNZB1xP04FYAjqoeVJNWTqrmEqflJ7xeR6lnDLTZAmUkJP2Tx5PEQO1aAMt01owQC4IB3ryyknXFaRZAOtOXZADxiTuzJstmikwDTxiCStxUdZCsLRuEAptzmMLePc9STMmPhTh8Q0HFPXSFGgXHQd0ZD");
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
