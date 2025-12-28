#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Complaint {
    int id;
    string name;
    string address;
    string type;
    string description;
    string status;
};

vector<Complaint> loadComplaints() {
    vector<Complaint> complaints;
    ifstream file("complaints.txt");
    Complaint c;
    while (file >> c.id) {
        file.ignore();
        getline(file, c.name);
        getline(file, c.address);
        getline(file, c.type);
        getline(file, c.description);
        getline(file, c.status);
        complaints.push_back(c);
    }
    file.close();
    return complaints;
}

void saveComplaints(const vector<Complaint>& complaints) {
    ofstream file("complaints.txt");
    for (const auto& c : complaints) {
        file << c.id << endl;
        file << c.name << endl;
        file << c.address << endl;
        file << c.type << endl;
        file << c.description << endl;
        file << c.status << endl;
    }
    file.close();
}

void registerComplaint() {
    vector<Complaint> complaints = loadComplaints();
    Complaint c;

    c.id = complaints.empty() ? 1 : complaints.back().id + 1;
    cin.ignore();
    cout << "\nEnter your name: ";
    getline(cin, c.name);
    cout << "Enter your address: ";
    getline(cin, c.address);
    cout << "Enter complaint type (Road/Garbage/Water/Electricity/Other): ";
    getline(cin, c.type);
    cout << "Enter complaint description: ";
    getline(cin, c.description);
    c.status = "Pending";

    complaints.push_back(c);
    saveComplaints(complaints);

    cout << "\nComplaint registered successfully! Your Complaint ID is: " << c.id << endl;
}

void viewComplaints() {
    vector<Complaint> complaints = loadComplaints();
    cout << "\n--- All Complaints ---\n";
    for (const auto& c : complaints) {
        cout << "\nComplaint ID: " << c.id
             << "\nName: " << c.name
             << "\nAddress: " << c.address
             << "\nType: " << c.type
             << "\nDescription: " << c.description
             << "\nStatus: " << c.status << endl;
    }
}

void updateComplaintStatus() {
    vector<Complaint> complaints = loadComplaints();
    int searchId;
    cout << "\nEnter Complaint ID to update: ";
    cin >> searchId;

    bool found = false;
    for (auto& c : complaints) {
        if (c.id == searchId) {
            cout << "Current Status: " << c.status << endl;
            cout << "Enter new status (Pending/In Progress/Resolved): ";
            cin.ignore();
            getline(cin, c.status);
            found = true;
            break;
        }
    }

    if (found) {
        saveComplaints(complaints);
        cout << "Status updated successfully!" << endl;
    } else {
        cout << "Complaint ID not found." << endl;
    }
}

bool adminLogin() {
    string username, password;
    cout << "\n--- Admin Login ---\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    // Simple hardcoded login (can be improved later)
    return username == "admin" && password == "1234";
}

int main() {
    int choice;
    do {
        cout << "\n==== Municipality Complaint & Feedback System ====\n";
        cout << "1. Register Complaint\n";
        cout << "2. Admin Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerComplaint();
                break;
            case 2:
                if (adminLogin()) {
                    int adminChoice;
                    do {
                        cout << "\n--- Admin Menu ---\n";
                        cout << "1. View All Complaints\n";
                        cout << "2. Update Complaint Status\n";
                        cout << "3. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> adminChoice;
                        switch (adminChoice) {
                            case 1:
                                viewComplaints();
                                break;
                            case 2:
                                updateComplaintStatus();
                                break;
                            case 3:
                                cout << "Logged out.\n";
                                break;
                            default:
                                cout << "Invalid choice.\n";
                        }
                    } while (adminChoice != 3);
                } else {
                    cout << "Invalid admin credentials!\n";
                }
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 3);

    return 0;
}