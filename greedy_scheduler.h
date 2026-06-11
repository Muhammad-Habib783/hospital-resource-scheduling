#ifndef GREEDY_SCHEDULER_H
#define GREEDY_SCHEDULER_H
#pragma once

#include"patient.h"
#include"graph.h"
#include"priority_queue.h"
using namespace std;

class GreedyScheduler 
{
public:
    // Stores Sorted order 
    Patient sortedPatients[MAX_PATIENTS];
    int sortedCount;

    GreedyScheduler();

    // Core function
    void BuildOrder(Patient* patients,int count,ConflictGraph& graph); 

    // Score Calculates the score
    int GetScore(const Patient& p) const;

    // Display sorted order
    void DisplayOrder() const;
};

#endif