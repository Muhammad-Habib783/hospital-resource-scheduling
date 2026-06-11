#include "graph.h"
#include <iostream>
using namespace std;

//  Constructor
ConflictGraph::ConflictGraph()
{
    nodeCount=0;
    adjList=new AdjList[MAX_PATIENTS];
    for(int i=0;i<MAX_PATIENTS;i++)
    {
        patientIds[i]= -1;
    }
}

// Destructor
ConflictGraph::~ConflictGraph()
{
    ResetGraph();
    delete[] adjList;
}

// Reset graph
void ConflictGraph::ResetGraph()
{
    for(int i=0;i<nodeCount;i++)
    {
        AdjNode* curr=adjList[i].head;
        while(curr!=nullptr)
        {
            AdjNode* temp=curr;
            curr=curr->next;
            delete temp;
        }
        adjList[i].head=nullptr;
    }
    nodeCount=0;
}

// Get index of patient in patientIds array
int ConflictGraph::GetNodeIndex(int patientId) const
{
    for(int i=0;i<nodeCount;i++)
    {
        if(patientIds[i]==patientId)
            return i;
    }
    return -1;
}

// Add edge between two patients (undirected)
void ConflictGraph::AddEdge(int fromId, int toId)
{
    int fromIdx=GetNodeIndex(fromId);
    int toIdx=GetNodeIndex(toId);

    if(fromIdx== -1||toIdx== -1) return;

    // Check if edge already exists
    AdjNode* curr=adjList[fromIdx].head;
    while(curr!=nullptr)
    {
        if(curr->patientId==toId) return;
        curr=curr->next;
    }

    // Add toId in fromId's list
    AdjNode* newNode1=new AdjNode(toId);
    newNode1->next=adjList[fromIdx].head;
    adjList[fromIdx].head=newNode1;

    // Add fromId in toId's list (undirected)
    AdjNode* newNode2=new AdjNode(fromId);
    newNode2->next=adjList[toIdx].head;
    adjList[toIdx].head=newNode2;
}


bool ConflictGraph::AreConflicting(Patient& p1, Patient& p2)
{
    // Same doctor → conflict
    if(p1.doctorId!=0 && p2.doctorId!=0 && p1.doctorId==p2.doctorId)
    {
        return true;
    }

    // Same nurse → conflict
    if(p1.nurseId!=0 && p2.nurseId!=0 && p1.nurseId==p2.nurseId)
    {
        return true;
    }

    // Same room → conflict
    if(p1.roomId!=0 && p2.roomId!=0 && p1.roomId==p2.roomId)
    {
        return true;
    }

    return false;
}

// Build conflict graph from patients array
void ConflictGraph::BuildGraph(Patient* patients,int count)
{
    ResetGraph();

    //  Add all admitted patients as nodes
    for(int i=0;i<count;i++)
    {
        if(patients[i].status=="Admitted")
        {
            patientIds[nodeCount++]=patients[i].id;
        }
    }

    // Check every pair for conflicts
    for(int i=0;i<count;i++)
    {
        if(patients[i].status!="Admitted") continue;

        for(int j=i + 1;j<count;j++)
        {
            if(patients[j].status!="Admitted") continue;

            if(AreConflicting(patients[i],patients[j]))
            {
                AddEdge(patients[i].id,patients[j].id);
                patients[i].degree++;
                patients[j].degree++;
            }
        }
    }

    cout<<"Conflict graph built with "<<nodeCount<<" patients.\n";
}

// Get degree of a patient
int ConflictGraph::GetDegree(int patientId) const
{
    int idx=GetNodeIndex(patientId);
    if(idx== -1) return 0;

    int degree=0;
    AdjNode* curr=adjList[idx].head;
    while(curr!=nullptr)
    {
        degree++;
        curr=curr->next;
    }
    return degree;
}

// Display graph
void ConflictGraph::DisplayGraph() const
{
    cout<<"\n--- Conflict Graph ---\n";
    for(int i=0;i<nodeCount;i++)
    {
        cout<<"Patient "<<patientIds[i]<<" conflicts with: ";
        AdjNode* curr=adjList[i].head;
        if(curr==nullptr)
        {
            cout<<"None";
        }
        while(curr!=nullptr)
        {
            cout<<curr->patientId;
            if(curr->next!=nullptr) cout<<", ";
            curr=curr->next;
        }
        cout<<"\n";
    }
    cout<<"----------------------\n";
}