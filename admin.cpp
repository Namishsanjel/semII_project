#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Base class for person
class Person {
protected:
    string name;
    string address;
public:
    virtual void display() const = 0;
    virtual ~Person() {}
};

// Complaint class
class Complaint : public Person {
private:
    int id;
    string type;
    string description;
    string status;
public:
    static int idCounter;

    Complaint() {}

    void loadFromFile(ifstream& in) {
        in >> id;
        in.ignore();
        getline(in, name);
        getline(in, address);
        getline(in, type);
        getline(in, description);
        getline(in, status);

        if (id > idCounter) idCounter = id;
    }

    void display() const override {
        cout << "\nComplaint ID: " << id
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

    int getId() const { return id; }
    void setStatus(const string& s) { status = s; }
};

int Complaint::idCounter = 0;

// Repository class to handle file
class ComplaintRepository {
private:
    vector<Complaint> complaints;
    string filename;
public:
    ComplaintRepository(const string& fname) : filename(fname) {
        load();
    }

    void load() {
        ifstream file(filename);
        if (!file) return;

        while (file.peek() != EOF) {
            Complaint c;
            c.loadFromFile(file);
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

    vector<Complaint>& getAll() { return complaints; }
};

// Admin class
class Admin {
public:
    bool login() {
        string username, password;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        return username == "admin" && password == "1234";
    }
};

// System controller
class ComplaintSystem {
private:
    ComplaintRepository repo;
    Admin admin;
public:
    ComplaintSystem(const string& fname) : repo(fname) {}

    void run() {
        if (!admin.login()) {
            cout << "Invalid credentials!\n";
            return;
        }

        int choice;
        do {
            cout << "\n--- Admin Menu ---\n"
                 << "1. View all complaints\n"
                 << "2. Update complaint status\n"
                 << "3. Exit\nChoice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    for (const auto& c : repo.getAll()) c.display();
                    break;
                case 2: {
                    int id;
                    cout << "Enter Complaint ID to update: ";
                    cin >> id;

                    bool found = false;
                    for (auto& c : repo.getAll()) {
                        if (c.getId() == id) {
                            cin.ignore();
                            string s;
                            cout << "Enter new status: ";
                            getline(cin, s);
                            c.setStatus(s);
                            found = true;
                            break;
                        }
                    }

                    if (found) {
                        repo.save();
                        cout << "Status updated!\n";
                    } else {
                        cout << "Complaint not found!\n";
                    }
                    break;
                }
                case 3:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 3);
    }
};

// Main
int main() {
    ComplaintSystem system("complaints.txt");
    system.run();
    return 0;
}
