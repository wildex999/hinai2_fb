#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "networkmanager.h"
#include "collaberativefiltering.h"
#include "fbgraph_parser.h"
#include <util.h>
#include <QDebug>
#include "locationTable.h"

// temp
LocationTable* locationTable;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // ================== OBS OBS ====================
    // Pass på at datafila "Fylke_og_kommuneoversikt.csv" ligger i build mappa du bruker.
    // Fila ligger i src mappa. Må kopiere den over manuelt.
    // kan også lastes ned på www.statkart.no/Documents/CSV-filer/Fylke_og_kommuneoversikt.csv.


    // pointer such that it can be passed around. Should be C++11 shared pointer, but neglected for simplicity.
    locationTable = new LocationTable(Util::ExtractLocationsFromCVSFile("Fylke_og_kommuneoversikt.csv"));

    FBGraph_Parser* parser = new FBGraph_Parser(locationTable);
    NetworkManager* net = new NetworkManager(this, parser);
    parser->setNetworkManager(net);

    QString oculuskeywords[] = {"oculus rift","oculus", "lol", 0};
    parser->addProduct("Oculus Rift", Product::WearableElectornics, oculuskeywords);

    net->setToken("CAACEdEose0cBAG3ARMwgbfotDYJoYiCUIS05vspTutmasHpeotRlZBRKhDYF36BEmJsW8aWGrdH57nGgbu3ibZBMSL4MDlx2KDcycjsKIHoUAlBeXXLVqsp46mkMkfg1zkMYxqKtyrgrmhOOO1drXtmrVVXxv89SDlbjfyp13i0ZC84B1LjeEw2R0swGaD4SRiZBl0GMLgZDZD");
    net->addGetGraphJob("&fields=posts", "expertnorge");

    //net->addGetFacebookAboutPersonPage("birgitte.haavardsholm");

    //net->addGetJob("www.statkart.no/Documents/CSV-filer/Fylke_og_kommuneoversikt.csv");



    
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
    delete locationTable;
}
