#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QHash>
#include <QDateTime>

class Product;
class Comment;
class Like;

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

    void addLike(Like* like);

    //Add a like to the list done by this person
    //void addLike(Like* like);

    //Get products this person has shown an interest in
    QHash<QString, Product*>& getProductInterests();

    void setUsername(QString username);
    void setGender(Gender gender);
    void setRegion(const QString& region) { region_ = region; }
    void setArea(const QString& area) { area_ = area; }
    void setAge(ushort age) { age_ = age; }

    QString& getId();
    QString& getName();
    QString& getUsername();
    Gender getGender();
    QString getRegion() const { return region_; }
    QString getArea() const { return area_; }
    ushort getAge() const { return age_; }

protected:
    QString id;
    QString name;
    QString username;
    Gender gender;
    ushort age_;
    QString region_; // Fylke
    QString area_; // Kommune

    QHash<QString, Product*> products;
    QHash<QString, Comment*> comments;
    QList<Like*> likes;

    int                     age;
    QString                 place;

    void                    findAge(QDateTime birthdate);
    void                    findPlace(QString localPlace);
};

#endif // PERSON_H
