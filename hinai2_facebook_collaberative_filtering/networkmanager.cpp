#include "networkmanager.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>

#include "fbgraph_parser.h"

NetworkManager::NetworkManager(QMainWindow* window, FBGraph_Parser* parser)
{
    nam = new QNetworkAccessManager(window);
    this->parser = parser;
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(onReply(QNetworkReply*)));
}

NetworkManager::~NetworkManager()
{
    delete nam;
}

void NetworkManager::addGetGraphJob(QString query, QString store)
{
    QString requestUrl = "https://graph.facebook.com/" + store + "?access_token=";
    nam->get(QNetworkRequest(QUrl(requestUrl + token + query)));
}

void NetworkManager::addGetPageJob(QString url)
{
  nam->get(QNetworkRequest(QUrl(url)));
}

void NetworkManager::addGetFacebookGraphPerson(QString id)
{
    QString requestUrl = "https://graph.facebook.com/" + id + "?access_token=";
    nam->get(QNetworkRequest(QUrl(requestUrl + token)));
}

void NetworkManager::addGetFacebookAboutPersonPage(QString fbUsername)
{
  QString url = "https://www.facebook.com/" + fbUsername + "/about";
  addGetPageJob(url);
}

void NetworkManager::addGetJob(QString url)
{
    nam->get(QNetworkRequest(QUrl(url)));
}

void NetworkManager::setToken(QString token)
{
    this->token = token;
}

void NetworkManager::onReply(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        //QMessageBox::warning(0, "Test, got data!", reply->readAll());
        //Send raw data to parser
        QByteArray rawdata = reply->readAll();
        QUrl url =  reply->url();

        if(url.encodedHost().contains("graph.facebook.com"))
        {
            QString path = url.path().remove(0,1);//Remove the '/'
            //Check if this is a shop or person reply. Persons are always numbers, so check if we can convert it
            parser->currentShop = path;
            parser->parse(rawdata);
        }
        else if(url.encodedHost().contains("www.facebook.com"))
          parser->parsePerson(rawdata);
        else
          QMessageBox::warning(0, "Unidentified Foreign ObjectHost", "Got an url that was not expected and wouldn't get handled.", reply->errorString());




    }
    else
    {
        QMessageBox::warning(0, "Error getting network data!", reply->errorString());
    }
}
