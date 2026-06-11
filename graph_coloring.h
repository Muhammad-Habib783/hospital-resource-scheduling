#ifndef GRAPH_COLORING_H
#define GRAPH_COLORING_H
#pragma once

#include"patient.h"
#include"graph.h"
#include"greedy_scheduler.h"
using namespace std;

const int MAX_SLOTS=20; // 20 time slots (9AM to 7PM)

class GraphColoring 
{
public:
    GraphColoring();

    // Core functions
    void AssignColors(GreedyScheduler& gs,ConflictGraph& graph,Patient* patients,int count);

    bool IsSafeColor(int patientId, int color,ConflictGraph& graph,
    Patient* patients,int count);

    // Slot number to time string
    string SlotToTime(int slot) const;

    // Display result
    void DisplayColoring(Patient* patients,int count) const;

    int totalSlotsUsed; // kitne slots use hue
};

#endif