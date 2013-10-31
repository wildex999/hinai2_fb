#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QHash>

class Product;

class Person
{
public:
    Person();

    //Add product as one this person has shown interest in
    void addProductInterest(QString productName);

    //Get products this person has shown an interest in
    QHash<QString, Product*>& getProductInterests();

protected:
    QHash<QString, Product*> products;
};

#endif // PERSON_H
