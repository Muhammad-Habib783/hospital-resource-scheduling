#ifndef PATIENT_H
#define PATIENT_H
#pragma once

#include<iostream>
#include<string>
#include<limits>
#include<ctime>
#include<fstream>
#include<cstring>
#include"resource.h"
using namespace std;

// Basic limits and defaults
const int INITIAL_CAPACITY=100;
const int MAX_NAME_LEN=50; // Patient name
const int MAX_MRN_LEN=20; // Medical Record Number
const int MAX_DIAG_LEN=100; // Diagnosis code (e.g. ICD-10)  International Classification of Diseases
const int MAX_CONTACT_LEN=15;
const int MAX_EMAIL_LEN=50;
const int DEFAULT_DURATION_MINUTES=30;
const int UNASSIGNED_SLOT=-1;

class Patient
{
public:
int id; // unique positive integer
string mrn; //  medical record number
string name;
int age;
char gender; // 'M' 'F' 'O'
int emergency; // 0..100 higher means more urgent
int severity; // 0..10
string diagnosis;
int doctorId; // 0 means no specific doctor requested
int nurseId;  // 0 means no specific nurse requested
int roomId;   // 0 means no specific room requested
int timeSlot; // 0..MAX_SLOTS-1 or UNASSIGNED_SLOT
int durationMinutes;
string contact;
string email;
long long registeredAt; // epoch seconds
int assignedColor; // -1 means unassigned
int degree; // number of conflicts
string status;

Patient()
{
id=0;
mrn="";
name="";
age=0;
gender='O';
emergency=0;
severity=0;
diagnosis="";
doctorId=0;    
nurseId=0;     
roomId=0;      
timeSlot=UNASSIGNED_SLOT;
durationMinutes=DEFAULT_DURATION_MINUTES;
contact="";
email="";
registeredAt=0;
assignedColor=-1;
degree=0;
status="Admitted";
}

// Minimal helper to set registration time
void SetRegisteredNow()
{
registeredAt=(long long)time(NULL);
}

// Print brief summary (implementation in patient.cpp)
void PrintSummary() const;

};

class PatientManager
{
public:
PatientManager();
~PatientManager();

// Core operations
bool AddPatient(); // returns false if duplicate id or failure
bool DeletePatient(ResourceManager& rm); // returns false if not found, also releases any assigned resources
bool DischargePatient(ResourceManager& rm);
bool SearchPatient(); // returns true if found, false otherwise
void DisplayAllPatients() const;

// Persistence (CSV file import/export)
bool SaveToFile(const string& filename) const;
bool SaveAllToFile(const string& filename) const;
bool LoadFromFile(const string& filename);

// Utilities
int GetPatientCount() const;
int FindIndexById(int id) const;
bool IsIdUnique(int id) const;

public:
 
Patient* patients;
int patientCount;
int patientCapacity;

void Resize(); // double capacity when full

// Input helpers 
bool ValidateName(const string& name) const;
bool ValidateMRN(const string& mrn) const;
bool ValidateContact(const string& contact) const;
bool ValidateEmail(const string& email) const;

bool IsIdUniqueGlobal(int id) const;
};

#endif