#ifndef POST_H
#define POST_H

#include <QString>
#include <QDate>

class Post
{
public:
    Post();

    //Get the contents of the post
    QString& getContents();

    //Get the posting date
    QDate& getDate();

};

#endif // POST_H
