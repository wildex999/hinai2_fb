#ifndef COMMENT_H
#define COMMENT_H

#include "post.h"

class Person;

class Comment : public Post
{
public:
    Comment(QString id, QString msg, QString date);

    Person* getPoster();

protected:
};

#endif // COMMENT_H
