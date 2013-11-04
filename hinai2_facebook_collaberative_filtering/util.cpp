#include "util.h"

#include <QFile>
#include <assert.h>
#include <QList>
#include <QDebug>
#include <QStringList>

Util::Util()
{
}

// TODO: Should probably make this a general CVS parser and put the specific stuff into another method.
// Lacks: Error handling and robustness.
QMap<QString, QList<QString> > Util::ExtractLocationsFromCVSFile(const QString &filename)
{

  QMap<QString, QList<QString> > locations;
  QStringList fields;
  QFile file(filename);

  if(file.open(QFile::ReadOnly))
  {
    QTextStream in(&file);
    in.setCodec("UTF-8");
    in.readLine(); // Hack to read past first header line.
    while(!in.atEnd())
    {
      QString line = in.readLine();
      fields = line.split(';');
      assert(fields.size() == 5); // Expecting exactly 5 columns.
      //qDebug() << "Fylke: " << fields[1] << ". Kommune: " << fields[3];

      QList<QString> asd;
      if(!locations.contains(fields[1]))
        locations.insert(fields[1],asd);

      QList<QString> value = locations.value(fields[1]);
      if(!value.contains(fields[3]))
      {
        value.append(fields[3]);
        locations.insert(fields[1], value);
      }
    }
    file.close();
  }
  return locations;
}

QString Util::ExtractTokenFromFile(const QString &filename)
{
  QString token;
  QFile file(filename);

  if(file.open(QFile::ReadOnly))
  {
    QTextStream in(&file);
    in.setCodec("UTF-8");
    token = in.readLine();
    file.close();
  }
  return token;
}

int Util::randomNumber(int min, int max)
{
  assert(min < max && "min was >= than max");
  return static_cast<int>((rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min);
}
