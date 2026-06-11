#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#pragma once
#include"patient.h"
using namespace std;

const int MAX_HEAP_SIZE=500;
class PatientPriorityQueue 
{
public:
    PatientPriorityQueue();

    // Core operations
    void Insert(const Patient& p);
    Patient ExtractMax();
    bool IsEmpty() const;
    int GetSize() const;
    void Display() const;

    // Heap operations
    void HeapifyUp(int index);
    void HeapifyDown(int index);

    // Priority calculation
    int GetPriority(const Patient& p) const;

private:
    Patient heap[MAX_HEAP_SIZE]; // array based heap
    int size;                    // current heap size
};

#endif