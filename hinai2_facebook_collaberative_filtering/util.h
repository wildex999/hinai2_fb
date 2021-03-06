#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QMap>
#include <string>

class Util
{
public:
  static QMap<QString, QList<QString> > ExtractLocationsFromCVSFile(const QString& filename);
  static QString ExtractTokenFromFile(const QString& filename);
  static int randomNumber(int min, int max);

private:
  Util();
};

#endif // UTIL_H
