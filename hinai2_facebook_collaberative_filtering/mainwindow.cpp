#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

#include <QTableView>
#include <QFile>
#include "networkmanager.h"
#include "collaberativefiltering.h"
#include "fbgraph_parser.h"
#include <util.h>
#include <QDebug>
#include "locationTable.h"
#include "QStandardItemModel"
#include "fbgraph_parser.h"
#include "comment.h"
#include "like.h"
#include "post.h"

// temp
LocationTable* locationTable;

int tabIndex = 2;
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
    model = new QStandardItemModel(cf.getnrGroupsValue(),cf.getnrProductsValue(),this);

    net = new NetworkManager(this, parser);
    parser->setNetworkManager(net);


    //QString token = "CAACEdEose0cBAMVHhHc9rTYfXVXVFgNLHHAln3ZCTpZAztU7NEEcONDEXIRpmU3fi6jS7AoD9LM3nZCTxnQHY1ZBLvsQ4kYbxzpyNk70a8xQRlLzdppZB7ZBlryJ6IA5j4yrbNaZABzCYQVRhZB2Xn3Q2I7FfDHs3qeJebfCbscjNJy270TeTcvqM7s82exZCU3MZD";
    //token = Util::ExtractTokenFromFile("token.txt");


    //QString oculuskeywords[] = {"oculus rift","oculus", "lol", 0};
    //parser->addProduct("Oculus Rift", Product::WearableElectornics, oculuskeywords);

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

//    net->addGetGraphJob("&fields=posts.fields(likes.limit(999),comments.limit(999),message)", "expertnorge");
//    net->addGetGraphJob("&fields=posts.fields(likes.limit(999),comments.limit(999),message)", "elkjop");

    //Listen to changes in people
    connect(parser, SIGNAL(newPersonAdded(Person*)), this, SLOT(onPeopleAdded(Person*)) );
    connect(parser, SIGNAL(relevantPersonUpdate(Person*)), this, SLOT(onPeopleUpdated(Person*)) );
    connect(parser, SIGNAL(newPostAdded(Post*)), this, SLOT(onPostAdded(Post*)) );
    connect(parser, SIGNAL(newCommentAdded(Comment*)), this, SLOT(onCommentAdded(Comment*)) );
    connect(parser, SIGNAL(newLikeAdded(Like*)), this, SLOT(onLikeAdded(Like*)) );
    connect(parser, SIGNAL(newPageParsing()), this, SLOT(onNewPageParsing()) );
    connect(parser, SIGNAL(doneParsing()), this, SLOT(onDoneParsing()) );

    statusParsing = true;
    statusPeopleCount = 0;
    statusRelevantPeopleCount = 0;
    statusPagesCount = 0;
    statusPostsCount = 0;
    statusRelevantPostsCount = 0;
    statusCommentsCount = 0;
    statusRelevantCommentsCount = 0;
    statusLikesCount = 0;
    statusRelevantLikesCount = 0;
    updateStatusBar();

    setupProductsTree();

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

    QFile file("/home/aleksander/alldata.txt");

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&file);

    int counter = 0;

    while(!in.atEnd()) {
        QString line = in.readLine();
        QList<QString> fields = line.split(":");
        for(int i = 0; i < fields.count(); i++)
        {
            if(counter < 28)
                cf.setTestData(counter,i,fields[i].toInt());
        }
        counter++;
    }

    file.close();

//    QFile file("/home/aleksander/testdata.txt");
//    file.open(QIODevice::ReadOnly | QIODevice::Text);

//    QTextStream in(&file);

//    int counter = 0;

//    while(!in.atEnd()) {
//        QString line = in.readLine();
//        QList<QString> fields = line.split(":");
//        for(int i = 0; i < fields.count(); i++)
//        {
//            if(counter < 28)
//                cf.setTrainData(counter,i,fields[i].toInt());
//        }
//        counter++;
//    }

//    file.close();

//    QFile file2("/home/aleksander/traindata.txt");
//    file2.open(QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream in2(&file2);

//    int counter2 = 0;

//    while(!in2.atEnd()) {
//        QString line = in2.readLine();
//        QList<QString> fields = line.split(":");
//        for(int i = 0; i < fields.count(); i++)
//        {
//            if(counter2 < 28)
//                cf.setTestData(counter2,i,fields[i].toInt());
//        }
//        counter2++;
//    }

//    file2.close();

    cf.makeCalculations();
    cf.predictNewTable();

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
    ui->tableView->resizeColumnsToContents();

    makeTable(cf,this);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete locationTable;
}

void MainWindow::onPeopleAdded(Person* person)
{
    statusPeopleCount++;
    updateStatusBar();
}

void MainWindow::onPeopleUpdated(Person* person)
{
    statusRelevantPeopleCount++;
    updateStatusBar();
}

void MainWindow::onNewPageParsing()
{
    statusPagesCount++;
    updateStatusBar();
}

void MainWindow::onPostAdded(Post *post)
{
    statusPostsCount++;
    //Check if relevant and increment there too
    if(post->relevant)
        statusRelevantPostsCount++;

    //Add post to products
    QHash<PRODUCT, Product*> products = post->getProducts();
    foreach(Product* product, products)
    {
        product->postsCount++;
        product->getItem()->child(0)->setText("Posts(" + QString::number(product->postsCount) + ")");
    }
    updateStatusBar();
}

void MainWindow::onCommentAdded(Comment *comment)
{
    statusCommentsCount++;
    if(comment->relevant)
        statusRelevantCommentsCount++;

    //Add comments to products
    QHash<PRODUCT, Product*> products = comment->getProducts();
    foreach(Product* product, products)
    {
        product->commentsCount++;
        product->getItem()->child(1)->setText("Comments(" + QString::number(product->commentsCount) + ")");
    }

    updateStatusBar();
}

void MainWindow::onLikeAdded(Like *like)
{
    statusLikesCount++;
    if(like->getPost()->relevant)
        statusRelevantLikesCount++;

    //Add likes to products
    QHash<PRODUCT, Product*> products = like->getPost()->getProducts();
    foreach(Product* product, products)
    {
        product->likesCount++;
        product->getItem()->child(2)->setText("Likes(" + QString::number(product->likesCount) + ")");
    }

    updateStatusBar();
}

void MainWindow::onDoneParsing()
{
    statusParsing = false;
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    QString parseStatus;
    if(statusParsing)
        parseStatus = "Parsing...";
    else
        parseStatus = "Done parsing!";


    QString status = QString("Status: %1 | People: %2(%3 Relevant) | ").arg(parseStatus, QString::number(statusPeopleCount), QString::number(statusRelevantPeopleCount));
    status += QString("%1 pages, %2 posts(%3 Relevant) | ").arg(QString::number(statusPagesCount), QString::number(statusPostsCount), QString::number(statusRelevantPostsCount));
    status += QString("%1 comments(%2 Relevant) | ").arg(QString::number(statusCommentsCount), QString::number(statusRelevantCommentsCount));
    status += QString("%1 likes(%2 Relevant)").arg(QString::number(statusLikesCount), QString::number(statusRelevantLikesCount));
    ui->statusBar->showMessage(status);
}

void MainWindow::setupProductsTree()
{
    QStandardItemModel* model = new QStandardItemModel();

    //Create items
    QHash<PRODUCT, Product*> products = parser->getProducts();
    foreach(Product* product, products)
    {
        QStandardItem* item = new QStandardItem(product->getProductName());
        product->setItem(item);
        //Add sub-items
        item->appendRow(new QStandardItem("Posts(0)"));
        item->appendRow(new QStandardItem("Comments(0)"));
        item->appendRow(new QStandardItem("Likes(0)"));

        model->appendRow(item);
    }

    ui->treeViewProducts->setModel(model);
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



    if(person.getRegion().compare(QString::fromStdWString(L"Østfold")) == 0)
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
    else if (person.getRegion().compare(QString::fromStdWString(L"Møre og Romsdal")) == 0)
        groups.append(more_og_romsdal);
    else if (person.getRegion().compare(QString::fromStdWString(L"Sør Trøndelag")) == 0)
        groups.append(sor_trondelag);
    else if (person.getRegion().compare(QString::fromStdWString(L"Nord Trøndelag")) == 0)
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


void MainWindow::on_setTokenButton_clicked()
{
    QString token = ui->tokenLine->text();
    net->setToken(token);

    net->addGetGraphJob("&fields=posts.fields(likes.limit(999),comments.limit(999),message)", "expertnorge");
    net->addGetGraphJob("&fields=posts.fields(likes.limit(999),comments.limit(999),message)", "elkjop");

}
void MainWindow::on_predictButton_clicked()
{

}



void MainWindow::on_updateButton_clicked()
{
    newTab = new QWidget(ui->tabWidget);
    QTableView *newTable = new QTableView(newTab);

    ui->tabWidget->insertTab(tabIndex,newTab,"Predicted");

    makeTable(cf,this);

    newTable->setModel(model);
    newTable->resize(1200,600);
    newTable->resizeColumnsToContents();

    tabIndex++;

}

void MainWindow::makeTable(CollaberativeFiltering cf, QObject *parent)
{
    model = new QStandardItemModel(cf.getnrGroupsValue(),cf.getnrProductsValue(),parent);

    for(int i=0; i<cf.getnrProductsValue(); i++)
            model->setHorizontalHeaderItem(i, new QStandardItem(QString(cf.getProductName(i))));

        for(int i=0; i<cf.getnrGroupsValue(); i++)
        model->setVerticalHeaderItem(i, new QStandardItem(QString(cf.getGroupName(i))));

        for(int i = 0; i < cf.getnrGroupsValue(); i++)
        {
            for(int j = 0; j < cf.getnrProductsValue(); j++)
            {
                QStandardItem *tableValue = new QStandardItem(QString::number(cf.getPredictedVotes(i,j)));
                model->setItem(i,j,tableValue);
            }
        }

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

    QFile file("/home/aleksander/alldata.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

//    QFile file2("/home/aleksander/testdata.txt");
//    file2.open(QIODevice::WriteOnly | QIODevice::Text);
//    QTextStream out2(&file2);


    QList<QList<int> > testData = cf.getTestVotes();
    QList<QList<int> > trainData = cf.getTrainVotes();

    for(int i = 0; i < testData.count(); i++)
    {
        for(int j = 0; j < testData[i].count(); j++)
        {
            if(j < testData[i].count()-1)
                out << (testData[i][j] + trainData[i][j]) << ":";
            else
                out << (testData[i][j] + trainData[i][j]);
        }
        out << "\n";
    }

//    for(int i = 0; i < trainData.count(); i++)
//    {
//        for(int j = 0; j < trainData[i].count(); j++)
//        {
//            if(j < testData[i],count()-1)
//                out2 << testData[i][j] << ":";
//            else
//                out2 << testData[i][j];
//        }
//        out2 << "\n";
//    }

    file.close();
//    file2.close();

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

