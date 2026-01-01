#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

/* ---------- BASE CLASS ---------- */
class User {
protected:
    string username, password;
public:
    virtual void menu() = 0;
};

/* ---------- ADMIN CLASS ---------- */
class Admin : public User {
public:
    Admin() {}
    Admin(string u, string p) { username = u; password = p; }

    void save() {
        ofstream file("admins.txt", ios::app);
        if (!file) {
            cout << "❌ Error saving admin!\n";
            return;
        }
        file << username << " " << password << endl;
        file.close();
    }

    static bool login(string u, string p) {
        ifstream file("admins.txt");
        if (!file) {
            cout << "❌ Admin file missing!\n";
            return false;
        }

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

/* ---------- COMPLAINT MANAGER ---------- */
class ComplaintManager {
public:
    static void viewAll() {
        ifstream file("complaints.txt");
        if (!file) {
            cout << "❌ No complaints available!\n";
            return;
        }

        cout << "\n========== ALL COMPLAINTS ==========\n";
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    static void updateStatus(int id, string status) {
        ifstream file("complaints.txt");
        ofstream temp("temp.txt");

        if (!file || !temp) {
            cout << "❌ File error!\n";
            return;
        }

        string line;
        bool found = false;

        while (getline(file, line)) {
            int cid;
            sscanf(line.c_str(), "%d|", &cid);

            if (cid == id) {
                line = line.substr(0, line.find_last_of('|') + 1) + status;
                found = true;
            }
            temp << line << endl;
        }

        file.close();
        temp.close();

        remove("complaints.txt");
        rename("temp.txt", "complaints.txt");

        if (found)
            cout << "✅ Status updated successfully!\n";
        else
            cout << "❌ Invalid Complaint ID!\n";
    }
};

/* ---------- ADMIN APPLICATION ---------- */
class AdminApp : public User {
private:
    void adminMenu() {
        int ch;
        do {
            cout << "\n========== ADMIN PANEL ==========\n";
            cout << "1. View All Complaints\n";
            cout << "2. Update Complaint Status\n";
            cout << "3. Logout\n";
            cout << "Enter choice: ";
            cin >> ch;

            switch (ch) {
                case 1:
                    ComplaintManager::viewAll();
                    break;

                case 2: {
                    int id;
                    string status;
                    cout << "Enter Complaint ID: ";
                    cin >> id;
                    cin.ignore();

                    cout << "Enter Status (Pending/In Progress/Resolved): ";
                    getline(cin, status);

                    if (status != "Pending" &&
                        status != "In Progress" &&
                        status != "Resolved") {
                        cout << "❌ Invalid status!\n";
                        break;
                    }

                    ComplaintManager::updateStatus(id, status);
                    break;
                }

                case 3:
                    cout << "Logging out...\n";
                    break;

                default:
                    cout << "❌ Invalid choice!\n";
            }
        } while (ch != 3);
    }

public:
    void menu() override {
        int choice;
        do {
            cout << "\n========== ADMIN MENU ==========\n";
            cout << "1. Login\n2. Register\n3. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;

                if (Admin::login(username, password)) {
                    adminMenu();
                } else {
                    cout << "❌ Invalid credentials!\n";
                }
            }
            else if (choice == 2) {
                cout << "New Username: ";
                cin >> username;
                cout << "New Password: ";
                cin >> password;

                Admin a(username, password);
                a.save();
                cout << "✅ Admin registered successfully!\n";
            }
            else if (choice == 3) {
                cout << "Exiting Admin...\n";
            }
            else {
                cout << "❌ Invalid choice!\n";
            }
        } while (choice != 3);
    }
};

int main() {
    AdminApp app;
    app.menu();
    return 0;
}
