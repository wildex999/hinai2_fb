#ifndef LIKE_H
#define LIKE_H

//A like links a person to the post they liked, including the date

class Person;
class Post;

class Like
{
public:
    Like(Post* post, Person* person);


protected:
    Person* person;
    Post* post;
};

#endif // LIKE_H
