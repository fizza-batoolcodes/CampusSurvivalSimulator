#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <iomanip>
#include <cstdio>
#include <conio.h>
using namespace std;

// ======================= Console Color =======================
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

enum ConsoleColor {
    BLACK=0, BLUE=1, GREEN=2, CYAN=3, RED=4, MAGENTA=5, YELLOW=6, WHITE=7,
    INTENSE_WHITE=15, INTENSE_RED=12, INTENSE_GREEN=10, INTENSE_YELLOW=14,
    INTENSE_BLUE=9, INTENSE_CYAN=11, INTENSE_MAGENTA=13
};

// ======================= Pagewise Pause =======================
void pauseScreen() {
    cout << "\n\nPress any key to continue...";
    getch();
    system("cls");
}

// ======================= Utility =======================
int getRandom(int min,int max) {
    return rand() % (max - min + 1) + min;
}

double percentageToGPA(double percentage) {
    if(percentage >= 90) return 4.0;
    if(percentage >= 85) return 3.7;
    if(percentage >= 80) return 3.3;
    if(percentage >= 75) return 3.0;
    if(percentage >= 70) return 2.7;
    if(percentage >= 65) return 2.3;
    if(percentage >= 60) return 2.0;
    if(percentage >= 55) return 1.7;
    if(percentage >= 50) return 1.0;
    return 0.0;
}

// ======================= ASCII Progress Bar =======================
void showProgressBar(string label,double percent,int width=30) {
    int filled = int((percent / 100.0) * width);
    cout << left << setw(12) << label << "[";
    for(int i=0; i < filled; i++) cout << '#';
    for(int i = filled; i < width; i++) cout << '-';
    cout << "] " << int(percent) << "%\n";
}

// ======================= Pomodoro =======================
void pomodoroTimer() {
    system("cls");
    int minutes = 25;
    cout << "\nEnter focus minutes (default 25): ";
    string line;
    getline(cin, line);
    if(!line.empty()) minutes = stoi(line);

    cout << "\nFocus session started for " << minutes << " minutes.\n";
    for(int t = minutes * 60; t >= 0; t--) {
        int mm = t / 60, ss = t % 60;
        cout << "\rTime left: "
             << setw(2) << setfill('0') << mm << ":"
             << setw(2) << setfill('0') << ss << flush;
        Sleep(1000);
    }
    cout << "\nSession complete!\n";
    pauseScreen();
}

// ======================= Save / Load Goals =======================
void saveGoals(string goals[], int count) {
    FILE *f = fopen("goals.txt","w");
    if(!f){ cout<<"Error saving goals\n"; return; }
    for(int i=0;i<count;i++)
        fprintf(f, "%s\n", goals[i].c_str());
    fclose(f);
    cout << "Saved today's goals!\n";
}

int loadGoals(string goals[]) {
    FILE *f = fopen("goals.txt","r");
    if(!f) return 0;
    char buffer[200];
    int count = 0;
    while(fgets(buffer,200,f)) {
        string s = buffer;
        s.erase(s.find_last_not_of("\n\r")+1);
        goals[count++] = s;
    }
    fclose(f);
    return count;
}

// ======================= Study Module =======================
struct StudySubject {
    string name;
    int hoursPerDay;
    int studiedHours = 0;
};

void studyModule() {
    system("cls");
    int choice;
    while(true) {
        setColor(INTENSE_BLUE);
        cout << "\n--- Study Module ---\n";
        setColor(INTENSE_YELLOW);
        cout << "1. Study Timetable\n2. Skill Development\n3. Pomodoro Focus Timer\n"
             << "4. Daily Goals\n5. Exit\n";
        setColor(INTENSE_WHITE);
        cout << "Enter choice: ";        
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: {
                system("cls");
                int subjects, totalHours;
                cout << "Number of subjects: ";
                cin >> subjects; cin.ignore();

                StudySubject subs[20];
                for(int i=0;i<subjects;i++){
                    cout<<"Subject "<<i+1<<" name: ";
                    getline(cin, subs[i].name);
                }

                cout<<"Total hours per day: ";
                cin >> totalHours; cin.ignore();

                int hoursPer = totalHours / subjects;
                for(int i=0;i<subjects;i++)
                    subs[i].hoursPerDay = hoursPer;

                for(int hour=1; hour<=totalHours; hour++){
                    system("cls");
                    setColor(INTENSE_BLUE);
                    cout<<"\nHour "<<hour<<" / "<<totalHours<<"\n";
                    setColor(INTENSE_YELLOW);
                    for(int i=0;i<subjects;i++){
                        cout<<i+1<<". "<<subs[i].name<<" ("<<subs[i].studiedHours<<"/"<<subs[i].hoursPerDay<<"h)\n";
                    }

                    setColor(INTENSE_WHITE);
                    cout<<"Choose subject: ";
                    int s;
                    cin >> s; cin.ignore();

                    if(s>=1 && s<=subjects) subs[s-1].studiedHours++;
                    else cout << "Invalid! Hour wasted.\n";

                    setColor(INTENSE_CYAN);
                    for(int i=0;i<subjects;i++){
                        double p = (double)subs[i].studiedHours / subs[i].hoursPerDay * 100;
                        if(p>100)p=100;
                        showProgressBar(subs[i].name,p);
                    }

                    setColor(INTENSE_WHITE);
                    pauseScreen();
                }
                break;
            }
            case 2: {
                system("cls");
                string skill;
                cout << "Skill: ";
                getline(cin, skill);
                setColor(INTENSE_CYAN);
                cout<<"Practice daily and set mini-goals.\n";
                setColor(INTENSE_WHITE);
                pauseScreen();
                break;
            }
            case 3:
                pomodoroTimer();
                break;
            case 4: {
                system("cls");
                cout<<"1. Save goals\n2. Load goals\nChoice: ";
                int g; cin >> g; cin.ignore();

                if(g==1){
                    int n;
                    cout<<"How many goals? "; cin >> n; cin.ignore();
                    string goals[20];
                    for(int i=0;i<n;i++){
                        cout<<"Goal "<<i+1<<": ";
                        getline(cin, goals[i]);
                    }
                    saveGoals(goals,n);
                }
                else{
                    string goals[20];
                    int count = loadGoals(goals);
                    setColor(INTENSE_GREEN);
                    cout<<"\nToday's Goals:\n";
                    for(int i=0;i<count;i++)
                        cout<<i+1<<". "<<goals[i]<<"\n";
                    setColor(INTENSE_WHITE);
                }
                pauseScreen();
                break;
            }
            case 5: return;
            default:
                cout<<"Invalid.\n";
                pauseScreen();
        }
    }
}

// ======================= Academic & Career Planner Module =======================
struct CareerOption {
    string careerName;
    double minGPA;
    string topSkills[5];
    string link;
};

void academicCareerModule() {
    system("cls");
    CareerOption options[5] = {
        {"Software Engineer", 3.5, {"C++","Python","Git","Problem Solving","Communication"}, "https://www.linkedin.com/jobs/software-developer/"},
        {"Computer Science & AI", 3.5, {"Python","ML","Data Structures","Algorithms","Problem Solving"}, "https://www.linkedin.com/jobs/computer-science-ai/"},
        {"BBA", 3.0, {"Management","Communication","Presentation","Excel","Teamwork"}, "https://www.linkedin.com/jobs/bba/"},
        {"Accounting & Finance", 3.0, {"Accounting","Excel","Finance","Analysis","Communication"}, "https://www.linkedin.com/jobs/accounting-finance/"},
        {"B.Ed", 2.5, {"Teaching","Communication","Creativity","Classroom Management","Planning"}, "https://www.linkedin.com/jobs/b-ed/"}
    };

    int choice;
    while(true){
        system("cls");
        setColor(INTENSE_BLUE);
        cout << "\n--- Academic & Career Planner ---\n";
        setColor(INTENSE_YELLOW);
        cout << "1. GPA / CGPA & Career Suggestions\n";
        cout << "2. Internship / Job Links\n";
        cout << "3. Skill Gap Analysis\n";
        cout << "4. Exit\n";
        setColor(INTENSE_WHITE);
        cout << "Choose option: "; cin >> choice; cin.ignore();

        switch(choice){
            case 1:{
                system("cls");
                cout << "Enter number of subjects: ";
                int n; cin >> n; cin.ignore();
                string names[20]; double marks[20], totalMarks[20], credits[20], gpas[20];
                double tq=0, tc=0;
                for(int i=0;i<n;i++){
                    cout << "Subject " << i+1 << " name: "; getline(cin,names[i]);
                    cout << "Obtained Marks: "; cin >> marks[i];
                    cout << "Total Marks: "; cin >> totalMarks[i];
                    cout << "Credits: "; cin >> credits[i]; cin.ignore();
                    gpas[i] = percentageToGPA((marks[i]/totalMarks[i])*100);
                    tq += gpas[i]*credits[i]; tc += credits[i];
                }
                double semesterGPA = tq/tc;
                setColor(INTENSE_CYAN);
                cout << "\nSemester GPA/CGPA: " << semesterGPA << "\n";
                cout << "Career Suggestions:\n";
                for(int i=0;i<5;i++){
                    if(semesterGPA>=options[i].minGPA){
                        cout << "• " << options[i].careerName << "\n  Skills: ";
                        for(int j=0;j<5;j++)
                            cout << options[i].topSkills[j] << (j<4?", ":"\n");
                    }
                }
                setColor(INTENSE_WHITE);
                pauseScreen();
                break;
            }
            case 2:{
                system("cls");
                setColor(INTENSE_GREEN);
                cout << "Select your field to open job link:\n";
                for(int i=0;i<5;i++) cout << i+1 << ". " << options[i].careerName << "\n";
                cout << "Choice: "; int f; cin >> f; cin.ignore();
                if(f>=1 && f<=5){
                    string cmd = "start " + options[f-1].link;
                    system(cmd.c_str());
                    cout << "Opening " << options[f-1].careerName << " job opportunities...\n";
                } else cout << "Invalid!\n";
                setColor(INTENSE_WHITE);
                pauseScreen();
                break;
            }
            case 3:{
                system("cls");
                string userSkills[10]; int count;
                cout << "How many skills do you have? "; cin >> count; cin.ignore();
                for(int i=0;i<count;i++){
                    cout << "Skill " << i+1 << ": "; getline(cin,userSkills[i]);
                }
                cout << "\nMissing skills for top careers:\n";
                for(int i=0;i<5;i++){
                    cout << "• " << options[i].careerName << ": ";
                    for(int j=0;j<5;j++){
                        bool has=false;
                        for(int k=0;k<count;k++) if(userSkills[k]==options[i].topSkills[j]) has=true;
                        if(!has) cout << options[i].topSkills[j] << " ";
                    }
                    cout << "\n";
                }
                pauseScreen();
                break;
            }
            case 4: return;
            default: cout<<"Invalid!\n"; pauseScreen();
        }
    }
}

// ======================= Campus Safety Module =======================
string generateCaseID() {
    int r = rand() % 9000 + 1000;
    time_t now = time(0);
    tm *lt = localtime(&now);
    char id[40];
    sprintf(id, "SAF-%04d%02d%02d-%d",
        lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, r
    );
    return string(id);
}

void saveSafetyReport(string caseID, string name, string type, string location, string details, int urgency, bool anonymous) {
    FILE *f = fopen("safety_reports.txt", "a");
    if(!f){ setColor(INTENSE_RED); cout<<"Error saving report!\n"; setColor(INTENSE_WHITE); return; }

    fprintf(f, "===== SAFETY REPORT =====\n");
    fprintf(f, "Case ID: %s\n", caseID.c_str());
    fprintf(f, "Reporter: %s\n", anonymous ? "ANONYMOUS" : name.c_str());
    fprintf(f, "Incident Type: %s\n", type.c_str());
    fprintf(f, "Location: %s\n", location.c_str());
    fprintf(f, "Urgency: %d\n", urgency);
    fprintf(f, "Details: %s\n", details.c_str());
    fprintf(f, "==========================\n\n");
    fclose(f);
}

void showSafetyTips() {
    setColor(INTENSE_CYAN);
    cout << "\n--- SAFETY TIPS ---\n";
    setColor(INTENSE_GREEN); cout << "• Stay in well-lit areas at night.\n";
    setColor(INTENSE_YELLOW); cout << "• Do not walk alone after 10 PM.\n";
    setColor(INTENSE_MAGENTA); cout << "• Report suspicious activity immediately.\n";
    setColor(INTENSE_BLUE); cout << "• Keep emergency contacts saved.\n";
    setColor(INTENSE_RED); cout << "• Avoid isolated shortcuts.\n";
    setColor(INTENSE_WHITE);
}

void threatMeter(int level) {
    cout << "\nThreat Level: ";
    if(level <= 2){ setColor(INTENSE_GREEN); cout << "[ LOW ]\n"; }
    else if(level <= 4){ setColor(INTENSE_YELLOW); cout << "[ MODERATE ]\n"; }
    else{ setColor(INTENSE_RED); cout << "[ HIGH ]\n"; }
    setColor(INTENSE_WHITE);
}

void campusSafetyModule() {
    int choice;
    while(true) {
        system("cls");
        setColor(INTENSE_RED);
        cout << "\n=== CAMPUS SAFETY CENTER (BLAST) ===\n";
        setColor(INTENSE_YELLOW);
        cout << "1. Report an Incident\n2. Safety Tips\n3. Emergency Gmail\n4. Threat Map\n5. Exit\n";
        setColor(INTENSE_WHITE);
        cout << "Choose option: "; cin >> choice; cin.ignore();

        switch(choice){
            case 1:{
                system("cls");
                string name,type,location,details;
                int urgency,anon;
                setColor(INTENSE_MAGENTA);
                cout << "Report anonymously? (1=Yes, 0=No): "; cin >> anon; cin.ignore();
                bool anonymous = (anon==1);
                if(!anonymous){ setColor(INTENSE_CYAN); cout << "Your name: "; getline(cin,name); } 
                else name="ANONYMOUS";

                setColor(INTENSE_BLUE);
                cout << "\nSelect Incident Type:\n";
                setColor(INTENSE_GREEN);
                cout << "1. Harassment\n2. Theft\n3. Bullying\n4. Suspicious Activity\n5. Emergency\n";
                setColor(INTENSE_YELLOW);
                cout << "Choice: "; int t; cin >> t; cin.ignore();
                type = (t==1?"Harassment":t==2?"Theft":t==3?"Bullying":t==4?"Suspicious Activity":"Emergency");

                setColor(INTENSE_CYAN);
                cout << "Location: "; getline(cin,location);
                setColor(INTENSE_MAGENTA);
                cout << "Urgency (1 to 5): "; cin >> urgency; cin.ignore();
                setColor(INTENSE_BLUE);
                cout << "Describe incident: "; getline(cin,details);

                string caseID = generateCaseID();
                saveSafetyReport(caseID,name,type,location,details,urgency,anonymous);
                threatMeter(urgency);

                setColor(INTENSE_YELLOW);
                cout << "\nCase ID: " << caseID << "\nReport submitted securely.\n";
                pauseScreen();
                break;
            }
            case 2:
                system("cls");
                showSafetyTips();
                pauseScreen();
                break;
            case 3:
                system("cls");
                setColor(INTENSE_BLUE);
                system("start https://mail.google.com/mail/u/0/?view=cm&fs=1&to=campus.safety@example.com&su=Emergency%20Help");
                cout << "Opening Gmail...\n";
                setColor(INTENSE_WHITE);
                pauseScreen();
                break;
            case 4:{
                system("cls");
                cout << "Enter location for threat map: ";
                string loc; getline(cin,loc);
                system(("start https://www.google.com/maps/search/"+loc).c_str());
                cout << "Opening Google Maps...\n";
                pauseScreen();
                break;
            }
            case 5: return;
            default: cout<<"Invalid!\n"; pauseScreen();
        }
    }
}

// ======================= MAIN MENU =======================
int main(){
    srand(time(0));
    int choice;
    while(true){
        system("cls");
        setColor(INTENSE_BLUE);
        cout<<"\n==== CAMPUS SURVIVAL SIMULATOR ====\n";
        setColor(INTENSE_YELLOW);
        cout<<"1. Study Module\n2. Academic & Career Planner\n3. Campus Safety\n4. Exit\n";
        setColor(INTENSE_WHITE);
        cout<<"Choose: "; cin >> choice; cin.ignore();

        switch(choice){
            case 1: studyModule(); break;
            case 2: academicCareerModule(); break;
            case 3: campusSafetyModule(); break;
            case 4: return 0;
            default: cout<<"Invalid.\n"; pauseScreen();
        }
    }
}
