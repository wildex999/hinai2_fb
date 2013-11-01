#include "post.h"
#include "comment.h"

Post::Post(QString id, QString msg, QString date)
{
    message = msg;
    this->id = id;
    this->date = date;
}

void Post::setMessage(QString msg)
{
    message = msg;
}

void Post::setId(QString id)
{
    this->id = id;
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
