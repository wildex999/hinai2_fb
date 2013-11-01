#include "product.h"

Product::Product(QString name)
{
    productName = name;
}

QList<QString>* Product::getKeywords()
{
    return &keywords;
}
