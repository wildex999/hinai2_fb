#include "networkmanager.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QByteArray>

#include "fbgraph_parser.h"

NetworkManager::NetworkManager(QMainWindow* window, FBGraph_Parser* parser)
{
    nam = new QNetworkAccessManager(window);
    this->parser = parser;
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(onReply(QNetworkReply*)));
    recCount = 0;

    initiateFacebookCookies();
}

NetworkManager::~NetworkManager()
{
    delete nam;
}

void NetworkManager::addGetGraphJob(QString query, QString store)
{
    QString requestUrl = "https://graph.facebook.com/" + store + "?access_token=";
    nam->get(QNetworkRequest(QUrl(requestUrl + token + query)));
    recCount++;
}

void NetworkManager::addGetPageJob(QString url)
{
  nam->get(QNetworkRequest(QUrl(url)));
  recCount++;
}

void NetworkManager::addGetFacebookGraphPerson(QString id)
{
    QString requestUrl = "https://graph.facebook.com/" + id + "?access_token=";
    nam->get(QNetworkRequest(QUrl(requestUrl + token)));
    recCount++;
}

void NetworkManager::addGetFacebookAboutPersonPage(QString fbUsername)
{
  QString url = "https://www.facebook.com/" + fbUsername + "/about";
  addGetPageJob(url);
}

void NetworkManager::addGetJob(QString url)
{
    url = url.replace("%28", "(");
    url = url.replace("%29", ")");
    nam->get(QNetworkRequest(QUrl(url)));
    recCount++;
}

void NetworkManager::setToken(QString token)
{
    this->token = token;
}

// TEMP: manually insert a cookie from a logged-in state using a browser.
// Current State: Does not work.
void NetworkManager::initiateFacebookCookies()
{
  QByteArray cnames;
  cnames.append("c_user");
//  cnames.append("csm");
//  cnames.append("datr");
//  cnames.append("fr");
//  cnames.append("locale");
//  cnames.append("lu");
//  cnames.append("s");
//  cnames.append("xs");

  QByteArray cvalues;
  cvalues.append("100007036170126");
//  cvalues.append("2");
//  cvalues.append("AXNyUr2eqROMprDWQeIBuOWU");
//  cvalues.append("0yoO8B9HT1CNErUfU.AWVvxJb2dUTuC8NggrNJmej671s.BScnP8.jf.AAA.AWV7GMa3");
//  cvalues.append("en_US");
//  cvalues.append("Rhexz0zFw5MCOQ81LEfQNUrw");
//  cvalues.append("a7UVT2yaq6vVO4Z.BSdSN6");
//  cvalues.append("176%3A5crVRz_opwTXaw%3A2%3A1383408506%3A10463");


  QNetworkCookie cookie(cnames, cvalues);
  QList<QNetworkCookie> cookies;
  cookies.append(cookie);

  // Add cookie to existing cookie jar already present in the NAM.
  QNetworkCookieJar* jar =  nam->cookieJar();
  if(!jar->setCookiesFromUrl(cookies, QUrl("https://www.facebook.com")))
    qDebug() << "Couldn't add cookies for some reason.";
  else
    qDebug() << "Init: Added FB cookies.";
}

void NetworkManager::onReply(QNetworkReply *reply)
{
    recCount--;
    if(reply->error() == QNetworkReply::NoError)
    {
        //Send raw data to parser
        QByteArray rawdata = reply->readAll();
        QUrl url =  reply->url();

        if(url.encodedHost().contains("graph.facebook.com"))
        {
            QString path = url.path().remove(0,1);//Remove the '/'

            //Convert from shop id to number(Id used when paging)
            QStringList shop = path.split("/");
            if(shop[0].compare("169107001669") == 0)
                path = "expertnorge";
            else if(shop[0].compare("152082264813992") == 0)
                path = "elkjop";
            parser->currentShop = path;
            parser->parse(rawdata);
        }
        else if(url.encodedHost().contains("www.facebook.com"))
          parser->parsePerson3(rawdata);
        else if(url.encodedHost().contains("no.wikipedia.org"))
        {
          parser->parseLocations(rawdata);
        }
        else
          QMessageBox::warning(0, "Unidentified Foreign ObjectHost", "Got an url that was not expected and wouldn't get handled.", reply->errorString());
    }
    else
    {
        qDebug() << "Error: " << reply->errorString();
        qDebug() << "Body: " << reply->readAll();
        QMessageBox::warning(0, "Error getting network data!", reply->errorString());
    }

    //Check if this was the last request
    if(recCount < 0)
        qDebug() << "ERROR: reCount < 0?????????";
    if(recCount == 0)
        parser->markNetComplete();
}
