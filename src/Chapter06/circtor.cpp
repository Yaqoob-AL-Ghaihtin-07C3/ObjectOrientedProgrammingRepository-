#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

const string DATA_FILE = "attendance.dat";
const string DELIMITER = "|||";

struct Student {
    int lectureNo;
    int id;
    string name;
    bool attended;
    string note;
};

vector<Student> students;

// وظائف النظام الأساسية
void loadData();
void saveData();
void addRecord();
void viewAll();
void searchStudent();
void generateReport();

int main() {
    loadData();
    
    while (true) {
        cout << "\n\nنظام إدارة الحضور والغياب\n";
        cout << "1. تسجيل حضور/غياب\n";
        cout << "2. عرض جميع السجلات\n";
        cout << "3. بحث عن طالب\n";
        cout << "4. إحصائية الحضور\n";
        cout << "5. خروج\n";
        cout << "اختر الخيار: ";
        
        int choice;
        cin >> choice;
        cin.ignore(); // لإزالة بقايا الإدخال
        
        switch (choice) {
            case 1: addRecord(); break;
            case 2: viewAll(); break;
            case 3: searchStudent(); break;
            case 4: generateReport(); break;
            case 5: saveData(); return 0;
            default: cout << "اختيار غير صحيح!\n";
        }
    }
}

void loadData() {
    ifstream file(DATA_FILE);
    if (!file) return;

    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find(DELIMITER);
        size_t pos2 = line.find(DELIMITER, pos1+1);
        size_t pos3 = line.find(DELIMITER, pos2+1);
        size_t pos4 = line.find(DELIMITER, pos3+1);

        Student s;
        s.lectureNo = stoi(line.substr(0, pos1));
        s.id = stoi(line.substr(pos1+3, pos2-pos1-3));
        s.name = line.substr(pos2+3, pos3-pos2-3);
        s.attended = (line.substr(pos3+3, pos4-pos3-3) == "1");
        s.note = line.substr(pos4+3);

        students.push_back(s);
    }
    file.close();
}

void saveData() {
    ofstream file(DATA_FILE);
    for (const auto& s : students) {
        file << s.lectureNo << DELIMITER
             << s.id << DELIMITER
             << s.name << DELIMITER
             << (s.attended ? "1" : "0") << DELIMITER
             << s.note << "\n";
    }
    file.close();
}

void addRecord() {
    Student s;
    cout << "رقم المحاضرة: "; cin >> s.lectureNo;
    cout << "ID الطالب: "; cin >> s.id;
    cin.ignore();
    cout << "الاسم: "; getline(cin, s.name);
    cout << "الحضور (1/0): "; cin >> s.attended;
    cin.ignore();
    cout << "ملاحظة: "; getline(cin, s.note);
    
    students.push_back(s);
    cout << "تمت الإضافة بنجاح!\n";
}

void viewAll() {
    cout << "\nجميع السجلات:\n";
    for (const auto& s : students) {
        cout << "المحاضرة: " << s.lectureNo 
             << " | ID: " << s.id 
             << " | الاسم: " << s.name 
             << " | الحضور: " << (s.attended ? "✅" : "❌")
             << " | ملاحظة: " << s.note << "\n";
    }
}

void searchStudent() {
    int id;
    cout << "أدخل ID الطالب: "; cin >> id;
    
    bool found = false;
    for (const auto& s : students) {
        if (s.id == id) {
            cout << "المحاضرة: " << s.lectureNo << " | الحضور: " 
                 << (s.attended ? "✅" : "❌") << " | ملاحظة: " << s.note << "\n";
            found = true;
        }
    }
    
    if (!found) cout << "لا توجد سجلات لهذا الطالب!\n";
}

void generateReport() {
    int present = count_if(students.begin(), students.end(), 
                          [](const Student& s) { return s.attended; });
    
    cout << "\nإحصائية الحضور:\n";
    cout << "عدد السجلات: " << students.size() << "\n";
    cout << "عدد الحضور: " << present << "\n";
    cout << "عدد الغياب: " << students.size() - present << "\n";
    cout << "نسبة الحضور: " << (present * 100.0 / students.size()) << "%\n";
}