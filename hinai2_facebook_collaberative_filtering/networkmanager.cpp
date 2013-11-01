#include "networkmanager.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>

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
        parser->parse(rawdata);
    }
    else
    {
        QMessageBox::warning(0, "Error getting network data!", reply->errorString());
    }
}
