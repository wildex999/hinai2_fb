#ifndef FBGRAPH_PARSER_H
#define FBGRAPH_PARSER_H

#include <QString>
#include <QList>
#include <QHash>
#include <qjson/parser.h>

class Product;
class Person;

class FBGraph_Parser
{
public:
    FBGraph_Parser();

    //Add a keyboard to watch when parsing(Pair, <Search word, stored name>)
    void addWatchKeyword(std::pair<QString, QString>& keyword);

    bool parseFile(std::string file); //Parse a file
    bool parse(QByteArray* data); //Parse raw data

    //Get a hash map of People
    QHash<QString, Person*>& getPeople();

    //Get a hash map of Products
    QHash<QString, Product*>& getProducts();

protected:
    QList< std::pair<QString, QString> > keywords; //Keywords the parser watch for
    QHash<QString, Person*> people;
    QHash<QString, Product*> products;

};

#endif // FBGRAPH_PARSER_H
