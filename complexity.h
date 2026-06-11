#ifndef COMPLEXITY_H
#define COMPLEXITY_H
#pragma once


#include"patient.h"
#include"resource.h"
#include"graph.h"
#include"greedy_scheduler.h"
#include"graph_coloring.h"
#include<ctime>
using namespace std;

class ComplexityAnalyzer 
{
public:
    ComplexityAnalyzer();

    // Main function
    void RunAnalysis(PatientManager& pm,ResourceManager& rm,ConflictGraph& graph,
    GreedyScheduler& greedy,GraphColoring& coloring);

private:
    // Individual module analysis
    void AnalyzeGraphBuilding(int n,int e);
    void AnalyzePriorityQueue(int n);
    void AnalyzeGreedyOrdering(int n);
    void AnalyzeGraphColoring(int n,int e);
    void ShowComparison(int n);
    void ShowOverall(int n);

    // Helper
    double MeasureTime(clock_t start,clock_t end);
    int CountEdges(ConflictGraph& graph,int n);
    double Log2(int n);
};

#endif