#ifndef COLLABERATIVEFILTERING_H
#define COLLABERATIVEFILTERING_H

#include <QString>
#include <QList>
#include <QMap>



enum GROUP{
    male,
    female,
    age14_20,
    age20_30,
    age30_40,
    age40_50,
    age50_60,
    age70_pluss,
    ostlandet,
    sorlandet,
    vestlandet,
    trondelag,
    nord_norge,
    utlandet
};

enum PRODUCT{
    ipad_mini,
    ipad_air,
    google_nexus,
    fifa_14,
    assasins_creed_4,
    microsoft_surface_rt_64_gb,
    samsung_smart_watch_galaxy_gear,
    samsung_galaxy_s_iv
};

enum TYPE{
    pads,
    mobile_phones,
    portable_computers,
    games,
    wearable_electronics,
    tvs
};

class CollaberativeFiltering
{
    public:
    CollaberativeFiltering();

    void                addToTable(GROUP group, PRODUCT product);
    void                writeToDebug();

    void                makeCalculations();
    void                generateRandomData();
    QString             getGroupName(int group);
    QString             getProductName(int product);

    int                 getTableValue(int group, int product);
    int                 getnrGroupsValue();
    int                 getnrProductsValue();

    private:
    QList<QList<int> >  groupProductVotes;
    QList<int>          groupSum;
    QList<double>       groupMean;
    QList<double>       groupWeight;
    QMap<GROUP, QString> groupmap;
    QMap<PRODUCT, QString> productmap;

    int                 nrGroups;
    int                 nrProducts;


    void                makeGroupMap();
    void                makeProductMap();
    void                calculateGroupSums();
    void                calculateGroupMean();
    double              predictVote(int activeGroup, int product);
    double              calculatePearsonCorrelationCoefficient(int activeGroup, int groups);

};

#endif // COLLABERATIVEFILTERING_H
