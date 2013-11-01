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

    net->setToken("CAACEdEose0cBAM6onUfhPV9JBwLLBMekp3e5GFSHHw5j84wGnuxcVyPN4To7jcZAoKqnzb8hoom8sVNTUwTMryTbcN9WPo23fHZBdBXHHdEhSOUe1Yc6CR28jziZAHj1gdJPND98ZBgp3JZC1wXf7WkTlZBJPRcqnJ6PbTDPF3TRWjyFChW5pgI3vN4ormDi4ZD");
    net->addGetGraphJob("&fields=posts", "expertnorge");
    
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
