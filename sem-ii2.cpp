#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/* =======================
   ABSTRACT BASE CLASS
   ======================= */
class Person {
protected:
    string name;
    string address;

public:
    virtual void input() = 0;
    virtual void display() const = 0;
    virtual ~Person() {}
};

/* =======================
   COMPLAINT CLASS
   ======================= */
class Complaint : public Person {
private:
    int id;
    string type;
    string description;
    string status;

public:
    static int idCounter;

    Complaint() {}

    int getId() const {
        return id;
    }

    void setStatus(const string& s) {
        status = s;
    }

    void input() override {
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Address: ";
        getline(cin, address);
        cout << "Enter Complaint Type: ";
        getline(cin, type);
        cout << "Enter Description: ";
        getline(cin, description);

        status = "Pending";
        id = ++idCounter;
    }

    void display() const override {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nAddress: " << address
             << "\nType: " << type
             << "\nDescription: " << description
             << "\nStatus: " << status << endl;
    }

    void writeToFile(ofstream& out) const {
        out << id << endl
            << name << endl
            << address << endl
            << type << endl
            << description << endl
            << status << endl;
    }

    void readFromFile(ifstream& in) {
        in >> id;
        in.ignore();
        getline(in, name);
        getline(in, address);
        getline(in, type);
        getline(in, description);
        getline(in, status);

        if (id > idCounter)
            idCounter = id;
    }
};

int Complaint::idCounter = 0;

/* =======================
   REPOSITORY CLASS
   ======================= */
class ComplaintRepository {
private:
    vector<Complaint> complaints;
    string filename = "complaints.txt";

public:
    ComplaintRepository() {
        load();
    }

    void load() {
        ifstream file(filename);
        if (!file) return;

        while (file.peek() != EOF) {
            Complaint c;
            c.readFromFile(file);
            complaints.push_back(c);
        }
        file.close();
    }

    void save() {
        ofstream file(filename);
        for (const auto& c : complaints)
            c.writeToFile(file);
        file.close();
    }

    vector<Complaint>& getAll() {
        return complaints;
    }

    void add(const Complaint& c) {
        complaints.push_back(c);
        save();
    }
};

/* =======================
   ADMIN CLASS
   ======================= */
class Admin {
public:
    bool login() {
        string u, p;
        cout << "Username: ";
        cin >> u;
        cout << "Password: ";
        cin >> p;
        return u == "admin" && p == "1234";
    }
};

/* =======================
   SYSTEM CONTROLLER
   ======================= */
class ComplaintSystem {
private:
    ComplaintRepository repo;
    Admin admin;

public:
    void registerComplaint() {
        Complaint c;
        c.input();
        repo.add(c);
        cout << "\nComplaint Registered Successfully!\n";
    }

    void viewComplaints() {
        for (const auto& c : repo.getAll())
            c.display();
    }

    void updateStatus() {
        int id;
        cout << "Enter Complaint ID: ";
        cin >> id;

        for (auto& c : repo.getAll()) {
            if (c.getId() == id) {
                string s;
                cout << "Enter New Status: ";
                cin.ignore();
                getline(cin, s);
                c.setStatus(s);
                repo.save();
                cout << "Status Updated!\n";
                return;
            }
        }
        cout << "Complaint Not Found!\n";
    }

    void run() {
        int choice;
        do {
            cout << "\n1. Register Complaint\n2. Admin Login\n3. Exit\nChoice: ";
            cin >> choice;

            if (choice == 1) {
                registerComplaint();
            } else if (choice == 2) {
                if (admin.login()) {
                    int ch;
                    do {
                        cout << "\n1. View\n2. Update\n3. Logout\nChoice: ";
                        cin >> ch;
                        if (ch == 1) viewComplaints();
                        else if (ch == 2) updateStatus();
                    } while (ch != 3);
                } else {
                    cout << "Invalid Login\n";
                }
            }
        } while (choice != 3);
    }
};

/* =======================
   MAIN
   ======================= */
int main() {
    ComplaintSystem system;
    system.run();
    return 0;
}