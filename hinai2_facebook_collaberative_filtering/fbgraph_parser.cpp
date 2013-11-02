#include "fbgraph_parser.h"

#include <QFile>
#include <QMessageBox>
#include <QVariantMap>
#include <QVariant>
#include <QDebug>

#include "networkmanager.h"
#include "post.h"
#include "comment.h"
#include "person.h"

FBGraph_Parser::FBGraph_Parser()
{
    networkmanager = NULL;
    pages = 0;
    currentShop = "Undefined";
}

FBGraph_Parser::~FBGraph_Parser()
{
    //Cleanup
    //TODO
}

void FBGraph_Parser::setNetworkManager(NetworkManager *nm)
{
    networkmanager = nm;
}

QList<Product*> FBGraph_Parser::findKeywords(QString message)
{
    QList<Product*> prodlist;

    QHash<QString, Product*>::Iterator prodit;
    for(prodit = products.begin(); prodit != products.end(); prodit++)
    {
        Product* product = *prodit;

        //Check if this product is mentioned in message
        if(findProductKeywords(product, message))
        {
            //Add product to list
            prodlist.append(product);
        }
    }
    return prodlist;
}

bool FBGraph_Parser::findProductKeywords(Product *product, QString message)
{
    QList< QString >* keywords = product->getKeywords();
    QList< QString >::Iterator keywordit;

    for(keywordit = keywords->begin(); keywordit != keywords->end(); keywordit++)
    {
        QString keyword = *keywordit;

        if(message.contains(keyword, Qt::CaseInsensitive))
        {
            qDebug() << "Found keyword " + keyword;
            return true;
        }

    }

    return false;
}

bool FBGraph_Parser::parseFile(std::string file)
{
    QFile qfile;
    qfile.setFileName(QString::fromStdString(file));
    if(!qfile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "Error while opening file for parsing!", qfile.errorString() );
        return false;
    }
    QByteArray rawdata = qfile.readAll();

    return parse(rawdata);
}

bool FBGraph_Parser::parse(QByteArray& rawdata)
{
    //Parse Json into QVariant
    bool ok;
    QVariantMap data = parser.parse(rawdata, &ok).toMap();
    if(!ok)
        return false;

    //Check if we are parsing a page or a person
    //Persons always use numbers as their path, so we do a check on that
    long long value = currentShop.toLongLong(&ok);
    if(ok) //Is a number, so we assume it's a person(TODO: Double check that)
    {
        return parsePerson2(data);
    }

    //Check if we are parsing like or comment pages
    //currentShop = postid/likes or postid/comments
    if(currentShop.contains("likes") || currentShop.contains("comments"))
    {
        QStringList strings = currentShop.split("/", QString::SkipEmptyParts);
        if(strings.size() != 2)
        {
            qDebug() << "Error parsing likes/comments pages!";
            return false;
        }

        QString postid = strings[0];
        QString pagetype = strings[1];

        //Get post from id
        Post* post = posts[postid];

        if(post == NULL)
        {
            qDebug() << "Post not found while paging!";
            return false;
        }

        if(pagetype.compare("likes") == 0)
        {
            //Parse likes
        }
        else if(pagetype.compare("comments") == 0)
        {
            //Parse comments
            parseComments(post, data);
            qDebug() << " Test";
        }

    }

    pages++;
    //qDebug() << "Parsing page " << pages << " | " << ok << " | " << currentShop;

    //Go through and get Posts, Comments and likes
    QVariantMap posts = data["posts"].toMap();
    if(posts.isEmpty())
        posts = data;

    //Parse posts, comments and collect likes in relevant posts
    QList<QVariant> postslist = posts["data"].toList();
    parsePosts(postslist);

    //Get next page
    if(!posts["paging"].isNull())
    {
        qDebug() << "Asking for next page";
        networkmanager->addGetJob(posts["paging"].toMap()["next"].toString());
    }
    else
        qDebug() << "No next page found";
}

bool FBGraph_Parser::parsePosts(QList<QVariant>& posts)
{
    foreach(QVariant postraw, posts)
    {
        QVariantMap postmap = postraw.toMap();

        QString message = postmap["message"].toString();
        QString id = postmap["id"].toString();
        QString date = postmap["created_time"].toString();

        //Add it if it doesn't exist
        QHash<QString, Post*>::Iterator postit = this->posts.find(id);
        if(postit == this->posts.end())
        {
            Post* post = new Post(id, message, date, currentShop);

            this->posts.insert(id, post);
            qDebug() << "Added post";

            //Search message for keywords
            QList<Product*> foundproducts = findKeywords(message);

            //If we found a keyword, add the post to the list, and parse likes
            if(foundproducts.size() != 0)
            {
                post->relevant = true; //Set relevant to true to indicate this post contains a product keyword

                //Get all likes

            }

            //Get all comments
            QVariantMap comments = postmap["comments"].toMap();
            parseComments(post, comments);
        }
        else
            continue;
    }
}

bool FBGraph_Parser::parseComments(Post* post, QVariantMap &comments)
{
    QList<QVariant> commentslist = comments["data"].toList();
    foreach(QVariant commentraw, commentslist)
    {
        QVariantMap comment = commentraw.toMap();

        QString message = comment["message"].toString();
        QString id = comment["id"].toString();
        QString date = comment["created_time"].toString();

        //No need to check if it exists, we have already done that check on the post
        Comment* newcomment = new Comment(id, message, date, post);

        //Add comment to the post
        post->addComment(newcomment);

        if(post->relevant)
            newcomment->relevant = true;

        //Search message for keywords
        QList<Product*> foundproducts = findKeywords(message);

        //Get all likes if we found a product keyword
        if(foundproducts.size() != 0)
        {
            //If a product keyword is mentioned inside the comment, it is relevant
            newcomment->relevant = true;
        }

        //Get the person
        QVariantMap from = comment["from"].toMap();
        Person* person = this->parsePerson1(from);
        //Add comment to person
        person->addComment(newcomment);
        //Add person to the comment
        newcomment->setPoster(person);
    }

    //Get next page of comments for this post
    if(!comments["paging"].isNull())
    {
        if(!comments["paging"].toMap()["next"].isNull())
        {
            networkmanager->addGetJob(comments["paging"].toMap()["next"].toString());
            qDebug() << "Get next comments page: " << comments["paging"].toMap()["next"].toString();
        }
    }
}

bool FBGraph_Parser::parsePerson3(QByteArray &rawData)
{
  QString data = rawData.data();
  qDebug() << data;

  return false;
}

Person* FBGraph_Parser::parsePerson1(QVariantMap &from)
{
    //Get the Id and name
    QString id = from["id"].toString();
    QString name = from["name"].toString();

    //Add the person
    Person* person = addPerson(id, name);

    return person;
}

bool FBGraph_Parser::parsePerson2(QVariantMap& data)
{
    if(data.isEmpty())
        return false;

    QString id = data["id"].toString();
    QString username = data["username"].toString();

    Person::Gender gender = Person::Unknown;
    QVariant genderVar = data["gender"];
    if(!genderVar.isNull()) //Check if we actually got a gender
    {
        QString genderStr = genderVar.toString().toLower();
        if(genderStr.compare("male") == 0)
            gender = Person::Male;
        else if(genderStr.compare("female") == 0)
            gender = Person::Female;
    }

    //Get person(We assume that parsePerson1 created the person if it didn't exist)
    Person* person = people[id];
    person->setUsername(username);
    person->setGender(gender);
    //TODO: More?

    qDebug() << "Parse person 2: " << username;
}

Product* FBGraph_Parser::addProduct(QString name, Product::ProductType type, QString keywords[])
{
    Product* product = NULL;

    //Check if product exists
    QHash<QString, Product*>::Iterator prodit = products.find(name);
    if(prodit == products.end())
    {
        product = new Product(name, type);
        products[name] = product;
    }
    else
        product = *prodit;

    QList<QString>* keywordlist = product->getKeywords();
    while(*keywords != NULL)
    {
        QString keyword = *keywords++;
        keywordlist->append(keyword);
    }

    return product;
}

Person* FBGraph_Parser::addPerson(QString id, QString name)
{
    //Check if the person already exists and add if not
    Person* person = NULL;
    QHash<QString, Person*>::Iterator personiter = people.find(id);
    if(personiter == people.end())
    {
        //Create and add the person
        person = new Person(id, name);
        people[id] = person;

        qDebug() << "Added person: " << name;

        //Send GET request for more info from FB Graph API
        networkmanager->addGetFacebookGraphPerson(id);
    }
    else
        person = *personiter;

    return person;
}
