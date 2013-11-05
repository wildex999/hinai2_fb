#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QMainWindow>

class FBGraph_Parser;

//The network manager has the task of connecting to Facebook Graph API and get data for the parser

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager(QMainWindow* window, FBGraph_Parser* parser);
    ~NetworkManager();

    //Add a request job for a store
    void addGetGraphJob(QString query, QString store);

    // Add a request to download a specified webpage.
    void addGetPageJob(QString url);
    void addGetFacebookGraphPerson(QString id);
    void addGetFacebookAboutPersonPage(QString fbUsername);

    //Do a normal http get request(Used for pagination and pulling people)
    void addGetJob(QString url);

    //Set the token used in graph request
    void setToken(QString token);

private slots:
    void onReply(QNetworkReply* reply);

protected:
    QNetworkAccessManager *nam;
    QString token;
    FBGraph_Parser* parser;
    int recCount;

private:
    void initiateFacebookCookies();

};

#endif // NETWORKMANAGER_H
