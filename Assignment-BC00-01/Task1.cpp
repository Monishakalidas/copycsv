#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <iomanip> // For formatting salary output
#include <limits>  // For clearing input buffer

using namespace std;

struct Employee {
    string name;
    string designation;
    float salary;
    int age;
    string phoneNumber;
};

bool isValidName(const string& name) {
    for (char c : name) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

bool isValidPhoneNumber(const string& phoneNumber) {
    return phoneNumber.length() == 10 && all_of(phoneNumber.begin(), phoneNumber.end(), ::isdigit);
}

bool isValidAge(const string& ageStr) {
    if (!all_of(ageStr.begin(), ageStr.end(), ::isdigit)) {
        return false;
    }
    int age = stoi(ageStr);
    return age >= 18 && age <= 100; // Example age range
}

void addToCSV(const vector<Employee>& employees) {
    ofstream file("Employees.csv");
    if (file.is_open()) {
        for (const auto& employee : employees) {
            file << employee.name << "," << employee.designation << "," 
                 << fixed << setprecision(0) << employee.salary << "," 
                 << employee.age << "," << employee.phoneNumber << endl;
        }
        file.close();
    } else {
        cout << "Unable to open file Employees.csv!" << endl;
    }
}

vector<Employee> readFromCSV() {
    ifstream file("Employees.csv");
    vector<Employee> employees;
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            Employee employee;
            size_t pos = 0;
            pos = line.find(",");
            employee.name = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            employee.designation = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            employee.salary = stof(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(",");
            employee.age = stoi(line.substr(0, pos));
            employee.phoneNumber = line.substr(pos + 1);

            employees.push_back(employee);
        }
        file.close();
    }
    return employees;
}

void copyToBCSV() {
    ifstream fileA("Employees.csv");
    ofstream fileB("B_Employees.csv");

    if (fileA.is_open() && fileB.is_open()) {
        string line;
        while (getline(fileA, line)) {
            fileB << line << endl;
        }
        fileA.close();
        fileB.close();
        cout << "Contents of Employees.csv copied to B_Employees.csv Successfully.\n";
    } else {
        cout << "Unable to open one or both of the files!" << endl;
    }
}

void displayBCSV() {
    ifstream file("B_Employees.csv");
    if (file.is_open()) {
        string line;
        cout << "Contents of B_Employees.csv:\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Unable to open B_Employees.csv!" << endl;
    }
}

void updateEmployee(vector<Employee>& employees, const string& name) {
    for (auto& employee : employees) {
        if (employee.name == name) {
            string ageStr;
            cout << "Enter updated Age: ";
            cin >> ageStr;

            if (!isValidAge(ageStr)) {
                cout << "Invalid age! Please enter a valid number between 18 and 100.\n";
                return;
            }

            string phoneNumber;
            cout << "Enter updated Phone Number (10 digits): ";
            cin >> phoneNumber;
            if (!isValidPhoneNumber(phoneNumber)) {
                cout << "Invalid phone number! Please enter a valid 10-digit number.\n";
                return;
            }

            string designation;
            cout << "Enter updated Designation: ";
            cin >> designation;

            float salary;
            cout << "Enter updated Salary: ";
            cin >> salary;

            employee.age = stoi(ageStr);
            employee.phoneNumber = phoneNumber;
            employee.designation = designation;
            employee.salary = salary;
            addToCSV(employees);
            cout << "Employee updated successfully.\n";
            return;
        }
    }
    cout << "Employee not found!\n";
}

void deleteEmployee(vector<Employee>& employees, const string& name) {
    auto it = remove_if(employees.begin(), employees.end(), [&](const Employee& e) {
        return e.name == name;
    });

    if (it != employees.end()) {
        employees.erase(it, employees.end());
        addToCSV(employees);
        cout << "Employee deleted successfully.\n";
    } else {
        cout << "Employee not found!\n";
    }
}

int main() {
    vector<Employee> employees;
    int choice;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add employee data to Employees.csv\n";
        cout << "2. Display employee data from Employees.csv\n";
        cout << "3. Update employee by name\n";
        cout << "4. Delete employee by name\n";
        cout << "5. Copy Employees.csv to B_Employees.csv\n";
        cout << "6. Display B_Employees.csv\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number between 1 and 7.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore();

        switch (choice) {
            case 1: {
                string name, phoneNumber, designation, ageStr;
                float salary;

                cout << "Enter Name: ";
                getline(cin, name);
                if (!isValidName(name)) {
                    cout << "Invalid name! Only alphabets and spaces allowed.\n";
                    break;
                }

                cout << "Enter Designation: ";
                getline(cin, designation);

                cout << "Enter Salary: ";
                cin >> salary;

                cout << "Enter Age: ";
                cin >> ageStr;
                if (!isValidAge(ageStr)) {
                    cout << "Invalid age! Enter a valid number between 18 and 100.\n";
                    break;
                }

                cout << "Enter Phone Number (10 digits): ";
                cin >> phoneNumber;
                if (!isValidPhoneNumber(phoneNumber)) {
                    cout << "Invalid phone number! Enter a valid 10-digit number.\n";
                    break;
                }

                employees.push_back({name, designation, salary, stoi(ageStr), phoneNumber});
                addToCSV(employees);
                cout << "Employee data added to Employees.csv Successfully.\n";
                break;
            }
            
            case 2: {
                employees = readFromCSV();
                for (const auto& employee : employees) {
                    cout << "Name: " << employee.name 
                         << ", Designation: " << employee.designation
                         << ", Salary: " << fixed << setprecision(0) << employee.salary 
                         << ", Age: " << employee.age
                         << ", Phone Number: " << employee.phoneNumber << endl;
                }
                break;
            }

            case 3: {
                cout << "Enter name of employee to update: ";
                string name;
                getline(cin, name);
                updateEmployee(employees, name);
                break;
            }

            case 4: {
                cout << "Enter name of employee to delete: ";
                string name;
                getline(cin, name);
                deleteEmployee(employees, name);
                break;
            }

            case 5:
                copyToBCSV();
                break;

            case 6:
                displayBCSV();
                break;

            case 7:
                cout << "Exiting program.\n";
                return 0;
            
            default:
                cout << "Invalid choice! Please enter a number between 1 and 7.\n";
                break;
        }
    }
    return 0;
}
