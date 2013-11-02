#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QHash>

class Product;
class Comment;

class Person
{
public:
    enum Gender
    {
        Male,
        Female,
        Unknown
    };

    Person(QString id, QString name);

    //Add product as one this person has shown interest in
    void addProductInterest(QString productName);

    //Add a comment to the list of comments this person has written
    void addComment(Comment* comment);

    //Add a like to the list done by this person
    //void addLike(Like* like);

    //Get products this person has shown an interest in
    QHash<QString, Product*>& getProductInterests();

    void setUsername(QString username);
    void setGender(Gender gender);

    QString& getId();
    QString& getName();
    QString& getUsername();
    Gender getGender();

protected:
    QString id;
    QString name;
    QString username;
    Gender gender;

    QHash<QString, Product*> products;
    QHash<QString, Comment*> comments;

};

#endif // PERSON_H
