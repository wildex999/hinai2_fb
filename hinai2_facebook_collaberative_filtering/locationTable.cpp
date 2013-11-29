#include "locationTable.h"

#include <stdlib.h>
#include <QDebug>

LocationTable::LocationTable(const QMap<QString, QList<QString> > &locTable)
  : locations_(locTable) {}

QList<QString> LocationTable::getAreasForRegion(const QString &region)
{
  return locations_.value(region);
}

QString LocationTable::getRegion(const QString& area)
{
  foreach(QList<QString> areas, locations_)
    if(areas.contains(area))
      return locations_.key(areas);

  return "";
}

QList<QString> LocationTable::getRegionList() const
{
  return locations_.keys();
}

// Returns a random region from the list.
QString LocationTable::getRandomRegion() const
{
    int random = rand();
  return locations_.keys().at(random % locations_.keys().size());
}

// Returns a random area from a valid region.
QString LocationTable::getRandomAreaByRegion(QString region)
{
  if(!locations_.contains(region))
    return "";

  QList<QString> areas = locations_.value(region);
  return areas.at(rand() % areas.size());
}

