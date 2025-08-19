#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <memory>

#define MAX_VERTICES 14
#define INFINITY_COST 999999
using namespace std;

int global_count = 0;

struct Hospital1 {
    int index;
    string name;
    string address;
};

// Exception handling class
class MyException : public runtime_error {
public:
    explicit MyException(const string& message) : runtime_error(message) {}
};

// Forward declarations
class Hospital;

// Doctor class derived from Staff

// Abstract Staff class
class Staff {
protected:
    int id;
    string name;
    string role;
    Hospital* department;
    string contactInfo;

public:
    Staff(int id, const string& name, const string& role, Hospital* department, const string& contactInfo)
        : id(id), name(name), role(role), department(department), contactInfo(contactInfo) {}

    virtual ~Staff() {}

    virtual double calculateSalary() const = 0;

    virtual void assignToDepartment(Hospital* newDepartment) {
        department = newDepartment;
    }

    void updateContactInfo(const string& newContactInfo) {
        contactInfo = newContactInfo;
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getRole() const {
        return role;
    }
};

class Doctor : public Staff {
private:
    string specialization;

public:
    Doctor(int id, const string& name, Hospital* department, const string& contactInfo, const string& specialization)
        : Staff(id, name, "Doctor", department, contactInfo), specialization(specialization) {}

    string getSpecialization() const {
        return specialization;
    }

    double calculateSalary() const override {
        return 5000; // Base salary + specialization bonus
    }
};


// Hospital class
class Hospital {
private:
    string name;
    string address;
    vector<unique_ptr<Staff>> staffMembers; // Use unique_ptr for automatic memory management
    map<int, string> appointments;
    vector<int> bookedRooms;

public:
    Hospital(const string& name, const string& address) : name(name), address(address) {}

    string getName() const {
        return name;
    }

    string getAddress() const {
        return address;
    }

    void addStaffMember(unique_ptr<Staff> staff) {
        staffMembers.push_back(move(staff));
    }

    vector<Staff*> searchDoctorBySpecialization(const string& specialization) const {
        vector<Staff*> doctors;
        for (const auto& staff : staffMembers) {
            if (staff->getRole() == "Doctor") {
                Doctor* doctor = dynamic_cast<Doctor*>(staff.get());
                if (doctor && caseInsensitiveCompare(doctor->getSpecialization(), specialization)) {
                    doctors.push_back(staff.get());
                }
            }
        }
        return doctors;
    }

    void makeAppointment(int appointmentId, const string& details) {
        if (appointments.find(appointmentId) != appointments.end()) {
            throw MyException("Appointment ID already exists");
        }
        appointments[appointmentId] = details;
    }

    void cancelAppointment(int appointmentId) {
        auto it = appointments.find(appointmentId);
        if (it != appointments.end()) {
            appointments.erase(it);
        } else {
            throw MyException("Appointment not found");
        }
    }

    void bookRoom(int roomId) {
        if (find(bookedRooms.begin(), bookedRooms.end(), roomId) != bookedRooms.end()) {
            throw MyException("Room already booked");
        }
        bookedRooms.push_back(roomId);
    }

    void cancelRoomBooking(int roomId) {
        auto it = find(bookedRooms.begin(), bookedRooms.end(), roomId);
        if (it != bookedRooms.end()) {
            bookedRooms.erase(it);
        } else {
            throw MyException("Room not booked");
        }
    }

    static bool caseInsensitiveCompare(const string& str1, const string& str2) {
        if (str1.size() != str2.size()) return false;
        for (size_t i = 0; i < str1.size(); ++i) {
            if (tolower(str1[i]) != tolower(str2[i])) return false;
        }
        return true;
    }
};

// Nurse class derived from Staff
class Nurse : public Staff {
private:
    string duty;

public:
    Nurse(int id, const string& name, Hospital* department, const string& contactInfo, const string& duty)
        : Staff(id, name, "Nurse", department, contactInfo), duty(duty) {}

    double calculateSalary() const override {
        return 3000; // Base salary
    }
};

class Appointment {
private:
    int id;
    string patientName;
    int doctorId;
    string date;
    string time;

public:
    Appointment(int id, const string& patientName, int doctorId, const string& date, const string& time)
        : id(id), patientName(patientName), doctorId(doctorId), date(date), time(time) {}

    int getId() const {
        return id;
    }

    string getPatientName() const {
        return patientName;
    }

    int getDoctorId() const {
        return doctorId;
    }

    string getDate() const {
        return date;
    }

    string getTime() const {
        return time;
    }
};
#include <iostream>
using namespace std;

// Billing class
class Billing {
private:
    double doctorFee;
    double labFee;
    double pharmacyFee;

public:
    Billing() : doctorFee(0), labFee(0), pharmacyFee(0) {}

    // Template function to add fees
    template <typename T>
    void addFee(T fee, double& feeVariable) {
        feeVariable = static_cast<double>(fee);
    }

    // Methods to add specific fees using the template function
    template <typename T>
    void addDoctorFee(T fee) {
        addFee(fee, doctorFee);
    }

    template <typename T>
    void addLabFee(T fee) {
        addFee(fee, labFee);
    }

    template <typename T>
    void addPharmacyFee(T fee) {
        addFee(fee, pharmacyFee);
    }

    double calculateTotal() const {
        return doctorFee + labFee + pharmacyFee;
    }

    void processPayment() const {
        double totalAmount = calculateTotal();
        cout << "Total payment amount: $" << totalAmount << endl;
        cout << "Payment of $" << totalAmount << " processed successfully.\n";
    }
};



// Pharmacy class
class Pharmacy {
public:
    void orderMedication(const string& medication, int quantity) {
        cout << "Ordered " << quantity << " units of " << medication << " from pharmacy.\n";
    }
};

class Laboratory {
public:
    void runTests(const string& testName) {
        // Code to run the specific test
        cout << "Running test: " << testName << endl;
    }

    void displayTestOptions() {
        cout << "Choose a test from the following options:" << endl;
        cout << "1. Complete Blood Count (CBC)" << endl;
        cout << "2. Blood Chemistry Panel" << endl;
        cout << "3. Urinalysis" << endl;
        cout << "4. Thyroid Function Tests" << endl;
        cout << "5. Lipid Profile" << endl;
        cout << "6. Liver Function Tests" << endl;
        cout << "7. Renal Function Tests" << endl;
        cout << "8. Electrolyte Panel" << endl;
        cout << "9. Coagulation Studies" << endl;
        cout << "10. Blood Glucose Test" << endl;
    }
};

// Room class
class Room {
public:
    void clean() {
        cout << "Room cleaned.\n";
    }
};



vector<Hospital1> hospitals = {
    {0, "Lingrajnagar Hospital", "Lingrajnagar"},
    {1, "KLE TECH Hospital", "KLE TECH"},
    {2, "Vidyanagar Hospital", "Vidyanagar"},
    {3, "KIMS Hospital", "KIMS"},
    {4, "Hosur Hospital", "Hosur"},
    {5, "Gokul Road Hospital", "Gokul Road"},
    {6, "Shreya Nagar Hospital", "Shreya Nagar"},
    {7, "Tolankeri Hospital", "Tolankeri"},
    {8, "Chennamma Circle Hospital", "Chennamma Circle"},
    {9, "HDMC Hospital", "HDMC"},
    {10, "Railway Colony Hospital", "Railway Colony"},
    {11, "Unkal Hospital", "Unkal"},
    {12, "Navanagar Hospital", "Navanagar"},
    {13, "Rayapur Hospital", "Rayapur"}
};

vector<int> distances = {
    10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140
};

void nearestHospital(int src) {
    int minDistance = INFINITY_COST;
    int nearestHospitalIndex = -1;

    for (int i = 0; i < hospitals.size(); i++) {
        if (distances[i] < minDistance) {
            minDistance = distances[i];
            nearestHospitalIndex = i;
        }
    }

    if (nearestHospitalIndex != -1) {
        cout << "Nearest hospital: " << hospitals[nearestHospitalIndex].name
             << " at a distance of " << minDistance << " km\n";
        int choice;
        cout << "Do You Want To Book an Ambulance (1: YES)? ";
        cin >> choice;
        if (choice == 1) {
            string phoneNumber;
            cout << "Enter YOUR Phone Number: ";
            cin >> phoneNumber;
            cout << "Your Ambulance Will be Arriving in "
                 << minDistance * 2 + 5 << " mins\n";
        }
    } else {
        cout << "No hospitals found.\n";
    }
}

void ambulance() {
    int src;
    cout << "CHOOSE YOUR LOCATION CODE\n"
         << "0 - Lingrajnagar\n"
         << "1 - KLE TECH\n"
         << "2 - Vidyanagar\n"
         << "3 - KIMS\n"
         << "4 - Hosur\n"
         << "5 - Gokul Road\n"
         << "6 - Shreya Nagar\n"
         << "7 - Tolankeri\n"
         << "8 - Chennamma Circle\n"
         << "9 - HDMC\n"
         << "10 - Railway Colony\n"
         << "11 - Unkal\n"
         << "12 - Navanagar\n"
         << "13 - Rayapur\n"
         << "Enter your location code: ";
    cin >> src;

    if (src < 0 || src >= MAX_VERTICES) {
        cout << "Invalid location code. Exiting...\n";
        return;
    }

    nearestHospital(src);
}


int main() {
    try {
        // Create Hospital
        string hospitalName = "CITY HOSPITAL";
        string hospitalAddress = "123 Main, Vidyanagar-30";
        Hospital hospital(hospitalName, hospitalAddress);

        // Create and add doctors to the hospital
        vector<pair<int, pair<string, string>>> doctorsList = {
            {1, {"Dr. Ayesha Khan", "Pediatrician"}},
            {2, {"Dr. Vikram Patel", "Gynecologist"}},
            {3, {"Dr. Anil Mehta", "Cardiologist"}},
            {4, {"Dr. Rajesh Singh", "Physician"}},
            {5, {"Dr. Neha Sharma", "Dermatologist"}},
            {6, {"Dr. Kavita Rao", "Orthopaedist"}},
            {7, {"Dr. Arjun Das", "Dentist"}},
            {8, {"Dr. Priya Iyer", "ENT"}},
            {9, {"Dr. Rohit Sinha", "Pediatrician"}},
            {10, {"Dr. Smita Joshi", "Cardiologist"}}
        };

        for (const auto& doctor : doctorsList) {
            hospital.addStaffMember(make_unique<Doctor>(doctor.first, doctor.second.first, &hospital, "", doctor.second.second));
        }

        // Display hospital information
        cout << "\n\n\t\t\t\tWelcome to " << hospital.getName() << " located at " << hospital.getAddress() << endl;

        int choice;
        do {
            cout << "\nMENU:\n";
            cout << "1. Search for Doctor\n";
            cout << "2. Make Appointment\n";
            cout << "3. Cancel Appointment\n";
            cout << "4. Book a Room\n";
            cout << "5. Cancel Room Booking\n";
            cout << "6. Laboratory\n";
            cout << "7. Billing\n";
            cout << "8. BOOK AMBULANCE\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if(cin.fail()) {
                cout << "Invalid input. Please enter a number.\n";
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                continue;
            }

            switch (choice) {
                case 1: {
                    string specialization;
                    cout << "Specializations available:\n";
                    cout << " Pediatrician\n Gynecologist\n Cardiologist\n Physician\n Dermatologist\n Orthopaedist\n Dentist\n ENT\n";
                    cout << "Enter doctor's specialization: ";
                    cin.ignore(); // To ignore any leftover newline character in the input buffer
                    getline(cin, specialization);
                    vector<Staff*> doctors = hospital.searchDoctorBySpecialization(specialization);
                    if (doctors.empty()) {
                        cout << "No doctors found with specialization " << specialization << ".\n";
                    } else {
                        cout << "Doctors with specialization " << specialization << ":\n";
                        for (const auto& doctor : doctors) {
                            cout << "ID: " << doctor->getId() << ", Name: " << doctor->getName() << endl;
                        }
                    }
                    break;
                }
                case 2: {
                    int appointmentId, doctorId;
                    string patientName, date, time;
                    cout << "Enter appointment ID: ";
                    cin >> appointmentId;
                    if(cin.fail()) {
                        cout << "Invalid input. Please enter a  valid appointment ID.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    cout << "Enter patient name: ";
                    cin.ignore(); // Ignore newline character
                    getline(cin, patientName);
                    cout << "Enter doctor ID: ";
                    cin >> doctorId;
                    if(cin.fail()) {
                        cout << "Invalid input. Please enter a valid doctor ID.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    cout << "Enter date (YYYY-MM-DD): ";
                    cin.ignore(); // Ignore newline character
                    getline(cin, date);
                    cout << "Enter time (HH:MM): ";
                    getline(cin, time);
                    Appointment appointment(appointmentId, patientName, doctorId, date, time);
                    hospital.makeAppointment(appointment.getId(), "Patient: " + appointment.getPatientName() + ", Doctor ID: " + to_string(appointment.getDoctorId()) + ", Date: " + appointment.getDate() + ", Time: " + appointment.getTime());
                    cout << "Appointment made successfully.\n";
                    break;
                }
                case 3: {
                    int appointmentId;
                    cout << "Enter appointment ID to cancel: ";
                    cin >> appointmentId;
                    if(cin.fail()) {
                        cout << "Invalid input. Please enter a valid appointment ID.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    hospital.cancelAppointment(appointmentId);
                    cout << "Appointment cancelled successfully.\n";
                    break;
                }
                case 4: {
                    int roomId;
                    cout << "Enter room ID to book: ";
                    cin >> roomId;
                    if(cin.fail()) {
                        cout << "Invalid input. Please enter a valid room ID.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    hospital.bookRoom(roomId);
                    cout << "Room booked successfully.\n";
                    break;
                }
                case 5: {
                    int roomId;
                    cout << "Enter room ID to cancel booking: ";
                    cin >> roomId;
                    if(cin.fail()) {
                        cout << "Invalid input. Please enter a valid room ID.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    hospital.cancelRoomBooking(roomId);
                    cout << "Room booking cancelled successfully.\n";
                    break;
                }
                case 6: {
                    Laboratory lab;
                    int choice;

    lab.displayTestOptions();

    cout << "Enter the number corresponding to the test you want to run: ";
    cin >> choice;

    switch (choice) {
        case 1:
            lab.runTests("Complete Blood Count (CBC)");
            break;
        case 2:
            lab.runTests("Blood Chemistry Panel");
            break;
        case 3:
            lab.runTests("Urinalysis");
            break;
        case 4:
            lab.runTests("Thyroid Function Tests");
            break;
        case 5:
            lab.runTests("Lipid Profile");
            break;
        case 6:
            lab.runTests("Liver Function Tests");
            break;
        case 7:
            lab.runTests("Renal Function Tests");
            break;
        case 8:
            lab.runTests("Electrolyte Panel");
            break;
        case 9:
            lab.runTests("Coagulation Studies");
            break;
        case 10:
            lab.runTests("Blood Glucose Test");
            break;
        default:
            cout << "Invalid choice!";
            break;
    }


                }
                case 7: {


    Billing bill;

    double doctorFee, labFee, pharmacyFee;

    // Ask user for input
    cout << "Enter the doctor's fee: ";
    cin >> doctorFee;
    bill.addDoctorFee(doctorFee);

    cout << "Enter the lab fee: ";
    cin >> labFee;
    bill.addLabFee(labFee);

    cout << "Enter the pharmacy fee: ";
    cin >> pharmacyFee;
    bill.addPharmacyFee(pharmacyFee);

    bill.processPayment();
    return 0;

                    break;
                }
                case 8:
                ambulance();
                break;
                case 0:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 0);
    } catch (const MyException& e) {
        cerr << "Exception: " << e.what() << endl;
    } catch (const exception& e)  {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
