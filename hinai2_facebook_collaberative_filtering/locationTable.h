#ifndef LOCATIONTABLE_H
#define LOCATIONTABLE_H

#include <QString>
#include <QMap>
#include <QList>

class LocationTable
{
public:
  explicit LocationTable(const QMap<QString, QList<QString> >& locTable);

  QList<QString> getAreasForRegion(const QString& region);
  QString getRegion(const QString& area);
  QList<QString> getRegionList() const;

  // Temporary bogus data such that for the rest of the simulation.
  QString getRandomRegion() const;
  QString getRandomAreaByRegion(QString region);

private:
  QMap<QString, QList<QString> > locations_;

};

#endif // LOCATION_H
