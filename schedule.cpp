#include"schedule.h"
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

// Constructor
Scheduler::Scheduler() {}

// Convert to lowercase
string Scheduler::ToLower(const string& str) 
{
    string result=str;
    for(char& c : result)
        c=tolower((unsigned char)c);
    return result;
}

//   Diagnosis to specialization mapping
string Scheduler::GetSpecialization(const string& diagnosis) 
{
    string d=ToLower(diagnosis);

    if(d=="heart attack" || d=="chest pain" ||
       d=="cardiac arrest")
        return "Cardiologist";

    if(d=="brain tumor" || d=="epilepsy"  ||
       d=="stroke"      || d=="migraine")
        return "Neurologist";

    if(d=="fracture"   || d=="bone pain" ||
       d=="joint pain" || d=="arthritis")
        return "Orthopedic";

    if(d=="fever" || d=="flu"   ||
       d=="cold"  || d=="cough" ||
       d=="typhoid")
        return "General Physician";

    if(d=="eye problem" || d=="vision loss" ||
       d=="eye infection")
        return "Ophthalmologist";

    if(d=="skin rash" || d=="acne" ||
       d=="eczema" || d=="psoriasis")
        return "Dermatologist";

    if(d=="child fever" || d=="child flu" ||
       d=="child infection")
        return "Pediatrician";

    if(d=="pregnancy" || d=="gynecology" ||
       d=="maternity")
        return "Gynecologist";

    if(d=="cancer" || d=="tumor" ||
       d=="lymphoma")
        return "Oncologist";

    if(d=="asthma" || d=="breathing problem" ||
       d=="pneumonia")
        return "Pulmonologist";

    if(d=="kidney failure" || d=="kidney stone" ||
       d=="kidney infection")
        return "Nephrologist";

    if(d=="stomach pain" || d=="liver problem" ||
       d=="hepatitis" || d=="ulcer")
        return "Gastroenterologist";

    if(d=="diabetes" || d=="thyroid" ||
       d=="hormone problem")
        return "Endocrinologist";

    if(d=="tooth pain" || d=="dental" ||
       d=="gum problem")
        return "Dentist";

    if(d=="ear problem" || d=="nose problem" ||
       d=="throat problem" || d=="tonsils")
        return "ENT Specialist";

    if(d=="depression" || d=="anxiety" ||
       d=="mental health" || d=="schizophrenia")
        return "Psychiatrist";

    if(d=="urinary problem" || d=="bladder" ||
       d=="prostate")
        return "Urologist";

    if(d=="blood disorder" || d=="anemia" ||
       d=="leukemia")
        return "Hematologist";

    if(d=="allergy" || d=="allergic reaction")
        return "Allergist";

    if(d=="muscle pain" || d=="rheumatism")
        return "Rheumatologist";

    if(d=="surgery" || d=="appendix" ||
       d=="hernia")
        return "General Surgeon";

    if(d=="spine problem" || d=="back pain" ||
        d=="slip disc")
        return "Spine Specialist";

    if(d=="foot pain" || d=="ankle problem")
        return "Podiatrist";

    if(d=="nutrition" || d=="diet problem" ||
       d=="obesity")
        return "Nutritionist";

    if(d=="heart surgery" || d=="chest surgery")
        return "Cardiothoracic Surgeon";

    if(d== "brain surgery")
        return "Neurosurgeon";

    if(d=="physiotherapy" || d=="rehabilitation")
        return "Physiotherapist";

    if(d=="xray" || d=="scan" || d=="mri")
        return "Radiologist";

    if(d=="emergency" || d=="trauma" ||
        d=="accident")
        return "Emergency Physician";

    // Default
    return "General Physician";
}

bool Scheduler::AssignResources(Patient& p,ResourceManager& rm) 
{
    string spec=GetSpecialization(p.diagnosis);
    bool doctorAssigned=false;
    bool nurseAssigned=false;
    bool roomAssigned=false;

    //  Find matching available doctor
    for(int i=0;i<rm.count;i++) 
    {
         if(rm.resources[i].type=="Doctor" &&
            rm.resources[i].specialization==spec &&
            rm.resources[i].currentLoad < rm.resources[i].capacity) 
            {
            p.doctorId=rm.resources[i].id;
            rm.AssignPatientToResource(rm.resources[i].id,p.id);
            doctorAssigned=true;
            cout<<"Doctor assigned: "
                <<rm.resources[i].name<<"\n";
            break;
        }
    }

    if(!doctorAssigned) 
    {
        cout<<"No doctor available for: "<<spec<<"\n";
        return false;
    }

    //  Find available nurse
    for(int i=0;i<rm.count;i++) 
    {
         if(rm.resources[i].type=="Nurse" &&
            rm.resources[i].currentLoad<rm.resources[i].capacity) 
            {
            p.nurseId=rm.resources[i].id;
            rm.AssignPatientToResource(rm.resources[i].id,p.id);
            nurseAssigned=true;
            cout<<"Nurse assigned: "
                <<rm.resources[i].name<<"\n";
            break;
        }
    }

    if(!nurseAssigned) 
    {
        cout<<"No nurse available!\n";
        return false;
    }

    //  Find available room
    for(int i=0;i<rm.count;i++) 
    {
         if(rm.resources[i].type=="Room" &&
            rm.resources[i].currentLoad<rm.resources[i].capacity) 
            {
            p.roomId=rm.resources[i].id;
            rm.AssignPatientToResource(rm.resources[i].id,p.id);
            roomAssigned=true;
            cout<<"Room assigned: "
                <<rm.resources[i].name<<"\n";
            break;
        }
    }

    if (!roomAssigned) 
    {
        cout<<"No room available!\n";
        return false;
    }

    return true;
}
// Save schedule to CSV
bool Scheduler::SaveSchedule(Patient* patients,int count,ResourceManager& rm) 
{
    ofstream fout("schedule.csv");
    if(!fout) return false;

    // Header
    fout<<"PatientId,PatientName,DoctorId,"
        <<"NurseId,RoomId,"
        <<"TimeSlot,AppointmentTime,Status\n";

    for(int i=0;i<count;i++) 
    {
       if(patients[i].status != "Admitted") continue;
       if(patients[i].assignedColor == -1) continue;

        fout<<patients[i].id<<","
            <<patients[i].name<<","
            <<patients[i].doctorId<<","
            <<patients[i].nurseId<<","
            <<patients[i].roomId<<","
            <<patients[i].assignedColor<<","
            <<coloring.SlotToTime(patients[i].assignedColor)<<","
            <<patients[i].status<<"\n";
    }

    fout.close();
    cout<<"Schedule saved to schedule.csv\n";
    return true;
}

// Display schedule on screen
void Scheduler::DisplaySchedule(Patient* patients,int count,ResourceManager& rm) 
{
    cout<<"\n========= HOSPITAL SCHEDULE =========\n";

    for(int i=0;i<count;i++) 
    {
        if(patients[i].status != "Admitted") continue;
        if(patients[i].assignedColor == -1) continue;

        // Get doctor name
        string doctorName = "N/A";
        string nurseName  = "N/A";
        string roomName   = "N/A";

        for(int j=0;j<rm.count;j++) 
        {
           if(rm.resources[j].id == patients[i].doctorId)
                doctorName=rm.resources[j].name;
           if(rm.resources[j].id == patients[i].nurseId)
                nurseName=rm.resources[j].name;
           if(rm.resources[j].id == patients[i].roomId)
                roomName=rm.resources[j].name;
        }

        cout<<"------------------------------------\n";
        cout<<"Patient  : "<< patients[i].name 
            <<" (ID: "<<patients[i].id<<")\n";
        cout<<"Diagnosis: "<<patients[i].diagnosis<<"\n";
        cout<<"Doctor   : "<<doctorName<<"\n";
        cout<<"Nurse    : "<<nurseName<<"\n";
        cout<<"Room     : "<<roomName<<"\n";
        cout<<"Time     : " 
            <<coloring.SlotToTime(patients[i].assignedColor)<<"\n";
        cout<<"Priority : " 
            <<patients[i].emergency + patients[i].severity<<"\n";
    }
    cout<<"=====================================\n";
}

void Scheduler::RunSchedule(PatientManager& pm,ResourceManager& rm) 
{
    cout<<"\n--- Running Hospital Scheduler ---\n";

    //  Load patients
    pm.LoadFromFile("patients.csv");
    if(pm.GetPatientCount() == 0) 
    {
        cout<<"No patients found!\n";
        return;
    }

    //  Load resources
    rm.LoadFromFile("resources.csv");

    //  Assign resources to each patient
    cout<<"\nAssigning resources...\n";
    for(int i=0;i<pm.GetPatientCount();i++) 
    {
        if(pm.patients[i].status == "Admitted" &&pm.patients[i].doctorId == 0) 
        {
          cout<<"\nPatient: "<<pm.patients[i].name<<"\n";

          AssignResources(pm.patients[i],rm);
        }
    }

    
    pm.SaveAllToFile("patients.csv"); // save any resource assignments back to file

    //  Build conflict graph
    cout<<"\nBuilding conflict graph...\n";
    graph.BuildGraph(pm.patients,pm.GetPatientCount());

    //  Build greedy order
    cout<<"\nBuilding greedy order...\n";
    greedy.BuildOrder(pm.patients,pm.GetPatientCount(),graph);

    //  Assign time slots
    cout<<"\nAssigning time slots...\n";
    coloring.AssignColors(greedy, graph,pm.patients,pm.GetPatientCount());

    // Display schedule
    DisplaySchedule(pm.patients,pm.GetPatientCount(),rm);

    //  Save schedule
    SaveSchedule(pm.patients,pm.GetPatientCount(),rm);

    cout<<"\n--- Scheduling Complete! ---\n";
}