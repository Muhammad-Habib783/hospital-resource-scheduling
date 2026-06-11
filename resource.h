#ifndef RESOURCE_H
#define RESOURCE_H
#pragma once
#include<iostream>
#include<string>
using namespace std;

class Resource 
{
public: 
    int id;                 // Unique resource ID
    string type;            // Doctor, Nurse, Room
    string name;            // Resource name
    string specialization;  // Only for doctors
    bool available;         // Availability flag
    int assignedPatient;    // Patient ID if assigned
    int capacity;      
    int currentLoad;   


    Resource();             // Constructor

    void InputResource();   // Input resource details
    void DisplayResource()  const; // Display resource details

    bool AssignPatient(int patientId); // Assign patient to resource
    void ReleaseResource();            // Free resource
};

// Manager class to handle multiple resources
class ResourceManager 
{
public:
    Resource resources[250]; 
    int count;               // Number of resources stored

public:
    ResourceManager();      // Constructor

    void AddResource();      // Add new resource
    bool DeleteResource(); // Delete resource by ID
    bool SearchResource() const; // Check if resource exists
    void DisplayAll();       // Show all resources
    bool IsResourceAvailable(int id);          // Check if resource is free
    bool AssignPatientToResource(int resourceId,int patientId); // Link patient
    void ReleaseResource(int resourceId);      // Free resource by ID

    // Persistence (CSV file import/export)
    bool SaveToFile(const string& filename) const; // Save all resources
    bool LoadFromFile(const string& filename);     // Load all resources

    bool SearchResourceById(int id) const ;

    
};

#endif
