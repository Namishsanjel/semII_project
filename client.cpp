#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

/* ---------- BASE CLASS ---------- */
class User {
protected:
    string name;
public:
    virtual void menu() = 0;   // abstraction
};

/* ---------- COMPLAINT CLASS ---------- */
class Complaint {
private:
    int id;
    string name, address, type, description, status;

public:
    Complaint(int cid, string n, string a, string t, string d)
        : id(cid), name(n), address(a), type(t), description(d), status("Pending") {}

    void save() {
        ofstream file("complaints.txt", ios::app);
        if (!file) {
            cout << "Error opening file!\n";
            return;
        }
        file << id << "|" << name << "|" << address << "|"
             << type << "|" << description << "|" << status << endl;
        file.close();
    }

    static bool showStatus(int searchId) {
        ifstream file("complaints.txt");
        if (!file) {
            cout << "File not found!\n";
            return false;
        }

        string line;
        while (getline(file, line)) {
            int id;
            sscanf(line.c_str(), "%d|", &id);
            if (id == searchId) {
                cout << "\nComplaint Found\n";
                cout << "Status: " << line.substr(line.find_last_of('|') + 1) << endl;
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }
};

/* ---------- CLIENT CLASS ---------- */
class Client : public User {
private:
    int generateID() {
        static int id = 1000;
        return ++id;
    }

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    void menu() override {
        int choice;
        do {
            cout << "\n========== CLIENT MENU ==========\n";
            cout << "1. Register Complaint\n";
            cout << "2. Show Complaint Status\n";
            cout << "3. Exit\n";
            cout << "Enter choice: ";

            if (!(cin >> choice)) {
                clearInput();
                cout << "Invalid input!\n";
                continue;
            }

            cin.ignore();

            switch (choice) {
                case 1: {
                    string address, type, desc;
                    cout << "Enter Name: ";
                    getline(cin, name);
                    cout << "Enter Address: ";
                    getline(cin, address);
                    cout << "Enter Problem Type: ";
                    getline(cin, type);
                    cout << "Enter Description: ";
                    getline(cin, desc);

                    if (name.empty() || address.empty() || type.empty() || desc.empty()) {
                        cout << "All fields are required!\n";
                        break;
                    }

                    int id = generateID();
                    Complaint c(id, name, address, type, desc);
                    c.save();

                    cout << "\nComplaint Registered Successfully!\n";
                    cout << "Your Complaint ID: " << id << endl;
                    break;
                }

                case 2: {
                    int id;
                    cout << "Enter Complaint ID: ";
                    if (!(cin >> id)) {
                        clearInput();
                        cout << "Invalid ID format!\n";
                        break;
                    }
                    if (!Complaint::showStatus(id)) {
                        cout << "No matching ID found!\n";
                    }
                    break;
                }

                case 3:
                    cout << "Exiting Client...\n";
                    break;

                default:
                    cout << "Invalid choice!\n";
            }

        } while (choice != 3);
    }
};

int main() {
    Client client;
    client.menu();
    return 0;
}
