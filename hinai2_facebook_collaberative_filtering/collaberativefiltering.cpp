#include "collaberativefiltering.h"
#include <QDebug>
#include <cmath>

CollaberativeFiltering::CollaberativeFiltering()
{
    srand (time(NULL));
    nrGroups = 14;
    nrProducts = 8;

    for(int i = 0; i < nrGroups; i++)
    {
        QList<int> temp;
        for(int j = 0; j < nrProducts; j++)
        {
            temp.push_back(0);
        }
        groupProductVotes.push_back(temp);

    }

    for(int i = 0; i < nrGroups; i++)
    {
        groupSum.push_back(0);
        groupMean.push_back(0);
    }
}

void CollaberativeFiltering::addToTable(GROUP group, PRODUCT product)
{
    groupProductVotes[int(group)][int(product)]++;
}

void CollaberativeFiltering::writeToDebug()
{
    QString output = "\n";

    for(int i = 0; i < nrGroups; i++)
    {
        for(int j = 0; j < nrProducts; j++)
        {
            output += QString::number(groupProductVotes[i][j]) + " ";
        }
        output += QString::number(groupSum[i]) + " " + QString::number(groupMean[i]) +  "\n";
    }

    double error = std::fabs((predictVote(male,fifa_14) - groupProductVotes[male][fifa_14]) / (2*(predictVote(male,fifa_14) + groupProductVotes[male][fifa_14]))) * 100;

    output += "Group male likes fifa 14 this much: " +  QString::number(predictVote(male,fifa_14)) + " real value: " + QString::number(groupProductVotes[male][fifa_14]) + " difference: " + QString::number(error) + "%" ;

    qDebug() << output;
}

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
            groupProductVotes[i][j] = rnd;
        }
    }

//    groupProductVotes[male][fifa_14] = 0;
}

void CollaberativeFiltering::calculateGroupSums()
{
    for(int i = 0; i < nrGroups;i++)
    {
        int sum = 0;
        for(int j = 0; j < nrProducts; j++)
        {
            sum += groupProductVotes[i][j];
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
            if(groupProductVotes[i][j]>0)
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
        double group = groupProductVotes[i][product] - groupMean[i];
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
        double active = groupProductVotes[activeGroup][j] - groupMean[activeGroup];
        double group = groupProductVotes[groups][j] - groupMean[groups];

        nominator += active *group  ;
        denominator +=  std::sqrt(active*active) * std::sqrt(group*group);
    }
    if(denominator>0)
        weight = nominator/denominator;

    qDebug() << QString::number(weight);
    return weight;
}











