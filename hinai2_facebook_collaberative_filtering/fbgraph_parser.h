#ifndef FBGRAPH_PARSER_H
#define FBGRAPH_PARSER_H

#include <QString>
#include <QList>
#include <QHash>
#include <QVariantMap>
#include <qjson/parser.h>
#include "locationTable.h"
#include "product.h"

class Person;
class Post;
class NetworkManager;

class FBGraph_Parser
{
public:
    FBGraph_Parser(LocationTable* locationTable);
    ~FBGraph_Parser();

    //Set the network manager to use when requesting further info
    void setNetworkManager(NetworkManager* nm);

    bool parseFile(std::string file); //Parse a file
    bool parse(QByteArray& rawdata); //Parse raw data
    bool parsePerson3(QByteArray& rawData);
    Person* parsePerson1(QVariantMap& from); //Parse person from main graph list(Comments/Likes)
    bool parsePerson2(QVariantMap& data); //Parse persons FB Graph
    bool parseLocations(const QByteArray& rawdata);

    //Parse posts, comments and their likes
    bool parsePosts(QList<QVariant>& posts);
    bool parseComments(Post* post, QVariantMap& comments);
    bool parseLikes(QList<QVariant>& people);

    //Add product
    Product* addProduct(QString name, Product::ProductType type, QString keywords[]);

    //Add person and parse further data
    Person* addPerson(QString id, QString name);

    //Get a hash map of People
    QHash<QString, Person*>& getPeople();

    //Get a hash map of Products
    QHash<QString, Product*>& getProducts();

    QString currentShop;

protected:
    //Find keywords from all products in message
    QList<Product*> findKeywords(QString message);

    //Find keywords from product in message
    bool findProductKeywords(Product* product, QString message);

    QHash<QString, Person*> people;
    QHash<QString, Product*> products;
    LocationTable* locationTable;

    QHash<QString, Post*> posts;

    QJson::Parser parser;
    NetworkManager* networkmanager;

    int pages;

};

#endif // FBGRAPH_PARSER_H
