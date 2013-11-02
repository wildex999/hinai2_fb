#ifndef COMMENT_H
#define COMMENT_H

#include "post.h"

class Person;

class Comment : public Post
{
public:
    Comment(QString id, QString msg, QString date, Post* parentpost);

    void setPoster(Person* poster);
    Person* getPoster();

protected:
    Post* parent;
    Person* poster;
};

#endif // COMMENT_H
