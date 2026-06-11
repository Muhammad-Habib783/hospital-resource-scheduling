#include "greedy_scheduler.h"
#include <iostream>
using namespace std;

// Constructor
GreedyScheduler::GreedyScheduler() 
{
    sortedCount=0;
}

// Score=Emergency + Severity + Degree
int GreedyScheduler::GetScore(const Patient& p) const 
{
    return p.emergency + p.severity + p.degree;
}

// Build sorted order using greedy approach
void GreedyScheduler::BuildOrder(Patient* patients,int count,ConflictGraph& graph) {
    sortedCount=0;

    // Copy admitted patients
    for(int i=0;i<count;i++) 
    {
        if(patients[i].status=="Admitted") 
        {
            sortedPatients[sortedCount++]=patients[i];
        }
    }

    //  Update degree from graph
    for(int i=0;i<sortedCount;i++) 
    {
        sortedPatients[i].degree=graph.GetDegree(sortedPatients[i].id);
    }

    //  Sort by score — highest first
    // Selection sort 
    for(int i=0;i<sortedCount - 1;i++) 
    {
        int maxIdx=i;
        for(int j=i+1;j<sortedCount;j++) 
        {
            if(GetScore(sortedPatients[j])>GetScore(sortedPatients[maxIdx])) 
                {
                maxIdx=j;
            }
        }
        // Swap
        if(maxIdx!=i) 
        {
            Patient temp=sortedPatients[i];
            sortedPatients[i]=sortedPatients[maxIdx];
            sortedPatients[maxIdx]=temp;
        }
    }

    cout<<"Greedy order built for "<<sortedCount<<" patients.\n";
}

// Display sorted order
void GreedyScheduler::DisplayOrder() const 
{
    if(sortedCount==0) 
    {
        cout<<"No patients in greedy order.\n";
        return;
    }

    cout<<"\n--- Greedy Schedule Order ---\n";
    for(int i=0;i<sortedCount;i++) 
    {
        cout<<"Rank "<<i + 1
            <<" | Patient ID: "<< sortedPatients[i].id
            <<" | Name: "<<sortedPatients[i].name
            <<" | Emergency: "<< sortedPatients[i].emergency
            <<" | Severity: "<< sortedPatients[i].severity
            <<" | Degree: "<< sortedPatients[i].degree
            <<" | Score: "<< GetScore(sortedPatients[i])
            <<"\n";
    }
    cout<<"-----------------------------\n";
}