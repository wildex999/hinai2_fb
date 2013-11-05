#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

#include <QTableView>
#include "networkmanager.h"
#include "collaberativefiltering.h"
#include "fbgraph_parser.h"
#include <util.h>
#include <QDebug>
#include "locationTable.h"
#include "QStandardItemModel"
#include "fbgraph_parser.h"
#include "like.h"
#include "post.h"

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
    QString token;
    token = Util::ExtractTokenFromFile("token.txt");

    FBGraph_Parser* parser = new FBGraph_Parser(locationTable);

    NetworkManager* net = new NetworkManager(this, parser);
    parser->setNetworkManager(net);


    QString ipadminikeywords[] = {"ipad mini", "ipad-mini", "mini", 0};
    parser->addProduct(ipad_mini, "iPad Mini", Product::Pad, ipadminikeywords);

    QString ipadairkw[] = {"ipad air", "ipad-air", "air", 0};
    parser->addProduct(ipad_air, "iPad Air", Product::Pad, ipadairkw);

    QString googlenexuskw[] = {"google nexus", "google-nexus", "nexus", 0};
    parser->addProduct(google_nexus, "Google Nexus", Product::MobilePhone, googlenexuskw);

    QString fifakw[] = {"fifa 14", "fifa14", "fifa-14", "fifa", 0};
    parser->addProduct(fifa_14, "FIFA 14", Product::Game, fifakw);

    QString asscreedkw[] = {"assassins' creed 4" , "assassins creed 4", "assassins creed", 0};
    parser->addProduct(assasins_creed_4, "Assassins' Creed 4", Product::Game, asscreedkw);

    QString micsurfkw[] = {"microsoft surface rt 64 gb", "microsoft surface rt 64gb", "surface rt 64 gb", "surface rt 64gb", "surface rt", 0};
    parser->addProduct(microsoft_surface_rt_64_gb, "Microsoft Surface RT 64 GB", Product::Pad, micsurfkw);

    QString samsmartkw[] = {"smart watch", "galaxy gear", "samsung gear", 0};
    parser->addProduct(samsung_smart_watch_galaxy_gear, "Samsung Smart Watch - Galaxy Gears", Product::WearableElectornics, samsmartkw);

    QString samgalkw[] = {"Galaxy S IV", "galaxy s 4", "s iv", "galaxy s4", "galaxy s", 0};
    parser->addProduct(samsung_galaxy_s_iv, "Samsung Galaxy S IV", Product::MobilePhone, samgalkw);


    net->setToken(token);

    net->addGetGraphJob("&fields=posts.fields(likes.limit(999),comments.limit(999),message)", "expertnorge");

    //net->addGetGraphJob("&fields=posts", "expertnorge");

    QHash<QString, Person*>& people = parser->getPeople();
    QHash<QString, Person*>::iterator i;
    for (i = people.begin(); i != people.end(); ++i)
        addPersonWithProducts(*i.value());



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


    QStandardItemModel *model = new QStandardItemModel(cf.getnrGroupsValue(),cf.getnrProductsValue(),this);



    for(int i=0; i<cf.getnrProductsValue(); i++)
        model->setHorizontalHeaderItem(i, new QStandardItem(QString(cf.getProductName(i))));

    for(int i=0; i<cf.getnrGroupsValue(); i++)
    model->setVerticalHeaderItem(i, new QStandardItem(QString(cf.getGroupName(i))));

    for(int i = 0; i < cf.getnrGroupsValue(); i++)
    {
        for(int j = 0; j < cf.getnrProductsValue(); j++)
        {
            QStandardItem *tableValue = new QStandardItem(QString::number(cf.getTableValue(i,j)));
            model->setItem(i,j,tableValue);
        }
    }

    ui->tableView->setModel(model);

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

void MainWindow::convertProductStringToProduct(Product product, PRODUCT &productenum)
{

}



void MainWindow::addPersonWithProducts(Person person)
{
    QList<GROUP> groups;
    QList<PRODUCT> productlist;
    convertPersonToGroup(person, groups);

    QHash<QString, Product*> products ;
    QList<Like*> likes = person.getLikes();
    foreach(Like* like , likes)
    {
       Post* post = like->getPost();
       if(post->relevant)
       {
           products= post->getProducts();
       }
    }

    QHash<QString, Product*>::iterator i;
//    for (i = products.begin(); i != products.end(); ++i)
//            productlist.push_back(i.value());

//    for(int i = 0; i < groups.count(); i++)
//        for(int j = 0; j < productlist.count(); j++)
//            cf.addToTable(groups[i],products[j]);

}

void MainWindow::on_setTolkenButton_clicked()
{

}
