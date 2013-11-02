#ifndef POST_H
#define POST_H

#include <QString>
#include <QDate>
#include <QHash>

class Person;
class Comment;

class Post
{
public:
    Post(QString id, QString msg, QString date, QString shop);

    //Get the contents of the post
    QString& getContents();

    //Get the posting date
    QDate& getDate();

    //Get a list of people who liked this post
    QList<Person*>& getLiked();

    //Get the shop this post was gathered from
    QString& getShop();

    QString getId();


    void setMessage(QString msg);
    void setId(QString id);

    void addComment(Comment* comment);

    bool relevant; //True if the post contaians or is linked to a keyword

protected:
    QString message;
    QString shop;
    QHash<QString, Comment*> comments;
    QString id;
    QString date;

};

#endif // POST_H
