#include"output.h"
#include<iostream>
#include<fstream>
#include<ctime>
using namespace std;

//  Constructor
ReportGenerator::ReportGenerator() {}

// Get resource name by ID
string ReportGenerator::GetResourceName(int id,ResourceManager& rm) 
{
    for(int i=0;i<rm.count;i++) 
    {
        if(rm.resources[i].id==id)
          return rm.resources[i].name;
    }
    return "N/A";
}

// Get resource specialization by ID
string ReportGenerator::GetResourceSpec(int id,ResourceManager& rm) 
{
    for(int i=0;i<rm.count;i++) 
    {
        if(rm.resources[i].id==id)
            return rm.resources[i].specialization;
    }
    return "N/A";
}

// Format epoch to readable time
string ReportGenerator::FormatTime(long long epoch) 
{
    time_t t=(time_t)epoch;
    tm* lt=localtime(&t);
    char buf[64];
    strftime(buf,sizeof(buf),"%d-%b-%Y %I:%M %p",lt);
    return string(buf);
}

// Print report on console
void ReportGenerator::PrintReportConsole(
    Patient* patients,int count,
    ResourceManager& rm,
    GraphColoring& coloring) 
    {

    cout<<"\n";
    cout<<"╔═════════════════════════════════════╗\n";
    cout<<"║     HOSPITAL SCHEDULE REPORT        ║\n";
    cout<<"╚═════════════════════════════════════╝\n";

    int reportCount=0;

    for(int i=0;i<count;i++) 
    {
        if(patients[i].status!="Admitted") continue;
        if(patients[i].assignedColor== -1) continue;

        reportCount++;

        // Patient info
        cout<<"\n";
        cout<<"=====================================\n";
        cout<<"         PATIENT REPORT #"<<reportCount<<"\n";
        cout<<"=====================================\n";
        cout<<"Patient Name  : "<<patients[i].name<<"\n";
        cout<<"Patient ID    : "<<patients[i].id<<"\n";
        cout<<"MRN           : "<<patients[i].mrn<<"\n";
        cout<<"Age           : "<<patients[i].age<<"\n";
        cout<<"Gender        : ";

        if (patients[i].gender == 'M'|| patients[i].gender == 'm')
            cout<<"Male\n";
        else if(patients[i].gender == 'F' || patients[i].gender == 'f')
            cout<<"Female\n";
        else
            cout<<"Other\n";
        cout<<"Diagnosis     : "<<patients[i].diagnosis<<"\n";
        cout<<"Emergency     : "<<patients[i].emergency<<"\n";
        cout<<"Severity      : "<<patients[i].severity<<"\n";
        cout<<"Status        : "<<patients[i].status<<"\n";
        cout<<"Registered At : "<<FormatTime(patients[i].registeredAt)<<"\n";

        // Resources
        cout<<"-------------------------------------\n";
        cout<<"       ASSIGNED RESOURCES\n";
        cout<<"-------------------------------------\n";
        cout<<" Doctor : " 
            <<GetResourceName(patients[i].doctorId,rm)
            <<" ( " 
            <<GetResourceSpec(patients[i].doctorId,rm)
            <<")\n";
        cout<<"Nurse  : " 
            <<GetResourceName(patients[i].nurseId,rm) 
            <<"\n";
        cout<<"Room   : " 
            <<GetResourceName(patients[i].roomId,rm) 
            <<"\n";

        // Schedule info
        cout<<"-------------------------------------\n";
        cout<<"         SCHEDULE INFO\n";
        cout<<"-------------------------------------\n";
        cout<<"Time Slot     : "<<patients[i].assignedColor<<"\n";
        cout<<"Appointment   : "<<coloring.SlotToTime(patients[i].assignedColor)<<"\n";
        cout<<"Priority Score: " 
            <<patients[i].emergency + patients[i].severity<<"\n";

        cout<<"-------------------------------------\n";
        cout<<"     ✅ SCHEDULE CONFLICT FREE!\n";
        cout<<"=====================================\n";
    }

    if(reportCount==0) 
    {
        cout<<"No scheduled patients found!\n";
    } 
    else
    {
        cout<<"\nTotal Patients Scheduled: "<<reportCount<<"\n";
    }
}

// Generate HTML report
void ReportGenerator::GenerateHTMLReport(Patient* patients,int count,
ResourceManager& rm,GraphColoring& coloring) 
    {

    ofstream fout("report.html");
    if (!fout) 
    {
        cout<<"Error: could not create report.html\n";
        return;
    }

    // HTML header
    fout<<"<!DOCTYPE html>\n";
    fout<<"<html lang='en'>\n";
    fout<<"<head>\n";
    fout<<"<meta charset='UTF-8'>\n";
    fout<<"<title>Hospital Schedule Report</title>\n";
    fout<<"<style>\n";
    fout<<"body { font-family: Arial, sans-serif; "
        <<"background: #f0f4f8; margin: 20px; }\n";
    fout<<"h1 { text-align: center; color: #2c3e50; "
        <<"background: #3498db; color: white; "
        <<"padding: 20px; border-radius: 10px; }\n";
    fout<<".patient-card { background: white; "
        <<"border-radius: 10px; padding: 20px; "
        <<"margin: 20px 0; "
        <<"box-shadow: 0 2px 8px rgba(0,0,0,0.1); }\n";
    fout<<".section-title { background: #2c3e50; "
        <<"color: white; padding: 8px 15px; "
        <<"border-radius: 5px; margin: 10px 0; }\n";
    fout<<".info-row { display: flex; "
        <<"padding: 5px 0; "
        <<"border-bottom: 1px solid #eee; }\n";
    fout<<".label { font-weight: bold; "
        <<"width: 180px; color: #555; }\n";
    fout<<".value { color: #2c3e50; }\n";
    fout<<".status-ok { background: #27ae60; "
        <<"color: white; padding: 8px 15px; "
        <<"border-radius: 5px; "
        <<"text-align: center; margin-top: 10px; }\n";
    fout<<".priority-high { color: #e74c3c; "
        <<"font-weight: bold; }\n";
    fout<<".priority-mid { color: #f39c12; "
        <<"font-weight: bold; }\n";
    fout<<".priority-low { color: #27ae60; "
        <<"font-weight: bold; }\n";
    fout<<"</style>\n";
    fout<<"</head>\n";
    fout<<"<body>\n";
    fout<<"<h1>🏥 Hospital Schedule Report</h1>\n";

    int reportCount=0;

    for(int i=0;i<count;i++) 
    {
        if(patients[i].status!="Admitted") continue;
        if(patients[i].assignedColor== -1) continue;

        reportCount++;

        int priority=patients[i].emergency + patients[i].severity;
        string priorityClass;
        if(priority>=80)      priorityClass="priority-high";

        else if(priority>=50) priorityClass="priority-mid";

        else                     priorityClass="priority-low";

        string gender;
        if(patients[i].gender=='M' || patients[i].gender=='m')
            gender="Male";

        else if(patients[i].gender=='F' || patients[i].gender=='f')
            gender="Female";

        else gender="Other";

        fout<<"<div class='patient-card'>\n";

        // Patient info section
        fout<<"<div class='section-title'>"
            <<"👤 Patient Report #"<<reportCount 
            <<"</div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Patient Name</span>"
            <<"<span class='value'>" 
            <<patients[i].name<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Patient ID</span>"
            <<"<span class='value'>" 
            <<patients[i].id<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>MRN</span>"
            <<"<span class='value'>" 
            <<patients[i].mrn<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Age</span>"
            <<"<span class='value'>" 
            <<patients[i].age<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Gender</span>"
            <<"<span class='value'>" 
            <<gender<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Diagnosis</span>"
            <<"<span class='value'>" 
            <<patients[i].diagnosis<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Emergency</span>"
            <<"<span class='value "<<priorityClass<<"'>" 
            <<patients[i].emergency<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Severity</span>"
            <<"<span class='value "<<priorityClass<<"'>" 
            <<patients[i].severity<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Status</span>"
            <<"<span class='value'>" 
            <<patients[i].status<<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Registered At</span>"
            <<"<span class='value'>" 
            <<FormatTime(patients[i].registeredAt) 
            <<"</span></div>\n";

        // Resources section
        fout<<"<div class='section-title'>"
            <<"🏥 Assigned Resources</div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Doctor</span>"
            <<"<span class='value'>" 
            <<GetResourceName(patients[i].doctorId,rm)
            <<" (" 
            <<GetResourceSpec(patients[i].doctorId,rm)
            <<")</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Nurse</span>"
            <<"<span class='value'>" 
            <<GetResourceName(patients[i].nurseId,rm)
            <<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Room</span>"
            <<"<span class='value'>" 
            <<GetResourceName(patients[i].roomId,rm)
            <<"</span></div>\n";

        // Schedule section
        fout<<"<div class='section-title'>"
            <<"📅 Schedule Info</div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Time Slot</span>"
            <<"<span class='value'>" 
            <<patients[i].assignedColor 
            <<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Appointment</span>"
            <<"<span class='value'>" 
            <<coloring.SlotToTime(patients[i].assignedColor)
            <<"</span></div>\n";

        fout<<"<div class='info-row'>"
            <<"<span class='label'>Priority Score</span>"
            <<"<span class='value "<<priorityClass<<"'>" 
            <<priority<<"</span></div>\n";

        fout<<"<div class='status-ok'>"
            <<"✅ SCHEDULE CONFLICT FREE!"
            <<"</div>\n";

        fout<<"</div>\n"; // end patient-card
    }

    if(reportCount==0) 
    {
        fout<<"<p style='text-align:center;"
            <<"color:red;'>No scheduled patients!</p>\n";
    } 
    else 
    {
        fout<<"<div style='text-align:center;"
            <<"background:#2c3e50;color:white;"
            <<"padding:15px;border-radius:10px;"
            <<"margin-top:20px;'>"
            <<"Total Patients Scheduled: " 
            <<reportCount<<"</div>\n";
    }

    fout<<"</body>\n</html>\n";
    fout.close();

    cout<<"HTML report saved to report.html\n";
}

// Generate both reports
void ReportGenerator::GenerateReport(Patient* patients, int count,ResourceManager& rm,
GraphColoring& coloring) 
    {

    //  Console report
    PrintReportConsole(patients,count,rm,coloring);

    //  HTML report
    GenerateHTMLReport(patients,count,rm,coloring);

    //   Ask user to open HTML
    cout<<"\nOpen HTML report in browser? (Y/N): ";
    char choice;
    cin>>choice;
    cin.ignore(1000, '\n');

    if(choice=='Y' || choice=='y') 
    {
        system("start report.html"); // Windows
    }
}