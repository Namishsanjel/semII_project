#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Abstract base class
class Person {
protected:
    string name;
    string address;
public:
    virtual void input() = 0;
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

    void input() override {
        cin.ignore();
        cout << "Enter your name: ";
        getline(cin, name);
        cout << "Enter your address: ";
        getline(cin, address);
        cout << "Enter complaint type (Road/Garbage/Water/Electricity/Other): ";
        getline(cin, type);
        cout << "Enter complaint description: ";
        getline(cin, description);

        status = "Pending";
        id = ++idCounter;
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::app);
        file << id << endl
             << name << endl
             << address << endl
             << type << endl
             << description << endl
             << status << endl;
        file.close();
    }

    static void initializeId(const string& filename) {
        ifstream file(filename);
        string line;
        int lastId = 0;
        while (getline(file, line)) {
            lastId++;
        }
        idCounter = (lastId / 6); // Each complaint uses 6 lines
        file.close();
    }

    int getId() const { return id; }
};

int Complaint::idCounter = 0;

// Main program
int main() {
    const string filename = "complaints.txt";

    Complaint::initializeId(filename);

    Complaint c;
    c.input();
    c.saveToFile(filename);

    cout << "\nComplaint registered successfully! Your Complaint ID is: " << c.getId() << endl;
    return 0;
}
