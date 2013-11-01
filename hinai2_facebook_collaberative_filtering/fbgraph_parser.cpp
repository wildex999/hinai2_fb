#include "fbgraph_parser.h"

#include <QFile>
#include <QMessageBox>
#include <QVariantMap>
#include <QVariant>
#include <QDebug>

#include "networkmanager.h"
#include "post.h"
#include "product.h"
#include "comment.h"

FBGraph_Parser::FBGraph_Parser()
{
    networkmanager = NULL;
    pages = 0;
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

    pages++;
    qDebug() << "Parsing page " << pages;

    //Go through and get Posts, Comments and likes
    QVariantMap posts = data["posts"].toMap();
    if(posts.isEmpty())
        posts = data;
    int i= 0;

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
        QVariantMap post = postraw.toMap();

        QString message = post["message"].toString();
        QString id = post["id"].toString();
        QString date = post["created_time"].toString();

        //Add it if it doesn't exist
        QHash<QString, Post*>::Iterator postit = this->posts.find(id);
        if(postit == this->posts.end())
        {
            Post* post = new Post(id, message, date);
            this->posts.insert(id, post);

            //Search message for keywords
            QList<Product*> foundproducts = findKeywords(message);

            //Get all likes if we found a product
            if(foundproducts.size() != 0)
            {

            }


            //Get all comments
        }
        else
            continue;
    }
}

bool FBGraph_Parser::parseComments(Post* post, QList<QVariant> &comments)
{
    foreach(QVariant commentraw, comments)
    {
        QVariantMap comment = commentraw.toMap();

        QString message = comment["message"].toString();
        QString id = comment["id"].toString();
        QString date = comment["created_time"].toString();

        //No need to check if it exists, we have already done that check on the post
        Comment* newcomment = new Comment(id, message, date);

        //Add comment to the post
        post->addComment(newcomment);

        //Search message for keywords
        QList<Product*> foundproducts = findKeywords(message);

        //Get all likes if we found a product
        if(foundproducts.size() != 0)
        {

        }
    }
}

bool FBGraph_Parser::parsePerson(QByteArray &rawData)
{
  return false;
}

void FBGraph_Parser::addProduct(QString name, QString keywords[])
{
    Product* product = NULL;

    //Check if product exists
    QHash<QString, Product*>::Iterator prodit = products.find(name);
    if(prodit == products.end())
    {
        product = new Product(name);
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
}
