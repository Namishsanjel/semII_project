#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Admin {
private:
    string username, password;

public:
    Admin(string u, string p) {
        username = u;
        password = p;
    }

    void save() {
        ofstream file("admins.txt", ios::app);
        file << username << " " << password << endl;
        file.close();
    }

    static bool login(string u, string p) {
        ifstream file("admins.txt");
        string user, pass;

        while (file >> user >> pass) {
            if (user == u && pass == p) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }
};

class ComplaintManager {
public:
    static void viewAll() {
        ifstream file("complaints.txt");
        string line;

        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    static void updateStatus(int searchId, string newStatus) {
        ifstream file("complaints.txt");
        ofstream temp("temp.txt");

        string line;
        bool found = false;

        while (getline(file, line)) {
            int id;
            sscanf(line.c_str(), "%d|", &id);

            if (id == searchId) {
                size_t last = line.find_last_of('|');
                line = line.substr(0, last + 1) + newStatus;
                found = true;
            }
            temp << line << endl;
        }

        file.close();
        temp.close();

        remove("complaints.txt");
        rename("temp.txt", "complaints.txt");

        if (found)
            cout << "Status updated successfully\n";
        else
            cout << "Invalid ID\n";
    }
};

class AdminApp {
public:
    void menu() {
        int choice;
        do {
            cout << "\n1. Login\n2. Register\n3. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                string u, p;
                cout << "Username: ";
                cin >> u;
                cout << "Password: ";
                cin >> p;

                if (Admin::login(u, p)) {
                    adminMenu();
                } else {
                    cout << "Invalid credentials\n";
                }
            }
            else if (choice == 2) {
                string u, p;
                cout << "New Username: ";
                cin >> u;
                cout << "New Password: ";
                cin >> p;

                Admin a(u, p);
                a.save();
                cout << "Admin registered successfully\n";
            }
            else if (choice == 3) {
                cout << "Exiting...\n";
            }
            else {
                cout << "Invalid Choice\n";
            }
        } while (choice != 3);
    }

private:
    void adminMenu() {
        int ch;
        do {
            cout << "\n1. View all complain\n2. Update complain status\n3. Logout\n";
            cout << "Enter choice: ";
            cin >> ch;

            if (ch == 1) {
                ComplaintManager::viewAll();
            }
            else if (ch == 2) {
                int id;
                string status;
                cout << "Enter Complaint ID: ";
                cin >> id;
                cout << "Enter new status (Pending/In progress/Resolved): ";
                cin.ignore();
                getline(cin, status);

                ComplaintManager::updateStatus(id, status);
            }
            else if (ch == 3) {
                cout << "Logging out...\n";
            }
            else {
                cout << "Invalid choice\n";
            }
        } while (ch != 3);
    }
};

int main() {
    AdminApp app;
    app.menu();
    return 0;
}
