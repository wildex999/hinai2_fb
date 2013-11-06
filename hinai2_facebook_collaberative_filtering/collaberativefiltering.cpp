#include "collaberativefiltering.h"
#include <QDebug>
#include <cmath>

CollaberativeFiltering::CollaberativeFiltering()
{
    srand (time(NULL));
    nrGroups = 28;
    nrProducts = 8;

    for(int i = 0; i < nrGroups; i++)
    {
        QList<int> temp;
        for(int j = 0; j < nrProducts; j++)
        {
            temp.push_back(0);
        }
        groupTestProductVotes.push_back(temp);
        groupTrainProductVotes.push_back(temp);
        groupProductVotes.push_back(temp);

    }

    for(int i = 0; i < nrGroups; i++)
    {
        groupSum.push_back(0);
        groupMean.push_back(0);
    }

    makeGroupMap();
    makeProductMap();
}

void CollaberativeFiltering::addToTable(GROUP group, PRODUCT product)
{
    groupProductVotes[int(group)][int(product)]++;
}

void CollaberativeFiltering::addToTestTable(GROUP group, PRODUCT product)
{
    groupTestProductVotes[int(group)][int(product)]++;
}

void CollaberativeFiltering::addToTrainTable(GROUP group, PRODUCT product)
{
    groupTrainProductVotes[int(group)][int(product)]++;
}

int CollaberativeFiltering::getTableValue(int group, int product)
{
    return groupTestProductVotes[group][product];
}

int CollaberativeFiltering::getnrGroupsValue()
{
    return nrGroups;
}

int CollaberativeFiltering::getnrProductsValue()
{
    return nrProducts;
}

void CollaberativeFiltering::splitDataset()
{
    for(int i = 0; i < nrGroups; i++)
    {
        for(int j = 0; j < nrProducts; j++)
        {
            int getVal = groupProductVotes[i][j];
            int testVal = getVal*0.7;
            groupTrainProductVotes[i][j] = testVal;
            groupTestProductVotes[i][j] = getVal-testVal;
        }
    }
}

void CollaberativeFiltering::train()
{
}
void CollaberativeFiltering::writeToDebug()
{
    QString output = "\n";

    for(int i = 0; i < nrGroups; i++)
    {
        for(int j = 0; j < nrProducts; j++)
        {
            output += QString::number(groupTestProductVotes[i][j]) + " ";
        }
        output += QString::number(groupSum[i]) + " " + QString::number(groupMean[i]) +  "\n";
    }

    //    double error = std::fabs(predictVote(male,fifa_14)*0.7 - groupTrainProductVotes[male][fifa_14]*0.3) / (groupTrainProductVotes[male][fifa_14]* 0.3) * 100;

    //    output += "Group male likes fifa 14 this much: " +  QString::number(predictVote(male,fifa_14)) + " real value: " + QString::number(groupTestProductVotes[male][fifa_14]) + " difference: " + QString::number(error) + "%" ;

    for(int i = 0; i < nrGroups; i++)
    {
        for(int j = 0; j < nrProducts; j++)
        {
            int check = groupTestProductVotes[i][j];
            if(check == 0)
            {
                double predictedValue = predictVote(i,j);
                int trainedValue = groupTrainProductVotes[i][j];
                if(trainedValue>0)
                {
                    double scaledTrainedValue = double(trainedValue)*0.3;
                    double scaledPredictedValue = predictedValue*0.7;
                    double diff = scaledPredictedValue - scaledTrainedValue;
                    double percent = std::fabs(diff)/scaledTrainedValue * 100;
                    output += getGroupName(i) + " likes " + getProductName(j) + " this much: " + QString::number(scaledPredictedValue) + " and trained vaalue is: " + QString::number(scaledTrainedValue) + " error: " + QString::number(percent) +"%\n";
                }
            }
        }
    }

    qDebug() << output;
}

//void CollaberativeFiltering::addData(QList<GROUP> group, QList<PRODUCT> product)
//{
////    int test = group.count()/3;
////    for(int i = 0; i < group.count())   //split
//}

void CollaberativeFiltering::makeCalculations()
{
    calculateGroupSums();
    calculateGroupMean();
}

void CollaberativeFiltering::generateRandomData()
{
    for(int i = 0; i < nrGroups; i++)
    {
        for(int j = 0; j < nrProducts; j++)
        {
            double rnd = rand() % 100;
            groupTestProductVotes[i][j] = rnd;
        }
    }

    //    groupProductVotes[male][fifa_14] = 0;
}

QString CollaberativeFiltering::getGroupName(int group)
{
    QString name = "";
    QMap<GROUP, QString>::iterator i;
    for (i = groupmap.begin(); i != groupmap.end(); ++i)
        if(i.key() == group)
            name = i.value();

    return name;
}

QString CollaberativeFiltering::getProductName(int product)
{
    QString name = "";
    QMap<PRODUCT, QString>::iterator i;
    for (i = productmap.begin(); i != productmap.end(); ++i)
        if(i.key() == product)
            name = i.value();

    return name;
}

void CollaberativeFiltering::calculateGroupSums()
{
    for(int i = 0; i < nrGroups;i++)
    {
        int sum = 0;
        for(int j = 0; j < nrProducts; j++)
        {
            sum += groupTrainProductVotes[i][j];
        }
        groupSum[i] = sum;
    }
}

void CollaberativeFiltering::calculateGroupMean()
{
    QList<int> groupI;
    for(int i = 0; i < nrGroups;i++)
    {
        int count = 0;
        for(int j = 0; j < nrProducts; j++)
        {
            if(groupTrainProductVotes[i][j]>0)
                count++;
        }
        groupI.push_back(count);
    }


    for(int i = 0; i < nrGroups;i++)
    {
        double mean = 0.0;

        if(groupI[i]!=0)
        {
            mean = 1.0/groupI[i] *groupSum[i];
        }

        groupMean[i] = mean;
    }
}

double CollaberativeFiltering::predictVote(int activeGroup, int product)
{
    double k = 1.0;
    double sum = 0.0;
    double weightSum = 0.0;
    for(int i = 0; i < nrGroups; i++)
    {
        double group = groupTestProductVotes[i][product] - groupMean[i];
        double coefficent = calculatePearsonCorrelationCoefficient(activeGroup, i);
        sum += coefficent*group;
        weightSum += std::fabs(coefficent);
    }

    double predict =  groupMean[activeGroup] + k * sum / weightSum;

    return predict;
}

double CollaberativeFiltering::calculatePearsonCorrelationCoefficient(int activeGroup, int groups)
{
    double weight = 0.0;

    double nominator = 0.0;
    double denominator = 0.0;
    for(int j = 0; j < nrProducts; j++)
    {
        double active = groupTestProductVotes[activeGroup][j] - groupMean[activeGroup];
        double group = groupTestProductVotes[groups][j] - groupMean[groups];

        nominator += active *group  ;
        denominator +=  std::sqrt(active*active) * std::sqrt(group*group);
    }
    if(denominator>0)
        weight = nominator/denominator;

    //qDebug() << QString::number(weight);
    return weight;
}

void CollaberativeFiltering::makeGroupMap()
{
    groupmap.insert(male, "male");
    groupmap.insert(female,"female");
    groupmap.insert(undefined, "undefined");
    groupmap.insert(age14_20, "age14_20");
    groupmap.insert(age20_30,"age20_30");
    groupmap.insert(age30_40, "age30_40");
    groupmap.insert(age40_50,"age40_50");
    groupmap.insert(age50_60, "age50_60");
    groupmap.insert(age70_pluss,"age70_pluss");
    groupmap.insert(ostfold,"ostfold");
    groupmap.insert(akershus,"akershus");
    groupmap.insert(oslo, "oslo");
    groupmap.insert(hedmark, "hedmark");
    groupmap.insert(oppland,"oppland");
    groupmap.insert(buskerud, "buskerud");
    groupmap.insert(vestfold,"vestfold");
    groupmap.insert(telemark, "telemark");
    groupmap.insert(aust_agder,"aust_agder");
    groupmap.insert(vest_agder,"vest_agder");
    groupmap.insert(rogaland,"rogaland");
    groupmap.insert(hordaland, "hordaland");
    groupmap.insert(sogn_og_fjordane, "sogn_og_fjordane");
    groupmap.insert(more_og_romsdal,"more_og_romsdal");
    groupmap.insert(sor_trondelag, "sor_trondelag");
    groupmap.insert(nord_trondelag,"nord_trondelag");
    groupmap.insert(nordland, "nordland");
    groupmap.insert(troms,"troms");
    groupmap.insert(finmark,"finmark");


}

void CollaberativeFiltering::makeProductMap()
{

    productmap.insert(ipad_mini, "ipad mini");
    productmap.insert(ipad_air,"ipad air");
    productmap.insert(google_nexus, "google nexus");
    productmap.insert(fifa_14,"fifa 14");
    productmap.insert(assasins_creed_4, "assasins creed_4");
    productmap.insert(microsoft_surface_rt_64_gb,"microsoft surface rt 64 gb");
    productmap.insert(samsung_smart_watch_galaxy_gear, "samsung smart watch galaxy gear");
    productmap.insert(samsung_galaxy_s_iv, "samsung galaxy s iv");
}











