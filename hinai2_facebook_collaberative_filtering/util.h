#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QMap>

class Util
{
public:
  static QMap<QString, QList<QString> > ExtractLocationsFromCVSFile(const QString& filename);
  static int randomNumber(int min, int max);

private:
  Util();
};

#endif // UTIL_H
