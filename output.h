#ifndef OUTPUT_H
#define OUTPUT_H
#pragma once

#include"patient.h"
#include"resource.h"
#include"graph.h"
#include"graph_coloring.h"
using namespace std;

class ReportGenerator 
{
public:
    ReportGenerator();

    // Console report
    void PrintReportConsole(Patient* patients,int count,
    ResourceManager& rm,GraphColoring& coloring); 

    // HTML report
    void GenerateHTMLReport(Patient* patients,int count,
    ResourceManager& rm,GraphColoring& coloring); 

    // Generate both reports
    void GenerateReport(Patient* patients,int count,ResourceManager& rm,
    GraphColoring& coloring);

private:
    // Helper functions
    string GetResourceName(int id, ResourceManager& rm);
    string GetResourceSpec(int id, ResourceManager& rm);
    string FormatTime(long long epoch);
};

#endif