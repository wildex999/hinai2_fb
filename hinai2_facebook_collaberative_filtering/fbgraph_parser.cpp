#include "fbgraph_parser.h"

#include <QFile>
#include <QMessageBox>
#include <QVariantMap>

FBGraph_Parser::FBGraph_Parser()
{
}


void FBGraph_Parser::addWatchKeyword(std::pair<QString, QString> &keyword)
{

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

    return parse(&rawdata);
}

bool FBGraph_Parser::parse(QByteArray *data)
{
    //Parse Json into QVariant
    bool ok;
    //QVariantMap data = parser.parse(rawdata, &ok).toMap();

    //Go through and get Posts, Comments and likes
}
