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

    parser = new FBGraph_Parser(locationTable);

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
    net->addGetGraphJob("&fields=posts.fields(likes.limit(999),comments.limit(999),message)", "elkjop");

    //net->addGetGraphJob("&fields=posts", "expertnorge");

    //net->addGetFacebookAboutPersonPage("birgitte.haavardsholm");

    //net->addGetJob("www.statkart.no/Documents/CSV-filer/Fylke_og_kommuneoversikt.csv");

//    cf.addToTable(male,ipad_air);
//    cf.addToTable(male,assasins_creed_4);
//    cf.addToTable(male,ipad_mini);
//    cf.addToTable(male,ipad_mini);
//    cf.addToTable(male,ipad_mini);
//    cf.addToTable(age30_40,microsoft_surface_rt_64_gb);


//    cf.generateRandomData();

//    cf.makeCalculations();

//    cf.writeToDebug();




}

MainWindow::~MainWindow()
{
    delete ui;
    delete locationTable;
}

void MainWindow::convertPersonToGroup(Person person, QList< GROUP>& groups)
{
    if(person.getGender() == 0)
        groups.append(male);
    else if(person.getGender() == 1)
        groups.append(female);
    else
        groups.append(undefined);

    if(person.getAge() < 20)
        groups.append(age14_20);
    else if(person.getAge() < 30)
        groups.append(age20_30);
    else if(person.getAge() < 40)
        groups.append(age30_40);
    else if(person.getAge() < 50)
        groups.append(age40_50);
    else if(person.getAge() < 60)
        groups.append(age50_60);
    else
        groups.append(age70_pluss);

    if(person.getRegion().compare("Østfold") == 0)
        groups.append(ostfold);
    else if (person.getRegion().compare("Akershus") == 0)
        groups.append(akershus);
    else if (person.getRegion().compare("Oslo") == 0)
        groups.append(oslo);
    else if (person.getRegion().compare("Hedmark") == 0)
        groups.append(hedmark);
    else if (person.getRegion().compare("Oppland") == 0)
        groups.append(oppland);
    else if (person.getRegion().compare("Buskerud") == 0)
            groups.append(buskerud);
    else if (person.getRegion().compare("Vestfold") == 0)
        groups.append(vestfold);
    else if (person.getRegion().compare("Telemark") == 0)
        groups.append(telemark);
    else if (person.getRegion().compare("Aust-Agder") == 0)
        groups.append(aust_agder);
    else if (person.getRegion().compare("Vest-Agder") == 0)
        groups.append(vest_agder);
    else if (person.getRegion().compare("Rogaland") == 0)
            groups.append(rogaland);
    else if (person.getRegion().compare("Hordaland") == 0)
        groups.append(hordaland);
    else if (person.getRegion().compare("Sogn og Fjordane") == 0)
        groups.append(sogn_og_fjordane);
    else if (person.getRegion().compare("Møre og Romsdal") == 0)
            groups.append(more_og_romsdal);
    else if (person.getRegion().compare("Sør Trøndelag") == 0)
        groups.append(sor_trondelag);
    else if (person.getRegion().compare("Nord Trøndelag") == 0)
        groups.append(nord_trondelag);
    else if (person.getRegion().compare("Nordland") == 0)
        groups.append(nordland);
    else if (person.getRegion().compare("Troms") == 0)
        groups.append(troms);
    else if (person.getRegion().compare("Finmark") == 0)
            groups.append(finmark);

}

void MainWindow::convertProductStringToProduct(Product product, PRODUCT &productenum)
{

}



void MainWindow::addPersonWithProducts(Person person, bool train)
{
    QList<GROUP> groups;
    QList<PRODUCT> productlist;
    convertPersonToGroup(person, groups);

    QHash<PRODUCT, Product*> products ;
    QList<Like*> likes = person.getLikes();
    foreach(Like* like , likes)
    {
       Post* post = like->getPost();
       if(post->relevant)
       {
           products= post->getProducts();
       }
    }

    if(products.count() > 0)
        qDebug() << "Products exits";

    QHash<PRODUCT, Product*>::iterator i;
    for (i = products.begin(); i != products.end(); ++i)
            productlist.push_back(i.key());

    for(int i = 0; i < groups.count(); i++)
        for(int j = 0; j < productlist.count(); j++)
        {
            if(train)
                cf.addToTrainTable(groups[i],productlist[j]);
            else
                cf.addToTestTable(groups[i],productlist[j]);
        }

}

void MainWindow::on_setTolkenButton_clicked()
{

}

void MainWindow::on_UpdatePeople_clicked()
{
    QHash<QString, Person*>& people = parser->getPeople();
    int train = people.count()*0.7;
    int count = 0;
    QHash<QString, Person*>::iterator i;
    for (i = people.begin(); i != people.end(); ++i)
    {
        count++;
        if(count<train)
            addPersonWithProducts(*i.value(),true);
        else
            addPersonWithProducts(*i.value(),false);
    }

    qDebug() << "Finished adding to cf lists";

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
