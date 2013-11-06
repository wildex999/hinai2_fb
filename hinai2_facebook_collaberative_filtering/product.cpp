#include "product.h"

Product::Product(PRODUCT product, QString name, ProductType type)
{
    this->product = product;
    productName = name;
    this->type = type;
    item = NULL;

    postsCount = 0;
    likesCount = 0;
    commentsCount = 0;
}

QList<QString>* Product::getKeywords()
{
    return &keywords;
}

Product::ProductType Product::getType()
{
    return type;
}

QString& Product::getProductName()
{
    return productName;
}

PRODUCT Product::getProductId()
{
    return product;
}
