#include "collaberativefiltering.h"
#include <QDebug>
#include <cmath>

CollaberativeFiltering::CollaberativeFiltering()
{
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

    qDebug() << output;
}

void CollaberativeFiltering::makeCalculations()
{
    calculateGroupSums();
    calculateGroupMean();
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
    double k = 0.2;
    double sum = 0.0;
    for(int i = 0; i < nrGroups; i++)
    {
        sum += calculatePearsonCorrelationCoefficient(activeGroup, i)*(groupProductVotes[i][product] - groupMean[i]);
    }

    double predict =  groupMean[activeGroup] + k * sum;
}

double CollaberativeFiltering::calculatePearsonCorrelationCoefficient(int activeGroup, int groups)
{
    double weight = 0.0;

    double nominator = 0.0;
    double denominator = 0.0;
    for(int j = 0; j < nrProducts; j++)
    {
        nominator += (groupProductVotes[activeGroup][j] - groupMean[activeGroup]) *(groupProductVotes[groups][j] - groupMean[groups])  ;
        denominator +=  std::pow(groupProductVotes[activeGroup][j] - groupMean[activeGroup],2) * std::pow(groupProductVotes[groups][j] - groupMean[groups],2);
    }
    weight = nominator/std::sqrt(denominator);

    return weight;
}











