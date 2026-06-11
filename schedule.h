#ifndef SCHEDULE_H
#define SCHEDULE_H
#pragma once

#include"patient.h"
#include"resource.h"
#include"graph.h"
#include"priority_queue.h"
#include"greedy_scheduler.h"
#include"graph_coloring.h"
using namespace std;

class Scheduler 
{
public:
    Scheduler();

    // Main function 
    void RunSchedule(PatientManager& pm,ResourceManager& rm);

    // On the basis on diagnosis assign doctor
    bool AssignResources(Patient& p,ResourceManager& rm);

    // Diagnosis to specialization mapping
    string GetSpecialization(const string& diagnosis);

    // Case insensitive conversion
    string ToLower(const string& str);

    // Save schedule to CSV
    bool SaveSchedule(Patient* patients,int count,ResourceManager& rm);

    // Display schedule
    void DisplaySchedule(Patient* patients,int count,ResourceManager& rm);

public:
    ConflictGraph graph;
    GreedyScheduler greedy;
    GraphColoring coloring;
};

#endif