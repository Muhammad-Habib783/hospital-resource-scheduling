#include"resource.h"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<limits>
#include<algorithm>
#include<iomanip>   // for setw
using namespace std;


/// Constructor: initialize safe defaults
Resource::Resource() 
{
    id=0;
    type="";
    name="";
    specialization="-";
    available=true;
    assignedPatient=0;
    capacity=1;       // (default 1)
    currentLoad=0;    
}

void Resource::InputResource() 
{
    cout << "Enter resource ID: ";
while (true) {
    string input;
    getline(cin, input);

    if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
        int id = stoi(input);

        // check for duplicate in file
        ifstream ifs("resources.csv");
        string line;
        getline(ifs, line); // skip header
        bool duplicateFound = false;

        while (getline(ifs, line)) {
            stringstream ss(line);
            string idStr;
            getline(ss, idStr, ','); // first column

            if (idStr == to_string(id)) {
                duplicateFound = true;
                break;
            }
        }
        ifs.close();

        if (duplicateFound) {
            cout << "Duplicate ID found. Try another.\n";
            continue;
        }

        // ID is valid and unique
        break;
    } else {
        cout << "Invalid input. Enter numeric resource ID: ";
    }
}


    cout<<"Enter resource type (Doctor/Nurse/Room): ";
    while(true) 
    {
        getline(cin, type);
        string lowerType;
        lowerType.resize(type.size());
        transform(type.begin(), type.end(), lowerType.begin(),
                  [](unsigned char c){ return tolower(c); });
        if(lowerType == "doctor" || lowerType == "nurse" || lowerType == "room") 
        {
            break;
        } 
        else 
        {
            cout<<"Invalid type. Please enter only Doctor, Nurse, or Room: ";
        }
    }

    cout<<"Enter resource name: ";
    while(true) 
    {
        getline(cin, name);
        if(!name.empty() && all_of(name.begin(), name.end(), [](char c) 
        {
            return isalpha(c) || isspace(c);
        })) 
        {
            break;
        } 
        else 
        {
            cout<<"Invalid name. Use only letters: ";
        }
    }

    if(type == "Doctor" || type == "doctor") 
    {
        cout<<"Enter specialization: ";
        getline(cin, specialization);
        if(specialization.empty()) specialization = "-";
        capacity = 1;       //  Doctor: 1 patient only
    } 
    else if(type == "Nurse" || type == "nurse") 
    {
        specialization = "-";
        capacity = 3;       //  Nurse: 3 patients
    } 
    else 
    {
        specialization = "-";
        capacity = 3;       //  Room: 3 patients
    }

    available = true;
    assignedPatient = 0;
    currentLoad = 0;        // initially no patients assigned

    // Check for duplicate ID in resources.csv
    ifstream ifs("resources.csv");
    string line;
    getline(ifs, line); // skip header
    while (getline(ifs, line)) 
    {
        stringstream ss(line);
        string f[8];      
        for(int i = 0; i < 8 && getline(ss, f[i], ','); ++i);
        if(!f[0].empty() && stoi(f[0]) == id) 
        {
            cout<<"Duplicate ID found. Resource not saved.\n";
            return;
        }
    }
    ifs.close();

    // Append new resource to resources.csv
    ofstream ofs("resources.csv", ios::app);
     ofs<<id<<","<<type<<","<<name<<","<<specialization<<","
        <<(available ? "true" : "false")<<","<<assignedPatient<<","
        <<capacity<<","<<currentLoad<<"\n"; 
    ofs.close();

    cout<<"Resource saved to resources.csv\n";
}


void Resource::DisplayResource() const 
{
    cout<<"ID: "              <<id<<"\n";
    cout<<"Type: "            <<type<<"\n";
    cout<<"Name: "            <<name<<"\n";
    cout<<"Specialization: "  <<specialization<<"\n";
    cout<<"Available: "       <<(available ? "Yes" : "No")<<"\n";
    cout<<"Assigned Patient: "<<assignedPatient<<"\n";
    cout<<"Capacity: "        <<capacity<<"\n";      
    cout<<"Current Load: "    <<currentLoad<<"\n";   
}

// Assign patient to resource
bool Resource::AssignPatient(int patientId) 
{
    if(currentLoad < capacity) 
    {  // ← capacity check
       assignedPatient = patientId;
        currentLoad++;             //  incerease load
        if(currentLoad >= capacity) 
        {
            available = false;     // false if full
        }
        return true;
    }
    cout<<"Resource is at full capacity.\n";
    return false;
}

// Release resource
void Resource::ReleaseResource() 
{
    if(currentLoad > 0) 
    {
        currentLoad--;             //  decrease load
    }
    assignedPatient = 0;
    if(currentLoad < capacity) 
    {
        available = true;          // available if not full
    }
}

// ---------------- ResourceManager Implementation ----------------

// Constructor: start with empty list
ResourceManager::ResourceManager() 
{
    count = 0;
}

// Add a new resource manually
void ResourceManager::AddResource() 
{
    if(count >= 200) 
    {
        cout<<"Cannot add more resources. Limit reached.\n";
        return;
    }

    Resource r;
    r.InputResource();  // collect details and save to resources.csv

    // store in array
    resources[count] = r;
    count++;

    cout<<"Resource added to manager.\n";
}

bool ResourceManager::DeleteResource() 
{
    int id;

    while(true) 
    {
        cout << "Enter resource ID to delete: ";
        if(!(cin >> id)) 
        {
            cout<<"Invalid input. Please enter a valid integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(id == 0) 
        {
            cout<<"Exiting delete operation.\n";
            return false;
        }

        bool found = false;

        // ---Check in runtime array ---
        int idx = -1;
        for(int i = 0; i < count; ++i) 
        {
            if(resources[i].id == id) 
            {
                idx = i;
                break;
            }
        }

        if(idx != -1) 
        {
            // Delete from runtime
            for(int j = idx; j < count - 1; ++j) 
            {
                resources[j] = resources[j + 1];
            }
            count--;
            found = true;
            cout<<"Resource with ID "<<id<<" deleted from runtime.\n";
        }

        // ---  Delete from file ---
        ifstream inFile("resources.csv");
        if(!inFile) 
        {
            cout<<"Error: could not open resources.csv\n";
            return false;
        }

        ofstream tempFile("temp.csv");
        string line;
        bool fileFound = false;

        // Copy header first
        if(getline(inFile, line)) 
        {
            tempFile<<line<<"\n";
        }

        // Copy all lines except the one with matching ID
        while(getline(inFile, line)) 
        {
            stringstream ss(line);
            string idStr;
            if(getline(ss, idStr,',')) 
            {
                try {
                    int fileId = stoi(idStr);
                    if(fileId == id) 
                    {
                        fileFound = true;
                        continue; // skip this line
                    }
                } catch (...) {
                    // ignore malformed line
                }
            }
            tempFile<<line<<"\n";
        }

        inFile.close();
        tempFile.close();

        if(remove("resources.csv") != 0) 
        {
            cout<<"Error: could not remove old resources.csv\n";
            return false;
        }
        if(rename("temp.csv", "resources.csv") != 0) 
        {
            cout<<"Error: could not rename temp.csv to resources.csv\n";
            return false;
        }

        if(fileFound) 
        {
            found = true;
            cout<<"Resource with ID " << id << " deleted from file.\n";
        }

        if(found) 
        {
            cout<<"Deletion completed successfully.\n";
            return true;
        } 
        else 
        {
            cout<<"No resource found with ID "<<id
                <<" in runtime or file. Try again or enter 0 to exit.\n";
            continue;
        }
    }
}

bool ResourceManager::SearchResource() const 
{
    int id;

    while(true) 
    {
        cout<<"Enter resource ID to search: ";
        if(!(cin>>id)) 
        {
            cout<<"Invalid input. Please enter a valid integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(id == 0) 
        {
            cout<<"Exiting search operation.\n";
            return false;
        }

        bool found = false;

        // ---  Search in runtime array ---
        for(int i = 0; i < count; ++i) 
        {
            if(resources[i].id == id) 
            {
                cout<<"Resource found in runtime:\n";
                resources[i].DisplayResource();
                found = true;
                break;
            }
        }

        // ---  Search in file ---
        ifstream inFile("resources.csv");
        if(!inFile.is_open()) 
        {
            cout<<"Error: could not open resources.csv\n";
            return false;
        }

        string line;
        bool fileFound = false;

        getline(inFile, line); // skip header

        while(getline(inFile,line)) 
        {
            stringstream ss(line);
            string idStr, type, name, specialization,
                   available, assignedPatient,
                   capacityStr, currentLoadStr; 

            getline(ss, idStr, ',');
            getline(ss, type, ',');
            getline(ss, name, ',');
            getline(ss, specialization, ',');
            getline(ss, available, ',');
            getline(ss, assignedPatient, ',');
            getline(ss, capacityStr, ',');       
            getline(ss, currentLoadStr, ',');    

            try {
                int fileId = stoi(idStr);
                if(fileId == id) 
                {
                    cout<<"Resource found in file:\n";
                    cout<<"ID: "              <<idStr<<"\n"
                        <<"Type: "            <<type<<"\n"
                        <<"Name: "            <<name<<"\n"
                        <<"Specialization: "  <<specialization<<"\n"
                        <<"Available: "       <<available<<"\n"
                        <<"Assigned Patient: "<<assignedPatient<<"\n"
                        <<"Capacity: "        <<capacityStr<<"\n"      
                        <<"Current Load: "    <<currentLoadStr<<"\n";  
                    fileFound = true;
                    break;
                }
            } catch (...) {
                // ignore malformed line
            }
        }

        inFile.close();

        if(fileFound) found = true;

        if(found) 
        {
            cout<<"Search completed successfully.\n";
            return true;
        } 
        else 
        {
            cout<<"No resource found with ID "<<id
                <<" in runtime or file. Try again or enter 0 to exit.\n";
            continue;
        }
    }
}

void ResourceManager::DisplayAll() 
{
    ifstream inFile("resources.csv");
    if(!inFile.is_open()) 
    {
        cout<<"Error: could not open resources.csv\n";
        return;
    }

    string line;

    cout<<"\n--- All Resources ---\n";

    // Print table headers
    cout<<left<<setw(8)<<"ID"
        <<setw(12)<<"Type"
        <<setw(25)<<"Name"
        <<setw(20)<<"Specialization"
        <<setw(12)<<"Available"
        <<setw(16)<<"AssignedPatient"
        <<setw(10)<<"Capacity"
        <<setw(12) <<"CurrLoad" << "\n"; // ← add karo
    cout<<"-------------------------------------------------------------------------------------\n";

    // Skip header line in file
    getline(inFile,line);

    // Print each record in formatted columns
    while(getline(inFile, line)) 
    {
        stringstream ss(line);
        string idStr, type, name, specialization,
               available, assignedPatient,
               capacityStr, currentLoadStr; 

        getline(ss, idStr, ',');
        getline(ss, type, ',');
        getline(ss, name, ',');
        getline(ss, specialization, ',');
        getline(ss, available, ',');
        getline(ss, assignedPatient, ',');
        getline(ss, capacityStr, ',');       
        getline(ss, currentLoadStr, ',');    

        cout<<left<<setw(8)<<idStr
            <<setw(12)<<type
            <<setw(25)<<name
            <<setw(20)<<specialization
            <<setw(12)<<available
            <<setw(16)<<assignedPatient
            <<setw(10)<<capacityStr      
            <<setw(12)<<currentLoadStr<<"\n"; 
    }

    inFile.close();
    cout<<"-------------------------------------------------------------------------------------\n";
}

bool ResourceManager::IsResourceAvailable(int id) 
{
    //  Check in runtime array ---
    for(int i = 0; i < count; ++i) 
    {
        if(resources[i].id == id) 
        {
            return resources[i].currentLoad < resources[i].capacity;
        }
    }

    //  Check in file ---
    ifstream inFile("resources.csv");
    if(!inFile.is_open()) 
    {
        cout<<"Error: could not open resources.csv\n";
        return false;
    }

    string line;
    getline(inFile, line); // skip header

    while(getline(inFile, line)) 
    {
        stringstream ss(line);
        string idStr, type, name, specialization,
               available, assignedPatient,
               capacityStr, currentLoadStr; 

        getline(ss, idStr, ',');
        getline(ss, type, ',');
        getline(ss, name, ',');
        getline(ss, specialization, ',');
        getline(ss, available, ',');
        getline(ss, assignedPatient, ',');
        getline(ss, capacityStr, ',');       
        getline(ss, currentLoadStr, ',');    

        if(stoi(idStr) == id) 
        {
            inFile.close();
            int cap = stoi(capacityStr);
            int load = stoi(currentLoadStr);
            return (load < cap); // checking capacity vs load
        }
    }

    inFile.close();
    return false;
}

bool ResourceManager::AssignPatientToResource(int resourceId, int patientId) 
{
    bool found = false;

    // ---  Update runtime array ---
    for(int i = 0; i < count; ++i) 
    {
        if(resources[i].id == resourceId) 
        {
            if(resources[i].currentLoad >= resources[i].capacity) 
            {
                cout<<"Resource is at full capacity.\n";
                return false;
            }
            resources[i].currentLoad++;
            resources[i].assignedPatient = patientId;
            if(resources[i].currentLoad >= resources[i].capacity) 
            {
                resources[i].available = false; // full ho gaya
            }
            found = true;
            break;
        }
    }

    // ---  Update file ---
    ifstream inFile("resources.csv");
    ofstream tempFile("temp.csv");
    string line;

    getline(inFile,line); // header
    tempFile<<line<<"\n";

    while(getline(inFile,line)) 
    {
        stringstream ss(line);
        string idStr, type, name, specialization,
               available, assignedPatient,
               capacityStr, currentLoadStr; 

        getline(ss, idStr, ',');
        getline(ss, type, ',');
        getline(ss, name, ',');
        getline(ss, specialization, ',');
        getline(ss, available, ',');
        getline(ss, assignedPatient, ',');
        getline(ss, capacityStr, ',');       
        getline(ss, currentLoadStr, ',');    

        if(stoi(idStr) == resourceId) 
        {
            int load = stoi(currentLoadStr) + 1;
            int cap = stoi(capacityStr);
            currentLoadStr = to_string(load);
            assignedPatient = to_string(patientId);
            if(load >= cap) 
            {
                available = "false"; // full
            }
            found = true;
        }

        tempFile<<idStr<<","<<type<<","<<name<<","
                <<specialization<<","<<available<<","
                <<assignedPatient<<","<<capacityStr<<","
                <<currentLoadStr<<"\n"; 
    }

    inFile.close();
    tempFile.close();
    remove("resources.csv");
    rename("temp.csv", "resources.csv");

    return found;
}

void ResourceManager::ReleaseResource(int resourceId) 
{
    ifstream inFile("resources.csv");
    ofstream tempFile("temp.csv");
    string line;

    if(!inFile) 
    {
        cout<<"Error: could not open resources.csv\n";
        return;
    }

    // Copy header
    if(getline(inFile, line)) 
    {
        tempFile<<line<<"\n";
    }

    bool found = false;

    while(getline(inFile, line)) 
    {
        stringstream ss(line);
        string idStr, type, name, specialization, 
               available, assignedPatient, 
               capacityStr, currentLoadStr;

        getline(ss, idStr, ',');
        getline(ss, type, ',');
        getline(ss, name, ',');
        getline(ss, specialization, ',');
        getline(ss, available, ',');
        getline(ss, assignedPatient, ',');
        getline(ss, capacityStr, ',');       
        getline(ss, currentLoadStr, ',');    

        if(stoi(idStr) == resourceId)  // check for matching ID
        {
            available = "true";
            assignedPatient = "0";
            currentLoadStr = "0";            // reset load
            found = true;
        }

        tempFile<<idStr<<","<<type<<","<<name<<","
                <<specialization<<","<<available<<","
                <<assignedPatient<<","<<capacityStr<<","
                <<currentLoadStr<<"\n";  // write back to temp file
    }

    inFile.close();
    tempFile.close();

    remove("resources.csv");
    rename("temp.csv", "resources.csv");

    if(found)  // feedback to user
    {
        cout<<"Resource with ID "<<resourceId<<" released successfully.\n";
    } 
    else
    {
        cout<<"Resource with ID "<<resourceId<<" not found.\n";
    }
}

bool ResourceManager::SaveToFile(const string& filename) const 
{
    ofstream outFile(filename);
    if(!outFile.is_open()) 
    {
        cout<<"Error: could not open "<<filename<<" for writing.\n";
        return false;
    }

    // Write header
    outFile<<"id,type,name,specialization,available,assignedPatient,capacity,currentLoad\n";

    // Write each resource
    for(int i = 0; i < count; ++i) 
    {
         outFile<<resources[i].id<<","
                <<resources[i].type<<","
                <<resources[i].name<<","
                <<resources[i].specialization<<","
                <<(resources[i].available ? "true" : "false")<<","
                <<resources[i].assignedPatient<<","
                <<resources[i].capacity<<","
                <<resources[i].currentLoad<<"\n";
    }

    outFile.close();
    cout<<"All resources saved successfully to "<<filename<<".\n";
    return true;
}

bool ResourceManager::LoadFromFile(const string& filename) 
{
    ifstream inFile(filename);
    if(!inFile.is_open()) 
    {
        cout<<"Error: could not open "<<filename<<" for reading.\n";
        return false;
    }

    string line;
    count = 0;

    // Skip header
    getline(inFile, line);

    while(getline(inFile,line)) 
    {
        // here skipping blank lines that may be present in file
        if(line.empty() || line == "\r") continue;

        stringstream ss(line);
        string idStr, type, name, specialization,
               availableStr, assignedPatientStr,
               capacityStr, currentLoadStr;

        getline(ss, idStr, ',');
        if(idStr.empty()) continue; // skip if ID is missing

        getline(ss, type, ',');
        getline(ss, name, ',');
        getline(ss, specialization, ',');
        getline(ss, availableStr, ',');
        getline(ss, assignedPatientStr, ',');
        getline(ss, capacityStr, ',');
        getline(ss, currentLoadStr, ',');

        //  here skipping any trailing \r that may be present due to Windows line endings
        if(!currentLoadStr.empty() && currentLoadStr.back() == '\r')
            currentLoadStr.pop_back();

        try {
            Resource r;
            r.id               = idStr.empty()             ? 0     : stoi(idStr);
            r.type             = type;
            r.name             = name;
            r.specialization   = specialization;
            r.available        = (availableStr == "true");
            r.assignedPatient  = assignedPatientStr.empty() ? 0    : stoi(assignedPatientStr);
            r.capacity         = capacityStr.empty()        ? 0    : stoi(capacityStr);
            r.currentLoad      = currentLoadStr.empty()     ? 0    : stoi(currentLoadStr);

            resources[count++] = r;

        } catch (...) {
            // Corrupt line — skip, crash mat karo
            continue;
        }
    }

    inFile.close();
    cout<<count<<" resources loaded from "<<filename<<".\n";
    return true;
}

bool ResourceManager::SearchResourceById(int id) const 
{
    ifstream inFile("resources.csv");
    if(!inFile.is_open()) return false;

    string line;
    getline(inFile, line); // skip header

    while(getline(inFile, line)) 
    {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        try {
            if(stoi(idStr) == id) 
            {
                return true;
            }
        } catch (...) {
            continue;
        }
    }
    return false;
}