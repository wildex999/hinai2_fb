#include "product.h"

Product::Product(QString name, ProductType type)
{
    productName = name;
    this->type = type;
}

QList<QString>* Product::getKeywords()
{
    return &keywords;
}

Product::ProductType Product::getType()
{
    return type;
}
