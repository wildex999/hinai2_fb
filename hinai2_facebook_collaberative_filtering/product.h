#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QHash>
#include <QStandardItem>
#include "collaberativefiltering.h"

class Person;

class Product
{
public:
    enum ProductType {
        Pad,
        MobilePhone,
        PortableComputer,
        Game,
        WearableElectornics,
        TV,
        Other
    };

    Product(PRODUCT product, QString name, ProductType type);

    void setItem(QStandardItem* item) { this->item = item; }

    QString& getProductName();
    PRODUCT getProductId();
    QList<QString>* getKeywords();
    ProductType getType();
    QStandardItem* getItem() { return item; }

    //Used for Status output
    long postsCount;
    long commentsCount;
    long likesCount;

protected:
    QHash<QString, Person*> people;
    QList<QString> keywords; //Keywords that can be used in place of the full name when searching
    ProductType type; //Product type
    QString productName;
    PRODUCT product;
    QStandardItem* item; //Link to the tree item
};

#endif // PRODUCT_H
