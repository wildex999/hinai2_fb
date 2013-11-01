#include "person.h"

#include "comment.h"

Person::Person(QString id, QString name)
{
    this->id = id;
    this->name = name;
}

void Person::addComment(Comment *comment)
{
    //No need to check if comment exists, as that is done when adding the comment itself to the parser
    comments[comment->getId()] = comment;
}

void Person::setUsername(QString username)
{
    this->username = username;
}

void Person::setGender(Gender gender)
{
    this->gender = gender;
}



void Person::findAge(QDateTime birthdate)
{
    QDateTime today = QDateTime::currentDateTime();
    age = today.date().year() - birthdate.date().year();
    if( birthdate > today.addYears(-age)) age--;
}


void Person::findPlace(QString localPlace)
{
}

