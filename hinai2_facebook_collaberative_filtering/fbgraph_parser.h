#ifndef FBGRAPH_PARSER_H
#define FBGRAPH_PARSER_H

#include <QString>
#include <QList>
#include <QHash>
#include <qjson/parser.h>

class Product;
class Person;
class Post;
class NetworkManager;

class FBGraph_Parser
{
public:
    FBGraph_Parser();

    //Set the network manager to use when requesting further info
    void setNetworkManager(NetworkManager* nm);

    bool parseFile(std::string file); //Parse a file
    bool parse(QByteArray& rawdata); //Parse raw data

    //Add product
    void addProduct(QString name, QString keywords[]);

    //Get a hash map of People
    QHash<QString, Person*>& getPeople();

    //Get a hash map of Products
    QHash<QString, Product*>& getProducts();

protected:
    //Find keywords from all products in message
    QList<Product*> findKeywords(QString message);

    //Find keywords from product in message
    bool findProductKeywords(Product* product, QString message);

    QHash<QString, Person*> people;
    QHash<QString, Product*> products;

    QHash<QString, Post*> posts;

    QJson::Parser parser;
    NetworkManager* networkmanager;

};

#endif // FBGRAPH_PARSER_H
