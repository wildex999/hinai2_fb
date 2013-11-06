#include "post.h"
#include "comment.h"

Post::Post(QString id, QString msg, QString date, QString shop)
{
    message = msg;
    this->id = id;
    this->date = date;
    this->shop = shop;
    relevant = false;
}

void Post::setMessage(QString msg)
{
    message = msg;
}

void Post::setId(QString id)
{
    this->id = id;
}

QString& Post::getShop()
{
    return shop;
}

QString Post::getId()
{
    return id;
}

void Post::addComment(Comment *comment)
{
    //Don't check if we overwrite, we don't care
    comments[comment->getId()] = comment;
}

void Post::addLike(Like *like)
{
    likes.append(like);
}

QHash<PRODUCT, Product*>& Post::getProducts()
{
    return products;
}
