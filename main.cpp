#include<iostream>
#include<fstream>
#include"patient.h"
#include"resource.h"
#include"schedule.h"
#include"output.h"
#include"complexity.h"
using namespace std;

// ============================================
//           UTILITY FUNCTIONS
// ============================================

void ClearScreen()
{
    system("cls");
}

void PressEnterToContinue()
{
    // removed
}

void PrintDivider(char c,int width)
{
    for(int i=0;i<width;i++) 
    cout<<c;
    cout<<"\n";
}

void ShowHeader(const string& title)
{
    cout<<"\n";
    PrintDivider('=',60);
    cout<<" HOSPITAL SCHEDULING SYSTEM\n";
    PrintDivider('=',60);
    cout<<">>"<<title<<"\n";
    PrintDivider('-',60);
    cout<<"\n";
}

// ============================================
//           CHECK FUNCTIONS
// ============================================

bool CheckPatientsExist(PatientManager& pm)
{
    if(pm.patientCount==0)
    {
        cout<<" [!] No patients found!\n";
        cout<<" Please add patients first.\n";
        return false;
    }
    return true;
}

bool CheckScheduleRan(PatientManager& pm)
{
    if (pm.patientCount==0)
    {
        cout<<" [!] No patients found!\n";
        cout<<" Please add patients first.\n";
        return false;
    }
    for(int i=0;i<pm.patientCount;i++)
    {
        if(pm.patients[i].assignedColor!=-1)
        {
            return true;
        }
    }
    cout<<" [!] Schedule not run yet!\n";
    cout<<" Please run schedule first\n";
    cout<<" from Schedule Management menu.\n";
    return false;
}

bool CheckResourcesExist(ResourceManager& rm)
{
    if (rm.count==0)
    {
        cout<<" [!] No resources found!\n";
        cout<<" Please add resources first.\n";
        return false;
    }
    return true;
}

// ============================================
//        PATIENT MANAGEMENT MENU
// ============================================

void PatientMenu(PatientManager& pm,ResourceManager& rm)
{
    int choice;
    do
    {
        ShowHeader("PATIENT MANAGEMENT");

        cout<<"  +------------------------------------+\n";
        cout<<"  |       PATIENT OPERATIONS           |\n";
        cout<<"  +------------------------------------+\n";
        cout<<"  |                                    |\n";
        cout<<"  |   [1]  Add New Patient             |\n";
        cout<<"  |   [2]  Search Patient              |\n";
        cout<<"  |   [3]  Delete Patient              |\n";
        cout<<"  |   [4]  Discharge Patient           |\n";
        cout<<"  |   [5]  Display All Patients        |\n";
        cout<<"  |   [6]  Clear Screen                |\n";
        cout<<"  |   [0]  Back to Main Menu           |\n";
        cout<<"  |                                    |\n";
        cout<<"  +------------------------------------+\n";
        cout<<"\n  Enter your choice: ";

        if(!(cin>>choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout<<"\n [!] Invalid input! Try again.\n";
            continue;
        }
        cin.ignore(1000,'\n');

        switch(choice)
        {
            case 1:
                ShowHeader("ADD NEW PATIENT");
                if(!CheckResourcesExist(rm))
                {
                    cout<<"\n [!] Cannot add patient without resources!\n";
                    break;
                }
                pm.AddPatient();
                break;

            case 2:
                ShowHeader("SEARCH PATIENT");
                if (!CheckPatientsExist(pm))
                {
                    break;
                }
                pm.SearchPatient();
                break;

            case 3:
                ShowHeader("DELETE PATIENT");
                if (!CheckPatientsExist(pm))
                {
                    break;
                }
                cout<<" [!] WARNING: This will permanently\n";
                cout<<" delete the patient record!\n\n";
                pm.DeletePatient(rm);
                break;

            case 4:
                ShowHeader("DISCHARGE PATIENT");
                if(!CheckPatientsExist(pm))
                {
                    break;
                }
                pm.DischargePatient(rm);
                break;

            case 5:
                ShowHeader("ALL PATIENTS");
                if (!CheckPatientsExist(pm))
                {
                    break;
                }
                pm.DisplayAllPatients();
                break;

            case 6:
                ClearScreen();
                break;

            case 0:
                cout<<"\n Returning to Main Menu...\n";
                break;

            default:
                cout<<"\n [!] Invalid choice! Try again.\n";
        }
    }while(choice!=0);
}

// ============================================
//        RESOURCE MANAGEMENT MENU
// ============================================

void ResourceMenu(ResourceManager& rm)
{
    int choice;
    do
    {
        ShowHeader("RESOURCE MANAGEMENT");

        cout<<"  +------------------------------------+\n";
        cout<<"  |       RESOURCE OPERATIONS          |\n";
        cout<<"  +------------------------------------+\n";
        cout<<"  |                                    |\n";
        cout<<"  |   [1]  Add New Resource            |\n";
        cout<<"  |   [2]  Search Resource             |\n";
        cout<<"  |   [3]  Delete Resource             |\n";
        cout<<"  |   [4]  Display All Resources       |\n";
        cout<<"  |   [5]  Clear Screen                |\n";
        cout<<"  |   [0]  Back to Main Menu           |\n";
        cout<<"  |                                    |\n";
        cout<<"  +------------------------------------+\n";
        cout<<"\n  Enter your choice: ";

        if(!(cin>>choice))
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"\n  [!] Invalid input! Try again.\n";
            continue;
        }
        cin.ignore(1000,'\n');

        switch(choice)
        {
            case 1:
                ShowHeader("ADD NEW RESOURCE");
                rm.AddResource();
                break;

            case 2:
                ShowHeader("SEARCH RESOURCE");
                if (!CheckResourcesExist(rm))
                {
                    break;
                }
                rm.SearchResource();
                break;

            case 3:
                ShowHeader("DELETE RESOURCE");
                if(!CheckResourcesExist(rm))
                {
                    break;
                }
                cout<<" [!] WARNING: This will permanently\n";
                cout<<" delete the resource record!\n\n";
                rm.DeleteResource();
                break;

            case 4:
                ShowHeader("ALL RESOURCES");
                if(!CheckResourcesExist(rm))
                {
                    break;
                }
                rm.DisplayAll();
                break;

            case 5:
                ClearScreen();
                break;

            case 0:
                cout<<"\n Returning to Main Menu...\n";
                break;

            default:
                cout<<"\n [!] Invalid choice! Try again.\n";
        }
    }while(choice!=0);
}

// ============================================
//        SCHEDULE MANAGEMENT MENU
// ============================================

void ScheduleMenu(PatientManager& pm,ResourceManager& rm,Scheduler& scheduler)
{
    int choice;
    do
    {
        ShowHeader("SCHEDULE MANAGEMENT");

        cout<<"  +------------------------------------+\n";
        cout<<"  |       SCHEDULE OPERATIONS          |\n";
        cout<<"  +------------------------------------+\n";
        cout<<"  |                                    |\n";
        cout<<"  |   [1]  Run Schedule                |\n";
        cout<<"  |   [2]  View Current Schedule       |\n";
        cout<<"  |   [3]  Clear Screen                |\n";
        cout<<"  |   [0]  Back to Main Menu           |\n";
        cout<<"  |                                    |\n";
        cout<<"  +------------------------------------+\n";
        cout<<"\n  Enter your choice: ";

        if(!(cin>>choice))
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"\n [!] Invalid input! Try again.\n";
            continue;
        }
        cin.ignore(1000,'\n');

        switch(choice)
        {
            case 1:
                ShowHeader("RUNNING SCHEDULE");
                if (!CheckPatientsExist(pm))
                {
                    break;
                }
                if (!CheckResourcesExist(rm))
                {
                    break;
                }
                cout<<"  [*] Please wait...\n\n";
                scheduler.RunSchedule(pm, rm);
                cout<<"\n  [OK] Schedule completed!\n";
                break;

            case 2:
                ShowHeader("CURRENT SCHEDULE");
                if(!CheckScheduleRan(pm))
                {
                    break;
                }
                scheduler.DisplaySchedule(pm.patients,pm.patientCount,rm);
                break;

            case 3:
                ClearScreen();
                break;

            case 0:
                cout<<"\n  Returning to Main Menu...\n";
                break;

            default:
                cout<<"\n  [!] Invalid choice! Try again.\n";
        }
    }while(choice!=0);
}

// ============================================
//        REPORT MENU
// ============================================

void ReportMenu(PatientManager& pm,ResourceManager& rm,Scheduler& scheduler,ReportGenerator& report)
{
    int choice;
    do
    {
        ShowHeader("REPORTS");

        cout<<"  +------------------------------------+\n";
        cout<<"  |          REPORT OPTIONS            |\n";
        cout<<"  +------------------------------------+\n";
        cout<<"  |                                    |\n";
        cout<<"  |   [1]  Generate Full Report        |\n";
        cout<<"  |   [2]  Open HTML Report            |\n";
        cout<<"  |   [3]  Clear Screen                |\n";
        cout<<"  |   [0]  Back to Main Menu           |\n";
        cout<<"  |                                    |\n";
        cout<<"  +------------------------------------+\n";
        cout<<"\n  Enter your choice: ";

        if (!(cin>>choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout<<"\n  [!] Invalid input! Try again.\n";
            continue;
        }
        cin.ignore(1000,'\n');

        switch (choice)
        {
            case 1:
                ShowHeader("GENERATING REPORT");
                if (!CheckScheduleRan(pm))
                {
                    break;
                }
                cout<<"  [*] Generating report...\n\n";
                report.GenerateReport(pm.patients,pm.patientCount,rm,scheduler.coloring);
                break;

            case 2:
                ShowHeader("OPENING HTML REPORT");
                {
                    ifstream checkFile("report.html");
                    if (!checkFile.is_open())
                    {
                        cout<<"  [!] No report found!\n";
                        cout<<"  Please generate report first.\n";
                        break;
                    }
                    checkFile.close();
                }
                cout<<" [*] Opening report.html...\n";
                system("start report.html");
                break;

            case 3:
                ClearScreen();
                break;

            case 0:
                cout<<"\n  Returning to Main Menu...\n";
                break;

            default:
                cout<<"\n [!] Invalid choice! Try again.\n";
        }
    }while(choice!=0);
}

// ============================================
//           MAIN MENU DISPLAY
// ============================================

void ShowMainMenu()
{
    cout<<"\n\n";
    cout<<"++====================================================================++\n";
    cout<<"||                                                                    ||\n";
    cout<<"||        WELCOME TO HOSPITAL RESOURCE SCHEDULING SYSTEM             ||\n";
    cout<<"||                                                                    ||\n";
    cout<<"||====================================================================||\n";
    cout<<"||     Kindly select the appropriate option to continue:              ||\n";
    cout<<"++====================================================================++\n";
    cout<<"||                                                                    ||\n";
    cout<<"||   [1] -->> Patient Management                                      ||\n";
    cout<<"||   [2] -->> Resource Management                                     ||\n";
    cout<<"||   [3] -->> Schedule Management                                     ||\n";
    cout<<"||   [4] -->> Reports                                                 ||\n";
    cout<<"||   [5] -->> Complexity Analysis                                     ||\n";
    cout<<"||   [6] -->> Clear Screen                                            ||\n";
    cout<<"||   [0] -->> Exit Application                                        ||\n";
    cout<<"||                                                                    ||\n";
    cout<<"++====================================================================++\n";
    cout<<"|| Enter your choice: ";
}

// ============================================
//              MAIN FUNCTION
// ============================================

int main()
{
    PatientManager pm;
    ResourceManager rm;
    Scheduler scheduler;
    ReportGenerator report;
    ComplexityAnalyzer complexity;

    // Safe file loading
    ifstream pFile("patients.csv");
    if(pFile.is_open())
    {
        pFile.seekg(0,ios::end);
        if(pFile.tellg()>0)
        {
            pFile.close();
            pm.LoadFromFile("patients.csv");
        }
        else
        {
            pFile.close();
        }
    }

    ifstream rFile("resources.csv");
    if(rFile.is_open())
    {
        rFile.seekg(0, ios::end);
        if (rFile.tellg()>0)
        {
            rFile.close();
            rm.LoadFromFile("resources.csv");
        }
        else
        {
            rFile.close();
        }
    }

    int choice;
    do
    {
        ShowMainMenu();

        if (!(cin>>choice))
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"\n [!] Invalid input! Try again.\n";
            continue;
        }
        cin.ignore(1000,'\n');

        switch(choice)
        {
            case 1:
                PatientMenu(pm,rm);
                break;

            case 2:
                ResourceMenu(rm);
                break;

            case 3:
                ScheduleMenu(pm,rm,scheduler);
                break;

            case 4:
                ReportMenu(pm,rm,scheduler,report);
                break;

            case 5:
                ShowHeader("COMPLEXITY ANALYSIS");
                if (!CheckPatientsExist(pm))
                {
                    cout<<" Run schedule first for accurate analysis!\n";
                    break;
                }
                complexity.RunAnalysis(pm,rm,scheduler.graph,scheduler.greedy,scheduler.coloring);
                break;

            case 6:
                ClearScreen();
                break;

            case 0:
                ClearScreen();
                cout<<"\n";
                PrintDivider('=',70);
                cout<<"||                                                                    ||\n";
                cout<<"||   *** THANK YOU FOR USING HOSPITAL RESOURCE SCHEDULING SYSTEM ***  ||\n";
                cout<<"||                                                                    ||\n";
                cout<<"||   << Your session has ended successfully >>                        ||\n";
                cout<<"||                                                                    ||\n";
                cout<<"||   ## We appreciate your commitment to efficient healthcare ##      ||\n";
                cout<<"||                                                                    ||\n";
                cout<<"||   -- Goodbye! Have a wonderful day ahead! --                       ||\n";
                cout<<"||                                                                    ||\n";
                PrintDivider('=',70);
                cout<<"\n";
                break;

            default:
                cout<<"\n  [!] Invalid choice! Try again.\n";
        }
    }while(choice!=0);

    return 0;
}