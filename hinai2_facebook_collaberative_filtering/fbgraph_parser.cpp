#include "fbgraph_parser.h"

#include <QFile>
#include <QMessageBox>
#include <QVariantMap>
#include <QVariant>
#include <QDebug>
#include <unistd.h>

#include "networkmanager.h"
#include "post.h"
#include "like.h"
#include "comment.h"
#include "person.h"
#include "util.h"


FBGraph_Parser::FBGraph_Parser(LocationTable* locTable)
  : locationTable(locTable)
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

    QHash<PRODUCT, Product*>::Iterator prodit;
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

bool FBGraph_Parser::findAddKeywords(QString message, QHash<PRODUCT, Product *> &list)
{
    QList<Product*> foundproducts = findKeywords(message);
    if(foundproducts.size() <= 0)
        return false;

    QList<Product*>::Iterator prodit;
    for(prodit = foundproducts.begin(); prodit != foundproducts.end(); prodit++)
        list[(*prodit)->getProductId()] = *prodit;
    //qDebug() << "GOT keyword";
    return true;
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
            //qDebug() << "Found keyword " + keyword;
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
    emit newPageParsing();
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
        qDebug() << "Asking for next page: " + posts["paging"].toMap()["next"].toString();
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
            //qDebug() << "Added post";

            //Search message for keywords and add them to the post
            bool foundproducts = findAddKeywords(message, post->getProducts());


            //If we found a keyword, add the post to the list, and parse likes
            if(foundproducts)
            {
                post->relevant = true; //Set relevant to true to indicate this post contains a product keyword
                //qDebug() << "Is relevant post: " << id << " Shop: " << post->getShop();
            }

            //Get and add all likes
            QVariantMap likes = postmap["likes"].toMap();
            parseLikes(post, likes);

            //Get all comments
            QVariantMap comments = postmap["comments"].toMap();
            parseComments(post, comments);

            //Emit that we are done parsing this post and it's ready for use
            emit newPostAdded(post);
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

        //TODO: Check if comment already exists
        Comment* newcomment = new Comment(id, message, date, post);

        //Add comment to the post
        post->addComment(newcomment);

        //If the post is relevant, so is the comment
        if(post->relevant)
            newcomment->relevant = true;

        //Search message for keywords
        bool foundproducts = findAddKeywords(message, newcomment->getProducts());

        //Get all likes if we found a product keyword
        if(foundproducts)
        {
            //If a product keyword is mentioned inside the comment, it is relevant
            newcomment->relevant = true;
            //qDebug() << "Is relevant comment: " << id << " Shop: " << newcomment->getShop();
        }

        QVariantMap likes = comment["likes"].toMap();
        parseLikes(newcomment, likes);

        //Get the person
        QVariantMap from = comment["from"].toMap();
        Person* person = this->parsePerson1(from);

        if(newcomment->relevant)
            getPersonExtended(person);

        person->addComment(newcomment);
        newcomment->setPoster(person);

        //Emit that we are done parsing this comment
        emit newCommentAdded(newcomment);
    }
}

bool FBGraph_Parser::parseLikes(Post* post, QVariantMap &data)
{
    QList<QVariant> likeslist = data["data"].toList();
    foreach(QVariant likeraw, likeslist)
    {
        QVariantMap map = likeraw.toMap();
        QString id = map["id"].toString();
        QString name = map["name"].toString();

        //Get the person using the id
        Person* person = addPerson(id, name);

        //If the post is relevant, get extra info from person, like gender
        if(post->relevant)
            getPersonExtended(person);

        //TODO: Check if like already exists
        Like* newlike = new Like(post, person);

        post->addLike(newlike);
        person->addLike(newlike);

        //qDebug() << "Got like from: " << person->getName();

        //Emit that we are done parsing this like
        emit newLikeAdded(newlike);
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

    // Adding some random location/age data so that other parts of the project can continue.
    QString region = locationTable->getRandomRegion();
    QString area = locationTable->getRandomAreaByRegion(region);
    ushort age = static_cast<ushort>(Util::randomNumber(15, 80));
    person->setRegion(region);
    person->setArea(area);
    person->setAge(age);

    //Emit that we are fully done parsing this person
    emit relevantPersonUpdate(person);

    //qDebug() << "Parse person 2:" << username << "Addr:" << person->getRegion() << person->getArea() << "Age:" << person->getAge();
}

Product* FBGraph_Parser::addProduct(PRODUCT product, QString name, Product::ProductType type, QString keywords[])
{
    Product* newproduct = NULL;

    //Check if product exists
    QHash<PRODUCT, Product*>::Iterator prodit = products.find(product);
    if(prodit == products.end())
    {
        newproduct = new Product(product, name, type);
        products[product] = newproduct;
    }
    else
        newproduct = *prodit;

    QList<QString>* keywordlist = newproduct->getKeywords();
    while(*keywords != NULL)
    {
        QString keyword = *keywords++;
        keywordlist->append(keyword);
    }

    return newproduct;
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

        //Emit that we have got a new person
        emit newPersonAdded(person);

        //qDebug() << "Added person: " << name;
    }
    else
        person = *personiter;

    return person;
}

void FBGraph_Parser::getPersonExtended(Person *person)
{
    if(person == NULL)
    {
        qDebug() << "ERROR: Got null person";
        return;
    }

    if(person->gotExtendedInfo)
    {
        //qDebug() << "Skipping existing";
        return; //No need to do it twice
    }

    person->gotExtendedInfo = true; //Mark that we don't need to do this again

    //Send GET request for more info from FB Graph API
    networkmanager->addGetFacebookGraphPerson(person->getId());
}

QHash<QString, Person *> &FBGraph_Parser::getPeople()
{
    return people;
}

QHash<PRODUCT, Product *> &FBGraph_Parser::getProducts()
{
    return products;
}

bool FBGraph_Parser::parseLocations(const QByteArray &rawdata)
{
  const QString html = rawdata.constData();



  return true;
}

void FBGraph_Parser::markNetComplete()
{
    qDebug() << "DONE PARSING!";
    emit doneParsing();
}
