#ifndef POST_H
#define POST_H

#include <QString>
#include <QDate>

class Person;

class Post
{
public:
    Post();

    //Get the contents of the post
    QString& getContents();

    //Get the posting date
    QDate& getDate();

    //Get a list of people who liked this post
    QList<Person*>& getLiked();

    void setMessage(QString msg);

protected:
    QString message;

};

#endif // POST_H
