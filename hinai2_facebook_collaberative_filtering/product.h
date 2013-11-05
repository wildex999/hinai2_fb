#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QHash>

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

    Product(QString name, ProductType type);

    QString& getProductName();
    QList<QString>* getKeywords();
    ProductType getType();

    QString productName;

protected:
    QHash<QString, Person*> people;
    QList<QString> keywords; //Keywords that can be used in place of the full name when searching
    ProductType type; //Product type
};

#endif // PRODUCT_H
