#ifndef COMMENT_H
#define COMMENT_H

#include "post.h"

class Person;

class Comment : public Post
{
public:
    Comment();

    Person* getPoster();
};

#endif // COMMENT_H
