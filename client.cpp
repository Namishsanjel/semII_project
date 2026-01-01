#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Complaint {
private:
    int id;
    string name, address, type, description, status;

public:
    Complaint(int cid, string n, string a, string t, string d) {
        id = cid;
        name = n;
        address = a;
        type = t;
        description = d;
        status = "Pending";
    }

    void saveToFile() {
        ofstream file("complaints.txt", ios::app);
        file << id << "|" << name << "|" << address << "|"
             << type << "|" << description << "|" << status << endl;
        file.close();
    }

    static void showStatus(int searchId) {
        ifstream file("complaints.txt");
        string line;
        bool found = false;

        while (getline(file, line)) {
            int id;
            string name, address, type, desc, status;

            sscanf(line.c_str(), "%d|", &id);

            if (id == searchId) {
                size_t last = line.find_last_of('|');
                status = line.substr(last + 1);
                cout << "Complaint Status: " << status << endl;
                found = true;
                break;
            }
        }

        file.close();

        if (!found) {
            cout << "No matching ID\n";
        }
    }
};

class ClientApp {
private:
    int generateID() {
        static int id = 1000;
        return ++id;
    }

public:
    void menu() {
        int choice;
        do {
            cout << "\n1. Register Complaint\n2. Show Status\n3. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                string name, address, type, desc;
                cout << "Name: ";
                getline(cin, name);
                cout << "Address: ";
                getline(cin, address);
                cout << "Problem Type: ";
                getline(cin, type);
                cout << "Description: ";
                getline(cin, desc);

                int id = generateID();
                Complaint c(id, name, address, type, desc);
                c.saveToFile();

                cout << "Complaint registered successfully!\n";
                cout << "Your Complaint ID is: " << id << endl;
            }
            else if (choice == 2) {
                int id;
                cout << "Enter Complaint ID: ";
                cin >> id;
                Complaint::showStatus(id);
            }
            else if (choice == 3) {
                cout << "Exiting...\n";
            }
            else {
                cout << "Invalid choice\n";
            }
        } while (choice != 3);
    }
};

int main() {
    ClientApp app;
    app.menu();
    return 0;
}
