#ifndef COLLABERATIVEFILTERING_H
#define COLLABERATIVEFILTERING_H

#include <QString>
#include <QList>
#include <QMap>



enum GROUP{
    male,
    female,
    undefined,
    age14_20,
    age20_30,
    age30_40,
    age40_50,
    age50_60,
    age70_pluss,
    ostfold,
    akershus,
    oslo,
    hedmark,
    oppland,
    buskerud,
    vestfold,
    telemark,
    aust_agder,
    vest_agder,
    rogaland,
    hordaland,
    sogn_og_fjordane,
    more_og_romsdal,
    sor_trondelag,
    nord_trondelag,
    nordland,
    troms,
    finmark
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
    void                splitDataset();

    void                addToTestTable(GROUP group, PRODUCT product);
    void                addToTrainTable(GROUP group, PRODUCT product);

    void                writeToDebug();

    //void                addData(); //
    void                makeCalculations();
    void                generateRandomData();
    QString             getGroupName(int group);
    QString             getProductName(int product);

    int                 getTableValue(int group, int product);
    int                 getnrGroupsValue();
    int                 getnrProductsValue();

    QList<QList<int> > getTestVotes();
    QList<QList<int> > getTrainVotes();

    private:
    QList<QList<int> >  groupTestProductVotes;
    QList<QList<int> >  groupTrainProductVotes;
    QList<QList<int> >  groupProductVotes;
    QList<int>          groupSum;
    QList<double>       groupMean;
    QList<double>       groupWeight;
    QMap<GROUP, QString> groupmap;
    QMap<PRODUCT, QString> productmap;

    int                 nrGroups;
    int                 nrProducts;



    void                train();
    void                makeGroupMap();
    void                makeProductMap();
    void                calculateGroupSums();
    void                calculateGroupMean();
    double              predictVote(int activeGroup, int product);
    double              calculatePearsonCorrelationCoefficient(int activeGroup, int groups);

};

#endif // COLLABERATIVEFILTERING_H
