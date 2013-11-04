#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>


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

    net->setToken("CAACEdEose0cBACZCQZCptyg0IjILFEltqBjsHoSg5kjr5wk06KxlIQWwd4cL00e9zDWILDequUSFVDO4ScjyxBsQjahV6tALZBRlAZB9fzksTjQ0n7mRZALXDZCmMjT8kDfciZBDDRSNMBpuVeNGbWOWBCTJUXWEZCp3WpTOyqT6tdipKM2nwwi87clNrZAEYr1vhcryKb0WYggZDZD");
    net->addGetGraphJob("&fields=posts", "expertnorge");

    //net->addGetFacebookAboutPersonPage("birgitte.haavardsholm");

    //net->addGetJob("www.statkart.no/Documents/CSV-filer/Fylke_og_kommuneoversikt.csv");

//    cf.addToTable(male,ipad_air);
//    cf.addToTable(male,assasins_creed_4);
//    cf.addToTable(male,ipad_mini);
//    cf.addToTable(male,ipad_mini);
//    cf.addToTable(male,ipad_mini);
//    cf.addToTable(age30_40,microsoft_surface_rt_64_gb);

    cf.generateRandomData();

    cf.makeCalculations();

    cf.writeToDebug();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete locationTable;
}

void MainWindow::convertPersonToGroup(Person person, QList< GROUP>& groups)
{
//    if(compare(person.getGender(),'male') == 0)
//        groups.append(male);
//    else if(std::compare(person.getGender(),'female') == 0)
//        groups.append(female);
//    //    else
//    //        groups.append();

//    if(person.getAge() < 20)
//        groups.append(age14_20);
//    else if(person.getAge() < 30)
//        groups.append(age20_30);
//    else if(person.getAge() < 40)
//        groups.append(age30_40);
//    else if(person.getAge() < 50)
//        groups.append(age40_50);
//    else if(person.getAge() < 60)
//        groups.append(age50_60);
//    else
//        groups.append(age70_pluss);

//    if(std::compare(person.getPlace(),'ostlandet') == 0)
//        groups.append(ostlandet);
//    else if (std::compare(person.getPlace(),'sorlandet') == 0)
//        groups.append(sorlandet);
//    else if (std::compare(person.getPlace(),'trondelage') == 0)
//        groups.append(trondelag);
//    else if (std::compare(person.getPlace(),'vestlandet') == 0)
//        groups.append(vestlandet);
//    else if (std::compare(person.getPlace(),'nord_norge') == 0)
//        groups.append(nord_norge);
//    else if (std::compare(person.getPlace(),'utlandet') == 0)
//        groups.append(utlandet);

}

void MainWindow::addPersonWithProducts(Person person)
{
//    QList<GROUP> groups;
//    QList<PRODUCT> products;
//    convertPersonToGroup(person, groups);
//    for(int i = 0; i < groups.count(); i++)
//        for(int j = 0; j < products.count(); j++)
//            cf.addToTable(groups[i],products[j]);

}
