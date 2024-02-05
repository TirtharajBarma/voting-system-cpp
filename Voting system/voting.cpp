#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <cctype>

using namespace std;

// Structure to represent a voter
struct Voter {
    string username;
    string password;
    string name;
    bool hasVoted;
    string dob; // Date of Birth
    string voterID; // Voter ID
};

// Structure to represent a candidate
struct Candidate {
    string name;
    int votes;
};

// Function prototypes
void registration(vector<Voter> &voters);
bool login(vector<Voter> &voters, Voter &currentUser);
void updateProfile(vector<Voter> &voters, Voter &currentUser);
void giveVote(vector<Voter> &voters, Voter &currentUser, map<string, Candidate> &candidates);
void displayResult(const map<string, Candidate> &candidates);
void viewPersonalDetails(const Voter &currentUser);
void loadVoterData(vector<Voter> &voters);
void saveVoterData(const vector<Voter> &voters);
void savePersonalDetails(const Voter &voter);
void saveVoteDetails(const Voter &voter, const string &candidateName);
string generateVoterID();

// File to store voter data
const string voterDataFile = "voter_data.txt";
// File to store personal details
const string personalDetailsFile = "personal_details.txt";
// File to store vote details
const string voteDetailsFile = "vote_details.txt";

void registration(vector<Voter> &voters) {
    Voter newVoter;
    cout << "Enter a unique username: ";
    cin >> newVoter.username;
    for (const Voter &voter : voters) {
        if (voter.username == newVoter.username) {
            cout << "Username already exists. Registration failed." << endl;
            return;
        }
    }
    cout << "Enter a password: ";
    cin >> newVoter.password;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, newVoter.name);
    cout << "Enter your date of birth (YYYY-MM-DD): ";
    cin >> newVoter.dob;

    // Validate DOB
    int year, month, day;
    if (sscanf(newVoter.dob.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        cout << "Invalid date format. Registration failed." << endl;
        return;
    }

    if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31) {
        cout << "Invalid date." << endl;
        return;
    }

    // Calculate age
    struct tm* bornDate;
    time_t now = time(0);
    bornDate = localtime(&now);
    bornDate->tm_year = year - 1900;
    bornDate->tm_mon = month - 1;
    bornDate->tm_mday = day;

    double seconds = difftime(now, mktime(bornDate));
    double age = seconds / (60 * 60 * 24 * 365.25);

    if (age < 18.0) {
        cout << "You must be at least 18 years old to register. Registration failed." << endl;
        return;
    }

    // Generate voter ID
    newVoter.voterID = generateVoterID();

    newVoter.hasVoted = false;
    voters.push_back(newVoter);
    cout << "Registration successful!" << endl;
    cout << "Your Voter ID is: " << newVoter.voterID << endl; // Display the generated voterID
    saveVoterData(voters); // Save voter data after registration
    savePersonalDetails(newVoter); // Save personal details to a file
}

bool login(vector<Voter> &voters, Voter &currentUser) {
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    for (Voter &voter : voters) {
        if (voter.username == username && voter.password == password) {
            currentUser = voter;
            return true;
        }
    }

    return false;
}

void updateProfile(vector<Voter> &voters, Voter &currentUser) {
    cout << "Update Profile Menu" << endl;
    cout << "1. Change Password\n2. Change Name\n3. Back to Main Menu" << endl;
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "Enter new password: ";
            cin >> currentUser.password;
            break;
        case 2:
            cout << "Enter new name: ";
            cin.ignore();
            getline(cin, currentUser.name);
            break;
        case 3:
            // Return to the main menu
            break;
        default:
            cout << "Invalid choice." << endl;
            return;
    }

    // Save the updated voter data to the file immediately
    saveVoterData(voters);
    cout << "Profile updated successfully!" << endl;
    savePersonalDetails(currentUser); // Save updated personal details to a file
}

void giveVote(vector<Voter> &voters, Voter &currentUser, map<string, Candidate> &candidates) {
    cout << "Candidates:" << endl;
    for (const auto &pair : candidates) {
        cout << pair.first << endl;
    }

    string candidateName;
    cout << "Enter the name of the candidate you want to vote for: ";
    cin.ignore();
    getline(cin, candidateName);

    if (candidates.find(candidateName) != candidates.end()) {
        candidates[candidateName].votes++;
        currentUser.hasVoted = true;
        cout << "Vote cast successfully!" << endl;

        // Save voter data after voting and update the 'hasVoted' field
        saveVoterData(voters);
        saveVoteDetails(currentUser, candidateName); // Save vote details to a file
    } else {
        cout << "Invalid candidate name." << endl;
    }
}

void displayResult(const map<string, Candidate> &candidates) {
    cout << "Voting Results:" << endl;
    for (const auto &pair : candidates) {
        cout << pair.first << ": " << pair.second.votes << " votes" << endl;
    }
}

void viewPersonalDetails(const Voter &currentUser) {
    cout << "Personal Details:" << endl;
    cout << "Username: " << currentUser.username << endl;
    cout << "Name: " << currentUser.name << endl;
    cout << "Date of Birth: " << currentUser.dob << endl;
    cout << "Has Voted: " << (currentUser.hasVoted ? "Yes" : "No") << endl;
    cout << "Voter ID: " << currentUser.voterID << endl;
}

void loadVoterData(vector<Voter> &voters) {
    ifstream inFile(voterDataFile);
    if (inFile.is_open()) {
        Voter voter;
        while (inFile >> voter.username >> voter.password >> voter.name >> voter.hasVoted >> voter.dob >> voter.voterID) {
            voters.push_back(voter);
        }
        inFile.close();
    }
}

void saveVoterData(const vector<Voter> &voters) {
    ofstream outFile(voterDataFile);
    if (outFile.is_open()) {
        for (const Voter &voter : voters) {
            outFile << voter.username << " " << voter.password << " " << voter.name << " " << voter.hasVoted << " " << voter.dob << " " << voter.voterID << endl;
        }
        outFile.close();
    }
}

void savePersonalDetails(const Voter &voter) {
    ofstream outFile(personalDetailsFile, ios::app); // Open in append mode to save multiple records
    if (outFile.is_open()) {
        outFile << "Username: " << voter.username << endl;
        outFile << "Name: " << voter.name << endl;
        outFile << "Date of Birth: " << voter.dob << endl;
        outFile << "Has Voted: " << (voter.hasVoted ? "Yes" : "No") << endl;
        outFile << "Voter ID: " << voter.voterID << endl;
        outFile << "------------------------\n"; // Separator
        outFile.close();
    }
}

void saveVoteDetails(const Voter &voter, const string &candidateName) {
    ofstream outFile(voteDetailsFile, ios::app); // Open in append mode to save multiple records
    if (outFile.is_open()) {
        outFile << "Username: " << voter.username << endl;
        outFile << "Voted for: " << candidateName << endl;
        outFile << "------------------------\n"; // Separator
        outFile.close();
    }
}

string generateVoterID() {
    static const char alphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const char numeric[] =
        "0123456789";

    string voterID = "";
    // First 5 characters are random uppercase alphabets
    for (int i = 0; i < 5; ++i) {
        voterID += alphabet[rand() % (sizeof(alphabet) - 1)];
    }
    // Next 4 characters are random numbers
    for (int i = 0; i < 4; ++i) {
        voterID += numeric[rand() % (sizeof(numeric) - 1)];
    }
    // Last character is a random uppercase alphabet
    voterID += alphabet[rand() % (sizeof(alphabet) - 1)];

    return voterID;
}

int main() {
    vector<Voter> voters;
    map<string, Candidate> candidates;
    Voter currentUser;
    int choice;

    // Load existing voter data from file
    loadVoterData(voters);

    // Add some sample candidates to the map
    Candidate candidateA;
    candidateA.name = "Candidate A";
    candidateA.votes = 0;
    candidates["trump"] = candidateA;

    Candidate candidateB;
    candidateB.name = "Candidate B";
    candidateB.votes = 0;
    candidates["putin"] = candidateB;

    Candidate candidateC;
    candidateC.name = "Candidate C";
    candidateC.votes = 0;
    candidates["modi"] = candidateC;

    // Seed the random number generator
    srand(static_cast<unsigned>(time(0)));

    bool exitProgram = false;

    while (!exitProgram) {
        cout << "Online Voting System" << endl;
        cout << "1. Registration\n2. Login/Log out\n3. Update Profile\n4. Give Vote\n5. Display Result\n6. View Personal Details\n7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registration(voters);
                break;
            case 2:
                if (!currentUser.username.empty()) {
                    currentUser = Voter(); // Log out
                    cout << "Logged out." << endl;
                } else {
                    if (login(voters, currentUser)) {
                        cout << "Login successful!" << endl;
                    } else {
                        cout << "Login failed. Please try again." << endl;
                    }
                }
                break;
            case 3:
                if (!currentUser.username.empty()) {
                    updateProfile(voters, currentUser);
                } else {
                    cout << "Please log in first." << endl;
                }
                break;
            case 4:
                if (!currentUser.username.empty()) {
                    if (!currentUser.hasVoted) {
                        giveVote(voters, currentUser, candidates);
                    } else {
                        cout << "You have already voted." << endl;
                    }
                } else {
                    cout << "Please log in first." << endl;
                }
                break;
            case 5:
                if (!currentUser.username.empty()) {
                    displayResult(candidates);
                } else {
                    cout << "Please log in first." << endl;
                }
                break;
            case 6:
                if (!currentUser.username.empty()) {
                    viewPersonalDetails(currentUser);
                } else {
                    cout << "Please log in first." << endl;
                }
                break;
            case 7:
                // Save voter data to file before exiting
                saveVoterData(voters);
                cout << "Exiting the program." << endl;
                exitProgram = true; // Exit the while loop and terminate the program
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
