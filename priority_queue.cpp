#include"priority_queue.h"
#include<iostream>
using namespace std;

// Constructor
PatientPriorityQueue::PatientPriorityQueue() 
{
    size=0;
}

// Priority = Emergency + Severity
int PatientPriorityQueue::GetPriority(const Patient& p) const 
{
    return p.emergency + p.severity;
}

// Insert patient into heap
void PatientPriorityQueue::Insert(const Patient& p) 
{
    if(size>=MAX_HEAP_SIZE) 
    {
        cout<<"Priority queue is full!\n";
        return;
    }

    // Add at end
    heap[size]=p;
    size++;

    // Fix heap upward
    HeapifyUp(size - 1);
}

// Extract patient with highest priority
Patient PatientPriorityQueue::ExtractMax() 
{
    if(IsEmpty()) 
    {
        cout<<"Priority queue is empty!\n";
        return Patient(); // empty patient
    }

    // Root has max priority
    Patient maxPatient=heap[0];

    // Move last element to root
    heap[0]=heap[size - 1];
    size--;

    // Fix heap downward
    HeapifyDown(0);

    return maxPatient;
}

// Fix heap upward after insert
void PatientPriorityQueue::HeapifyUp(int index) 
{
    while(index>0) 
    {
        int parent=(index - 1) / 2;

        if(GetPriority(heap[index])>GetPriority(heap[parent])) 
        {
            // Swap
            Patient temp=heap[index];
            heap[index]=heap[parent];
            heap[parent]=temp;
            index=parent;
        } 
        else 
        {
            break;
        }
    }
}

// Fix heap downward after extract
void PatientPriorityQueue::HeapifyDown(int index) 
{
    while (true) 
    {
        int left=2 * index + 1;
        int right=2 * index + 2;
        int largest=index;

        // Check left child
        if(left<size && GetPriority(heap[left]) > GetPriority(heap[largest])) 
        {
            largest=left;
        }

        // Check right child
        if(right < size && GetPriority(heap[right]) > GetPriority(heap[largest])) {
            largest=right;
        }

        // If largest is not root — swap
        if(largest!=index) 
        {
            Patient temp=heap[index];
            heap[index]=heap[largest];
            heap[largest]=temp;
            index=largest;
        } 
        else 
        {
            break;
        }
    }
}

// Check if queue is empty
bool PatientPriorityQueue::IsEmpty() const 
{
    return size==0;
}

// Get current size
int PatientPriorityQueue::GetSize() const 
{
    return size;
}

// Display all patients in queue
void PatientPriorityQueue::Display() const 
{
    if(IsEmpty()) 
    {
        cout<<"Priority queue is empty!\n";
        return;
    }

    cout<<"\n--- Priority Queue ---\n";
    cout<<"Total patients: "<<size<<"\n";
    for(int i=0;i<size;i++) 
    {
        cout<<"Priority: "<<GetPriority(heap[i])
             <<" | Patient ID: "<<heap[i].id
             <<" | Name: "<<heap[i].name
             <<" | Emergency: "<<heap[i].emergency
             <<" | Severity: "<<heap[i].severity<<"\n";
    }
    cout<<"----------------------\n";
}