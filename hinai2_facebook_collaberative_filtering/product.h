#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QHash>

class Person;

class Product
{
public:
    Product(QString name);

    QList<QString>* getKeywords();

    QString productName;

protected:
    QHash<QString, Person*> people;
    QList<QString> keywords; //Keywords that can be used in place of the full name when searching
};

#endif // PRODUCT_H
