#include "comment.h"

Comment::Comment(QString id, QString msg, QString date, Post* parentpost)
    : Post(id, msg, date, parentpost->getShop())
{
    parent = parentpost;
}

Person* Comment::getPoster()
{
    return poster;
}

void Comment::setPoster(Person *poster)
{
    this->poster = poster;
}

