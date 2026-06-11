#ifndef GRAPH_H
#define GRAPH_H
#pragma once

#include"patient.h"
#include"resource.h"
using namespace std;

const int MAX_PATIENTS=500;

// Adjacency list node
struct AdjNode 
{
    int patientId;   // connected patient ID
    AdjNode* next;   // next node in list

    AdjNode(int id) 
    {
        patientId=id;
        next=nullptr;
    }
};

// Adjacency list for one patient
struct AdjList 
{
    AdjNode* head;

    AdjList() 
    {
        head=nullptr;
    }
};

// Graph class
class ConflictGraph 
{
public:
    AdjList* adjList;    // array of adjacency lists
    int patientIds[MAX_PATIENTS]; // patient IDs
    int nodeCount;       // number of patients/nodes

    ConflictGraph();
    ~ConflictGraph();

    // Core operations
    void BuildGraph(Patient* patients,int count);
    void AddEdge(int fromId,int toId);
    bool AreConflicting(Patient& p1,Patient& p2);
    int  GetDegree(int patientId) const;
    int  GetNodeIndex(int patientId) const;
    void DisplayGraph() const;
    void ResetGraph();
};

#endif