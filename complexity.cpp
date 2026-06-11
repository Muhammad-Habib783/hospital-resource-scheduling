#include"complexity.h"
#include<iostream>
#include<cmath>
using namespace std;

// Constructor
ComplexityAnalyzer::ComplexityAnalyzer() {}

// Measure time in milliseconds
double ComplexityAnalyzer::MeasureTime(clock_t start,clock_t end)
{
    return double(end - start) / CLOCKS_PER_SEC * 1000.0;
}

// Calculate log base 2
double ComplexityAnalyzer::Log2(int n) 
{
    if(n<=0) 
    return 0;
    return log((double)n) / log(2.0);
}

// Count total edges in graph
int ComplexityAnalyzer::CountEdges(ConflictGraph& graph,int n) 
    {
    int edges=0;
    for(int i=0;i<n;i++) 
    {
        AdjNode* curr=graph.adjList[i].head;
        while(curr!=nullptr) 
        {
            edges++;
            curr=curr->next;
        }
    }
    // Divide by 2 — undirected graph
    return edges / 2;
}

// Analyze graph building
void ComplexityAnalyzer::AnalyzeGraphBuilding(int n,int e) 
{
    cout<<"\n------------------------------------------\n";
    cout<<"    MODULE 1: CONFLICT GRAPH BUILDING       \n";
    cout<<"------------------------------------------\n";

    clock_t start=clock();

    // Simulate n² comparisons
    long long ops=0;
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            ops++;

    clock_t end=clock();

    cout<<"Patients (n)    : "<<n<<"\n";
    cout<<"Conflicts (e)   : "<<e<< "\n";
    cout<<"Comparisons     : n*(n-1)/2 = "<<ops<<"\n";
    cout<<"Time Taken      : "<< MeasureTime(start,end)<<" ms\n";
    cout<<"Complexity      : O(n^2)\n";
}

// Analyze priority queue
void ComplexityAnalyzer::AnalyzePriorityQueue(int n) 
{
    cout<<"\n------------------------------------------\n";
    cout<<"   MODULE 2: PRIORITY QUEUE (MAX HEAP)      \n";
    cout<<"------------------------------------------\n";

    clock_t start=clock();

    // Simulate heap operations
    long long ops=0;
    for(int i=0;i<n;i++)
        ops += (int)Log2(i + 1); // log n per insert

    clock_t end=clock();

    cout<<"Patients (n)    : "<<n<<"\n";
    cout<<"log2(n)         : "<< Log2(n)<<"\n";
    cout<<"Insert ops      : n*log(n) = "<<ops<<"\n";
    cout<<"Time Taken      : "<<MeasureTime(start,end)<<" ms\n";
    cout<<"Per Insert      : O(log n)\n";
    cout<<"Build Heap      : O(n log n)\n";
}

// Analyze greedy ordering
void ComplexityAnalyzer::AnalyzeGreedyOrdering(int n) 
{
    cout<<"\n------------------------------------------\n";
    cout<<"     MODULE 3: GREEDY ORDERING             \n";
    cout<<"------------------------------------------\n";

    clock_t start=clock();

    // Simulate selection sort
    long long ops=0;
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            ops++;

    clock_t end=clock();

    cout<<"Patients (n)    : "<<n<<"\n";
    cout<<"Comparisons     : n*(n-1)/2 = "<<ops<<"\n";
    cout<<"Time Taken      : "<<MeasureTime(start,end)<<" ms\n";
    cout<<"Complexity    : O(n^2)\n";
}

// Analyze graph coloring
void ComplexityAnalyzer::AnalyzeGraphColoring(int n,int e) 
{
    cout<<"\n------------------------------------------\n";
    cout<<"         MODULE 4: GRAPH COLORING          \n";
    cout<<"------------------------------------------\n";

    clock_t start=clock();

    // Simulate coloring operations
    long long ops=n+e;

    clock_t end=clock();

    cout<<"Patients (n)    : "<<n<<"\n";
    cout<<"Conflicts (e)   : "<<e<<"\n";
    cout<<"Operations      : n + e = "<<ops<<"\n";
    cout<<"Time Taken      : "<<MeasureTime(start,end)<<" ms\n";
    cout<<"Complexity      : O(n + e)\n";
}

// Show comparison table
void ComplexityAnalyzer::ShowComparison(int n) 
{
    cout<<"\n------------------------------------------\n";
    cout<<"       COMPARISON WITH OTHER METHODS        \n";
    cout<<"------------------------------------------\n";
    cout<<"Method        Complexity  Conflicts  Priority\n";
    cout<<"------------- ----------  ---------  --------\n";
    cout<<"Manual        O(n^2)         NO         NO\n";
    cout<<"FCFS          O(n)           NO         NO\n";
    cout<<"Round Robin   O(n)           NO         NO\n";
    cout<<"Our Hybrid    O(n^2)         YES        YES\n";
    cout<<"------------------------------------------\n";
    cout<<"With n = "<<n<<" patients:\n";
    cout<<"FCFS ops      : "<<n<<"\n";
    cout<<"Our system ops: " << (long long)n * n<<"\n";
    cout<<"Extra cost    : Conflict-free + Priority!\n";
}

// Show overall complexity
void ComplexityAnalyzer::ShowOverall(int n) 
{
    cout<<"\n------------------------------------------\n";
    cout<<"OVERALL SYSTEM COMPLEXITY\n";
    cout<<"------------------------------------------\n";
    cout<<" Dominant Step  : Graph Building\n";
    cout<<" Overall        : O(n^2)\n";
    cout<<" \nWHY O(n^2)?\n";
    cout<<" Graph building compares\n";
    cout<<" Every Patient With other!\n";
    cout<<" n=" << n << " with patinets :\n";
    cout<<"Max operations = "<<(long long)n * n<<"\n";
    cout<<"------------------------------------------\n";
    cout<<"Space Complexity : O(n + e)\n";
    cout<<"  n = patients\n";
    cout<<"  e = conflict edges\n";
    cout<<"------------------------------------------\n";
}

// Main analysis function
void ComplexityAnalyzer::RunAnalysis(PatientManager& pm,ResourceManager& rm,
    ConflictGraph& graph,GreedyScheduler& greedy,GraphColoring& coloring) 
    {

    int n=pm.patientCount;
    int e=CountEdges(graph,graph.nodeCount);

    cout<<"\n";
    cout<<"==========================================\n";
    cout<<"      COMPLEXITY ANALYSIS REPORT\n";
    cout<<"==========================================\n";
    cout<<"Total Patients (n) : "<<n<<"\n";
    cout<<"Total Conflicts (e): "<<e<<"\n";

    // Analyze each module
    AnalyzeGraphBuilding(n,e);
    AnalyzePriorityQueue(n);
    AnalyzeGreedyOrdering(n);
    AnalyzeGraphColoring(n,e);

    // Comparison
    ShowComparison(n);

    // Overall
    ShowOverall(n);

    cout<<"\n==========================================\n";
    cout<<"         ANALYSIS COMPLETE!\n";
    cout<<"==========================================\n";
}