#include <iostream>
#include <string>
using namespace std;

class Patient {
public:
    int id;
    string name;
    int age;
    string disease;
    string admissionDate;

    Patient() {
        this->id = 0;
        this->name = "";
        this->age = 0;
        this->disease = "";
        this->admissionDate = "";
    }

    Patient(int pid, string pname, int page, string pdisease, string pdate) {
        this->id = pid;
        this->name = pname;
        this->age = page;
        this->disease = pdisease;
        this->admissionDate = pdate;
    }
};

class Bed {
public:
    int bedNumber;
    Patient patient;
    bool occupied;
    Bed* next;
    Bed* prev;

    Bed(int number) {
        this->bedNumber = number;
        this->occupied = false;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

class Hospital {
public:
    Bed* head;
    Bed* tail;

    Hospital() {
        this->head = nullptr;
        this->tail = nullptr;
    }

    ~Hospital() {
        Bed* temp = head;
        while (temp) {
            Bed* next = temp->next;
            delete temp;
            temp = next;
        }
    }

    void addBed(int bedNumber) {
        Bed* newBed = new Bed(bedNumber);
        if (head == nullptr) {
            head = tail = newBed;
        } else {
            tail->next = newBed;
            newBed->prev = tail;
            tail = newBed;
        }
    }

    bool isIDDuplicate(int id) {
        Bed* temp = head;
        while (temp) {
            if (temp->occupied && temp->patient.id == id)
                return true;
            temp = temp->next;
        }
        return false;
    }

    bool isValidDate(string date) {
        if (date.size() != 10) return false;
        if (date[2] != '/' || date[5] != '/') return false;
        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) continue;
            if (!isdigit(date[i])) return false;
        }
        return true;
    }

    void assignPatient(int bedNumber, Patient p) {
        if (p.id <= 0) { cout << "Error: Invalid Patient ID.\n"; return; }
        if (p.age <= 0) { cout << "Error: Invalid Age.\n"; return; }
        if (p.name.empty()) { cout << "Error: Name cannot be empty.\n"; return; }
        if (p.disease.empty()) { cout << "Error: Disease cannot be empty.\n"; return; }
        if (!isValidDate(p.admissionDate)) { cout << "Error: Invalid date format (dd/mm/yyyy).\n"; return; }
        if (isIDDuplicate(p.id)) { cout << "Error: Duplicate Patient ID.\n"; return; }

        Bed* temp = head;
        while (temp) {
            if (temp->bedNumber == bedNumber) {
                if (!temp->occupied) {
                    temp->patient = p;
                    temp->occupied = true;
                    cout << "Patient assigned to bed " << bedNumber << ".\n";
                } else {
                    cout << "Error: Bed " << bedNumber << " is already occupied by " << temp->patient.name << ".\n";
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Error: Bed " << bedNumber << " not found.\n";
    }

    void dischargePatient(int bedNumber) {
        Bed* temp = head;
        while (temp) {
            if (temp->bedNumber == bedNumber) {
                if (temp->occupied) {
                    temp->occupied = false;
                    cout << "Patient discharged from bed " << bedNumber << ".\n";
                } else {
                    cout << "Error: Bed " << bedNumber << " is already empty.\n";
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Error: Bed " << bedNumber << " not found.\n";
    }

    void displayBedsForward() {
        if (head == nullptr) {
            cout << "No beds available.\n";
            return;
        }
        Bed* temp = head;
        cout << "\n--- Bed List (Forward) ---\n";
        while (temp) {
            cout << "Bed " << temp->bedNumber << ": ";
            if (temp->occupied)
                cout << "Occupied by " << temp->patient.name << ", Age: " << temp->patient.age
                     << ", Disease: " << temp->patient.disease
                     << ", Date: " << temp->patient.admissionDate << "\n";
            else
                cout << "Available\n";
            temp = temp->next;
        }
    }

    void displayBedsBackward() {
        if (tail == nullptr) {
            cout << "No beds available.\n";
            return;
        }
        Bed* temp = tail;
        cout << "\n--- Bed List (Backward) ---\n";
        while (temp) {
            cout << "Bed " << temp->bedNumber << ": ";
            if (temp->occupied)
                cout << "Occupied by " << temp->patient.name << ", Age: " << temp->patient.age
                     << ", Disease: " << temp->patient.disease
                     << ", Date: " << temp->patient.admissionDate << "\n";
            else
                cout << "Available\n";
            temp = temp->prev;
        }
    }

    void searchByBedNumber(int bedNumber) {
        Bed* temp = head;
        while (temp) {
            if (temp->bedNumber == bedNumber) {
                if (temp->occupied)
                    cout << "Bed " << bedNumber << " is occupied by " << temp->patient.name << ".\n";
                else
                    cout << "Bed " << bedNumber << " is available.\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Error: Bed not found.\n";
    }

    void searchByPatientName(string name) {
        Bed* temp = head;
        while (temp) {
            if (temp->occupied && temp->patient.name == name) {
                cout << name << " is in bed " << temp->bedNumber << ".\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Patient not found.\n";
    }

    void displayStatistics() {
        int total = 0, occupied = 0;
        Bed* temp = head;
        while (temp) {
            total++;
            if (temp->occupied) occupied++;
            temp = temp->next;
        }
        cout << "Total Beds: " << total
             << ", Occupied: " << occupied
             << ", Available: " << total - occupied << "\n";
        if (total > 0)
            cout << "Occupancy Rate: " << (occupied * 100 / total) << "%\n";
    }
};

int main() {
    Hospital hospital;
    int choice, bedNum, totalBeds;
    Patient p;
    string name;

    cout << "Enter total number of beds in hospital: ";
    cin >> totalBeds;
    for (int i = 1; i <= totalBeds; i++) {
        hospital.addBed(i);
    }

    do {
        cout << "\n====== Hospital Bed Management System ======\n";
        cout << "1. Assign Patient to Bed\n";
        cout << "2. Discharge Patient\n";
        cout << "3. Display Beds (Forward)\n";
        cout << "4. Display Beds (Backward)\n";
        cout << "5. Search by Bed Number\n";
        cout << "6. Search by Patient Name\n";
        cout << "7. Display Statistics\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Bed Number: "; cin >> bedNum;
            cout << "Enter Patient ID: "; cin >> p.id;
            cin.ignore();
            cout << "Enter Name: "; getline(cin, p.name);
            cout << "Enter Age: "; cin >> p.age;
            cin.ignore();
            cout << "Enter Disease: "; getline(cin, p.disease);
            cout << "Enter Admission Date (dd/mm/yyyy): "; getline(cin, p.admissionDate);
            hospital.assignPatient(bedNum, p);
            break;
        case 2:
            cout << "Enter Bed Number to discharge: "; cin >> bedNum;
            hospital.dischargePatient(bedNum);
            break;
        case 3:
            hospital.displayBedsForward();
            break;
        case 4:
            hospital.displayBedsBackward();
            break;
        case 5:
            cout << "Enter Bed Number: "; cin >> bedNum;
            hospital.searchByBedNumber(bedNum);
            break;
        case 6:
            cin.ignore();
            cout << "Enter Patient Name: "; getline(cin, name);
            hospital.searchByPatientName(name);
            break;
        case 7:
            hospital.displayStatistics();
            break;
        case 8:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
