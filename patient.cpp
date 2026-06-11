#include"resource.h"
#include"patient.h"
#include<sstream>
#include<iostream>
#include<limits>
#include<ctime>
#include<fstream>
#include<vector>
using namespace std;

// ---------------- Patient methods ----------------
void Patient::PrintSummary() const 
{
    cout<<"Patient ID    : "<<id<<"\n";
    cout<<"Name          : "<<name<<"\n";
    cout<<"MRN           : "<<mrn<<"\n";
    cout<<"Age           : "<<age<<"\n";
    cout<<"Gender        : "<<gender<<"\n";
    cout<<"Emergency     : "<<emergency<<"\n";
    cout<<"Severity      : "<<severity<<"\n";
    cout<<"Diagnosis     : "<<diagnosis<<"\n";
    cout<<"Contact       : "<<contact<<"\n";
    cout<<"Email         : "<<email<<"\n";
    cout<<"Status        : "<<status<<"\n";

    time_t t=(time_t)registeredAt;
    tm* lt=localtime(&t);
    char buf[64];
    strftime(buf,sizeof(buf),"%d-%b-%Y %I:%M %p",lt);
    cout<<"Registered At : "<<buf<<"\n";
    cout<<"-----------------------------------\n";
}

// ---------------- PatientManager methods ----------------
PatientManager::PatientManager()
{
    patientCapacity=INITIAL_CAPACITY;
    patientCount=0;
    patients=new Patient[patientCapacity];
}

PatientManager::~PatientManager()
{
    delete[] patients;
}

void PatientManager::Resize()
{
    int newCapacity=patientCapacity * 2;
    Patient* newArr=new Patient[newCapacity];
    for(int i=0;i<patientCount;i++)
        newArr[i]=patients[i];
    delete[] patients;
    patients=newArr;
    patientCapacity=newCapacity;
}

int PatientManager::GetPatientCount() const { return patientCount; }

int PatientManager::FindIndexById(int id) const
{
    for(int i=0;i<patientCount;i++)
        if (patients[i].id == id) return i;
    return -1;
}

bool PatientManager::IsIdUnique(int id) const
{
    return FindIndexById(id) == -1;
}

// ---------------- Input validation helpers ----------------
bool PatientManager::ValidateName(const string& name) const
{
    if(name.empty() || name.size() > MAX_NAME_LEN) return false;
    for(char c : name)
        if(!(isalpha((unsigned char)c) || c == ' ' || c == '-')) return false;
    return true;
}

bool PatientManager::ValidateMRN(const string& mrn) const
{
    if(mrn.size() > MAX_MRN_LEN) return false;
    for(char c : mrn)
        if(!(isalnum((unsigned char)c) || c == '-' || c == '_')) return false;
    return true;
}

bool PatientManager::ValidateContact(const string& contact) const
{
    if(contact.empty() || contact.size() > MAX_CONTACT_LEN) return false;
    for(char c : contact)
        if(!(isdigit((unsigned char)c) || c == '+' || c == '-' || c == ' ')) return false;
    return true;
}

bool PatientManager::ValidateEmail(const string& email) const
{
    if(email.empty() || email.size() > MAX_EMAIL_LEN) return false;
    size_t at = email.find('@');
    if(at == string::npos) return false;
    size_t dot = email.find('.',at);
    return dot != string::npos;
}

// ---------------- Core operations ----------------
bool PatientManager::AddPatient()
{
    Patient p;

    // Input and validation

    // ID
while(true)
{
    cout<<"Enter patient ID: ";
    if(!(cin>>p.id))
    {
        cout<<"Invalid input. Please enter a valid integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        continue;
    }
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    // File check for uniqueness
    bool foundInFile=false;
    ifstream inFile("patients.csv");
    if(inFile.is_open())
    {
        string line;
        getline(inFile,line); // skip header if present
        while(getline(inFile,line))
        {
            if(!line.empty() && line.back() == '\r')
                line.pop_back();
            if(line.empty()) continue;

            stringstream ss(line);
            string idStr;
            getline(ss,idStr,','); // first column

            if(idStr == to_string(p.id)) 
            {
                foundInFile = true;
                break;
            }
        }
        inFile.close();
    }

    if(foundInFile)
    {
        cout<<"ID "<<p.id<<" already exists in file. Try another.\n";
        continue;
    }
    break; // ID is unique
}

    // Name
    while(true)
    {
        cout<<"Enter patient name: ";
        getline(cin, p.name);
        if(!ValidateName(p.name))
        {
            cout<<"Invalid name. Try again.\n";
            continue;
        }
        break;
    }

    // MRN
    while(true)
    {
        cout<<"Enter MRN: ";
        getline(cin,p.mrn);
        if(!ValidateMRN(p.mrn))
        {
            cout<<"Invalid MRN. Try again.\n";
            continue;
        }
        break;
    }

    //  Age
    while(true)
    {
        cout<<"Enter age: ";
        if(!(cin >> p.age) || p.age < 0 || p.age > 120)
        {
            cout<<"Invalid age. Try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        break;
    }

    // Gender
    while(true)
    {
        cout<<"Enter gender (M/F/O): ";
        cin>>p.gender;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        if(p.gender == 'M' || p.gender == 'F' || p.gender == 'O'||p.gender == 'm' || p.gender == 'f' || p.gender == 'o') break;
        cout << "Invalid gender. Try again.\n";
    }

    // Contact
    while(true)
    {
        cout<<"Enter contact: ";
        getline(cin, p.contact);
        if(!ValidateContact(p.contact))
        {
            cout<<"Invalid contact. Try again.\n";
            continue;
        }
        break;
    }

    // Email
    while(true)
    {
        cout<<"Enter email: ";
        getline(cin, p.email);
        if(!ValidateEmail(p.email))
        {
            cout<<"Invalid email. Try again.\n";
            continue;
        }
        break;
    }

    // Diagnosis
    while(true) 
    {
    cout<<"Enter diagnosis: ";
    getline(cin,p.diagnosis);
    if(p.diagnosis.empty()) 
    {
        cout<<"Diagnosis cannot be empty. Try again.\n";
        continue;
    }
    break;
}


    while(true) 
    {
    cout<<"Enter emergency level (0-100): ";
    if(!(cin >> p.emergency) || p.emergency < 0 || p.emergency > 100) 
    {
        cout<<"Invalid! Enter between 0-100.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        continue;
    }
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    break;
}

    while(true) 
    {
    cout<<"Enter severity (0-10): ";
    if(!(cin >> p.severity) || p.severity < 0 || p.severity > 10) {
        cout<<"Invalid! Enter between 0-10.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        continue;
    }
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    break;
}

    // Set registration time
    p.SetRegisteredNow();

    p.status="Admitted";

    
    // Store patient
    if(patientCount >= patientCapacity) Resize();
    patients[patientCount++] = p;

    cout<<"Patient added successfully!\n";
    p.PrintSummary();

    // Auto-save
    SaveToFile("patients.csv");

    return true;
}
bool PatientManager::IsIdUniqueGlobal(int id) const 
{
    // Check runtime array
    if(FindIndexById(id) != -1) return false;

    // Check file
    ifstream inFile("patients.csv");
    if(!inFile.is_open()) return true; // if file not found, assume unique

    string line;
    getline(inFile, line); // skip header
    while(getline(inFile, line)) 
    {
        stringstream ss(line);
        string idStr;
        if(getline(ss, idStr, ',')) 
        {
            try {
                int fileId = stoi(idStr);
                if(fileId == id) 
                {
                    return false;
                }
            } catch (...) { continue; }
        }
    }
    return true;
}

bool PatientManager::DeletePatient(ResourceManager& rm) 
{
    int id;

    while(true) 
    {
        cout<<"Enter patient ID to delete: ";
        if(!(cin>>id)) 
        {
            cout<<"Invalid input. Please enter a valid integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        if(id == 1) 
        {
            cout<<"Exiting delete operation.\n";
            return false;
        }

        bool found = false;

        //  Find patient in runtime ---
        int idx = FindIndexById(id);
        if(idx != -1) 
        {
            // Free resources only if assigned
            if(patients[idx].doctorId != 0)
                rm.ReleaseResource(patients[idx].doctorId);
            if(patients[idx].nurseId != 0)
                rm.ReleaseResource(patients[idx].nurseId);
            if(patients[idx].roomId != 0)
                rm.ReleaseResource(patients[idx].roomId);

            // Remove patient from array
            for(int i = idx; i < patientCount - 1; i++) 
            {
                patients[i]=patients[i + 1];
            }
            patientCount--;
            found=true;
            cout<<"Patient with ID "<<id<<" deleted and resources freed.\n";
        }

        //    Rewrite patients.csv without this patient ---
        ifstream inFile("patients.csv");
        if(!inFile) 
        {
            cout<<"Error: could not open patients.csv\n";
            return false;
        }

        ofstream tempFile("temp.csv");
        string line;
        bool fileFound = false;

        // Copy header first
        if(getline(inFile,line)) 
        {
            tempFile<<line<<"\n";
        }

        // Copy all lines except the one with matching ID
        while(getline(inFile, line)) 
        {
            stringstream ss(line);
            string idStr;
            if(getline(ss,idStr,',')) 
            {
                try {
                    int fileId = stoi(idStr);
                    if (fileId == id) 
                    {
                        fileFound = true;
                        continue; // skip this line
                    }
                } catch (...) {
                    // ignore malformed line
                }
            }
            tempFile<<line<< "\n";
        }

        inFile.close();
        tempFile.close();

        remove("patients.csv");
        rename("temp.csv","patients.csv");

        if(fileFound) 
        {
            found = true;
            cout<<"Patient with ID "<<id<<" deleted from file.\n";
        }

        if(found) 
        {
            cout<<"Deletion completed successfully.\n";
            return true;
        } 
        else 
        {
            cout<<"No patient found with ID "<<id
                <<" in runtime or file. Try again or enter 1 to exit.\n";
            continue;
        }
    }
}

bool PatientManager::DischargePatient(ResourceManager& rm) 
{
    int id;

    while(true) 
    {
        cout<<"Enter patient ID to discharge: ";
        if(!(cin>>id)) 
        {
            cout<<"Invalid input. Please enter a valid integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        if(id == 1) 
        {
            cout<<"Exiting discharge operation.\n";
            return false;
        }
        break;
    }

    int idx = FindIndexById(id);
    if(idx == -1) {
        cout << "Patient not found.\n";
        return false;
    }

    //  Release resources if assigned
    if(patients[idx].doctorId != 0)
        rm.ReleaseResource(patients[idx].doctorId);
    if(patients[idx].nurseId != 0)
        rm.ReleaseResource(patients[idx].nurseId);
    if(patients[idx].roomId != 0)
        rm.ReleaseResource(patients[idx].roomId);

    //    Update runtime status
    patients[idx].status = "Discharged";

    //   Rewrite patients.csv with updated status
    ifstream inFile("patients.csv");
    ofstream tempFile("temp.csv");
    string line;

    if(!inFile) 
    {
        cout<<"Error: could not open patients.csv\n";
        return false;
    }

    // Copy header
    if(getline(inFile,line)) 
    {
        tempFile<<line<<"\n";
    }

    while(getline(inFile,line)) 
    {
        if(line.empty() || line == "\r") continue;

        stringstream ss(line);
        string idStr, mrn, name, age, gender,
               emergency, severity, diagnosis,
               contact, email, registeredAt,
               doctorId, nurseId, roomId, status; 

        getline(ss, idStr, ',');
        getline(ss, mrn, ',');
        getline(ss, name, ',');
        getline(ss, age, ',');
        getline(ss, gender, ',');
        getline(ss, emergency, ',');
        getline(ss, severity, ',');
        getline(ss, diagnosis, ',');
        getline(ss, contact, ',');
        getline(ss, email, ',');
        getline(ss, registeredAt, ',');
        getline(ss, doctorId, ',');   
        getline(ss, nurseId, ',');    
        getline(ss, roomId, ',');     
        getline(ss, status, ',');

        // Windows \r\n fix
        if(!status.empty() && status.back() == '\r')
            status.pop_back();

        try {
            if(stoi(idStr) == id) 
            {
                status = "Discharged";
            }
        } catch (...) {
            tempFile<<line<<"\n";
            continue;
        }

         tempFile<<idStr<<","<<mrn<<","<<name<< ","
                 <<age<<","<<gender<<","<<emergency<<","
                 <<severity<<","<<diagnosis<<","
                 <<contact<<","<<email<<","
                 <<registeredAt<<","
                 <<doctorId<<","   
                 <<nurseId<<","    
                 <<roomId<<","     
                 <<status<<"\n";
    }

    inFile.close();
    tempFile.close();
    remove("patients.csv");
    rename("temp.csv","patients.csv");

    cout<<"Patient with ID "<<id 
        <<" discharged successfully.\n";
    return true;
}

bool PatientManager::SearchPatient() 
{
    int id;

    while(true) 
    {
        cout<<"Enter patient ID to search: ";
        if(!(cin>>id)) 
        {
            cout<<"Invalid input. Please enter a valid integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(id == 0)   // 0 means exit search
        {
            cout<<"Exiting search operation.\n";
            return false;
        }

        bool found = false;

        //   Search in runtime array 
        int idx = FindIndexById(id);
        if(idx != -1) 
        {
            cout<<"Patient found in runtime:\n";
            patients[idx].PrintSummary();
            found = true;
        }

        //  Search in file 
        ifstream inFile("patients.csv");
        if(!inFile.is_open()) 
        {
            cout<<"Error: could not open patients.csv\n";
            return false;
        }

        string line;
        bool fileFound = false;

        getline(inFile,line); // skip header

        while(getline(inFile, line)) 
        {
            if(line.empty() || line == "\r") continue;

            stringstream ss(line);
            string field;
            Patient p;

            try {
                getline(ss, field, ','); 
                if(field.empty()) continue;
                p.id = stoi(field);

                getline(ss, p.mrn, ',');
                getline(ss, p.name, ',');
                getline(ss, field, ','); 
                p.age = field.empty() ? 0 : stoi(field);
                getline(ss, field, ','); 
                p.gender = field.empty() ? 'O' : field[0];
                getline(ss, field, ','); 
                p.emergency = field.empty() ? 0 : stoi(field);
                getline(ss, field, ','); 
                p.severity = field.empty() ? 0 : stoi(field);
                getline(ss, p.diagnosis, ',');
                getline(ss, p.contact, ',');
                getline(ss, p.email, ',');
                getline(ss, field, ','); 
                p.registeredAt = field.empty() ? 0 : stoll(field);
                getline(ss, field, ','); 
                p.doctorId = field.empty() ? 0 : stoi(field);  
                getline(ss, field, ','); 
                p.nurseId = field.empty() ? 0 : stoi(field);   
                getline(ss, field, ','); 
                p.roomId = field.empty() ? 0 : stoi(field);   
                getline(ss, p.status, ',');

                // Windows \r\n fix
                if(!p.status.empty() && p.status.back() == '\r')
                    p.status.pop_back();

            } catch (...) {
                continue;
            }

            if(p.id == id) {
                cout<<"Patient found in file:\n";
                p.PrintSummary();
                fileFound = true;
                break;
            }
        }

        inFile.close();

        if (fileFound) found = true;

        if(found) 
        {
            cout<<"Search completed successfully.\n";
            return true;
        } 
        else 
        {
            cout<<"No patient found with ID "<<id
                <<" in runtime or file. "
                <<"Try again or enter 0 to exit.\n";
            continue;
        }
    }
}

void PatientManager::DisplayAllPatients() const
{
    // Reload from file before displaying
    PatientManager temp = *this; // make a copy so const is respected
    if(!temp.LoadFromFile("patients.csv"))
    {
        cout<<"Error: could not load patients from file.\n";
        return;
    }

    if(temp.patientCount == 0)
    {
        cout<<"No patients found in records.\n";
        return;
    }

    cout<<"Displaying all patients from file:\n";
    for(int i = 0; i < temp.patientCount; i++)
        temp.patients[i].PrintSummary();

    cout<<"Total patients: "<<temp.patientCount<<"\n";
}

// ---------------- Persistence ----------------
bool PatientManager::SaveToFile(const string& filename) const 
{
    ifstream checkFile(filename);
    bool fileExists = checkFile.good();
    checkFile.close();

    ofstream fout(filename,ios::app);
    if(!fout) return false;

    if(!fileExists) 
    {
        fout<<"ID,MRN,Name,Age,Gender,Emergency,"
            <<"Severity,Diagnosis,Contact,Email,"
            <<"RegisteredAt,DoctorId,NurseId,"
            <<"RoomId,Status\n"; 
    }

    const Patient& p = patients[patientCount - 1];
    fout<<p.id<<","<<p.mrn<<","
        <<p.name<<","<<p.age<<","
        <<p.gender<<","<<p.emergency<<","
        <<p.severity<<","<<p.diagnosis<<","
        <<p.contact<<","<<p.email<<","
        <<p.registeredAt<<","
        <<p.doctorId<<","   
        <<p.nurseId<<","    
        <<p.roomId<<","     
        <<p.status<<"\n";

    return true;
}

bool PatientManager::LoadFromFile(const string& filename)
{
    ifstream fin(filename);
    if(!fin) return false;

    string line;
    patientCount=0;

    // Skip header line
    getline(fin,line);

    while(getline(fin, line))
    {
        // empty line skip karo
        if(line.empty() || line == "\r") continue;

        Patient p;
        string field;
        stringstream ss(line);

        try {
            getline(ss,field,',');
            if(field.empty()) continue;
            p.id = stoi(field);

            getline(ss,p.mrn,',');
            getline(ss,p.name,',');

            getline(ss,field,',');
            p.age = field.empty() ? 0 : stoi(field);

            getline(ss,field,',');
            p.gender = field.empty() ? 'O' : field[0];

            getline(ss,field,',');
            p.emergency = field.empty() ? 0 : stoi(field);

            getline(ss,field,',');
            p.severity = field.empty() ? 0 : stoi(field);

            getline(ss,p.diagnosis,',');
            getline(ss,p.contact,',');
            getline(ss,p.email,',');

            getline(ss,field,',');
            p.registeredAt = field.empty() ? 0 : stoll(field);

            // Doctor, nurse, room IDs
            getline(ss,field,',');
            p.doctorId = field.empty() ? 0 : stoi(field);

            getline(ss,field,',');
            p.nurseId = field.empty() ? 0 : stoi(field);

            getline(ss,field,',');
            p.roomId = field.empty() ? 0 : stoi(field);

            getline(ss,p.status,',');

            // Windows \r\n fix means simply remove trailing \r if present --  status field
            if(!p.status.empty() && p.status.back() == '\r')
                p.status.pop_back();

            if(patientCount >= patientCapacity) Resize();
            patients[patientCount++] = p;

        } catch (...) {
            continue;
        }
    }

    return true;
}

bool PatientManager::SaveAllToFile(const string& filename) const 
{
    ofstream fout(filename);
    if(!fout) return false;

    // Header
    fout<<"ID,MRN,Name,Age,Gender,Emergency,"
        <<"Severity,Diagnosis,Contact,Email,"
        <<"RegisteredAt,DoctorId,NurseId,"
        <<"RoomId,Status\n";

    // all patients save
    for(int i = 0; i < patientCount; i++) 
    {
        const Patient& p = patients[i];
        fout<<p.id<<","<<p.mrn<<","
            <<p.name<<","<<p.age<<","
            <<p.gender<<","<<p.emergency<<","
            <<p.severity<<","<<p.diagnosis<<","
            <<p.contact<<","<<p.email<< ","
            <<p.registeredAt<<","
            <<p.doctorId<<","
            <<p.nurseId<<","
            <<p.roomId<<","
            <<p.status<<"\n";
    }

    fout.close();
    return true;
}