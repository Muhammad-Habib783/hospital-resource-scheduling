#include"graph_coloring.h"
#include<iostream>
using namespace std;

//  Constructor
GraphColoring::GraphColoring() 
{
    totalSlotsUsed=0;
}

// Convert slot number to time string
string GraphColoring::SlotToTime(int slot) const 
{
    // Slot 1 = 9:00 AM, each slot = 30 mins
    int totalMinutes=(slot - 1) * 30;
    int hour=9 + totalMinutes / 60;
    int minute=totalMinutes % 60;

    string period="AM";
    if(hour>=12) 
    {
        period="PM";
        if(hour>12) 
        hour -= 12;
    }

    string hourStr=(hour < 10 ? "0" : "") + to_string(hour);
    string minStr=(minute < 10 ? "0" : "") + to_string(minute);

    return hourStr + ":" + minStr + " " + period;
}

// Check if color (time slot) is safe for patient
bool GraphColoring::IsSafeColor(int patientId,int color,ConflictGraph& graph,
Patient* patients,int count) 
{
    int idx=graph.GetNodeIndex(patientId);
    if(idx== -1) return true;

    AdjNode* curr=graph.adjList[idx].head;
    while(curr!=nullptr) 
    {
        for(int i=0;i<count;i++) 
        {
            if(patients[i].id==curr->patientId) 
            {
                if(patients[i].assignedColor==color) 
                {
                    return false; // conflict with neighbor
                }
                break;
            }
        }
        curr=curr->next;
    }
    return true;
}

// Assign time slots (colors) to all patients
void GraphColoring::AssignColors(GreedyScheduler& gs,ConflictGraph& graph,
Patient* patients,int count) 
{
    totalSlotsUsed=0;

    // Process patients in greedy order
    for(int i=0;i<gs.sortedCount;i++) 
    {
        int pid=gs.sortedPatients[i].id;

        // Find patient in main array
        int pIdx= -1;
        for(int j=0;j<count;j++) 
        {
            if(patients[j].id==pid) 
            {
                pIdx=j;
                break;
            }
        }
        if(pIdx== -1) continue;

        // Try each slot until safe one found
        int assigned= -1;
        for(int color=1;color<=MAX_SLOTS;color++) 
        {
            if(IsSafeColor(pid,color,graph,patients,count)) 
            {
                patients[pIdx].assignedColor=color;
                patients[pIdx].timeSlot=color;
                assigned=color;

                if(color>totalSlotsUsed) 
                {
                    totalSlotsUsed=color;
                }
                break;
            }
        }

        // If no safe slot found, force assign next available
        if(assigned== -1) 
        {
            patients[pIdx].assignedColor=++totalSlotsUsed;
            patients[pIdx].timeSlot=totalSlotsUsed;
        }
    }

    cout<<"Graph coloring complete!\n";
    cout<<"Total time slots used: "<<totalSlotsUsed<<"\n";
}

// Display coloring result
void GraphColoring::DisplayColoring(Patient* patients,int count) const 
{
    cout<<"\n--- Time Slot Assignment ---\n";
    for(int i=0;i<count;i++) 
    {
        if(patients[i].status!="Admitted") continue;
        if(patients[i].assignedColor== -1) continue;

        cout<<"Patient ID: "<< patients[i].id
            <<" | Name: "<< patients[i].name
            <<" | Slot: "<< patients[i].assignedColor
            <<" | Time: "<< SlotToTime(patients[i].assignedColor)
            <<"\n";
    }
    cout << "---------------------------\n";
}
