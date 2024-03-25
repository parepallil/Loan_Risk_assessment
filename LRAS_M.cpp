#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// Define structs for Applicant and Loan
struct Applicant {
    string name;
    double income;
    int creditScore;
    bool employmentStatus;
};

struct Loan {
    double amount;
    int term;
    double interestRate;
    bool defaultStatus;
};

// Forward declaration of getUserInput function
void getUserInput(Applicant& applicant, Loan& loan);

// DataCollection class
class DataCollection {
public:
    vector<Applicant> readApplicantsFromFile(const string& filename);
    vector<Loan> readLoansFromFile(const string& filename);
    void writeApplicantsToFile(const vector<Applicant>& applicants, const string& filename);
};

// DecisionTree class
class DecisionTree {
public:
    DecisionTree(const vector<Applicant>& applicants, const vector<Loan>& loans);
    bool predict(const Applicant& applicant, const Loan& loan);
};

// AdaBoost class
class AdaBoost {
private:
    vector<DecisionTree> weakLearners;
    vector<double> alphaValues;

public:
    AdaBoost(const vector<Applicant>& applicants, const vector<Loan>& loans);
    bool predict(const Applicant& applicant, const Loan& loan);
};

// IntegrationAndDeployment class
class IntegrationAndDeployment {
public:
    bool assessLoanRisk(const Applicant& applicant, const Loan& loan, const string& applicantsFile, const string& loansFile);
    vector<Applicant> readApplicantsFromFile(const string& filename);
    void writeApplicantsToFile(const vector<Applicant>& applicants, const string& filename);
};

// Function to get user input for applicant details and loan details
void getUserInput(Applicant& applicant, Loan& loan) {
    cout << "Enter applicant's name: ";
    cin >> applicant.name;
    cout << "Enter applicant's income: ";
    cin >> applicant.income;
    cout << "Enter applicant's credit score: ";
    cin >> applicant.creditScore;
    cout << "Enter applicant's employment status (1 for employed, 0 for unemployed): ";
    cin >> applicant.employmentStatus;
    cout << "Enter loan amount: ";
    cin >> loan.amount;
    cout << "Enter loan term (in months): ";
    cin >> loan.term;
    cout << "Enter loan interest rate: ";
    cin >> loan.interestRate;
    cout << "Enter loan default status (1 for default, 0 for no default): ";
    cin >> loan.defaultStatus;
}

// Define methods of DataCollection class
vector<Applicant> DataCollection::readApplicantsFromFile(const string& filename) {
    vector<Applicant> applicants;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Applicant applicant;
        ss >> applicant.name >> applicant.income >> applicant.creditScore >> applicant.employmentStatus;
        applicants.push_back(applicant);
    }
    return applicants;
}

vector<Loan> DataCollection::readLoansFromFile(const string& filename) {
    vector<Loan> loans;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Loan loan;
        ss >> loan.amount >> loan.term >> loan.interestRate >> loan.defaultStatus;
        loans.push_back(loan);
    }
    return loans;
}

void DataCollection::writeApplicantsToFile(const vector<Applicant>& applicants, const string& filename) {
    ofstream file(filename);
    for (const auto& applicant : applicants) {
        file << applicant.name << " " << applicant.income << " " << applicant.creditScore << " " << applicant.employmentStatus << endl;
    }
}

// Define methods of DecisionTree class
DecisionTree::DecisionTree(const vector<Applicant>& applicants, const vector<Loan>& loans) {
    // Implement decision tree construction
}

bool DecisionTree::predict(const Applicant& applicant, const Loan& loan) {
    // Example logic: If applicant's credit score is greater than 700, predict no default, otherwise predict default
    return applicant.creditScore > 700;
}

// Define methods of AdaBoost class
AdaBoost::AdaBoost(const vector<Applicant>& applicants, const vector<Loan>& loans) {
    size_t dataSize = applicants.size();
    vector<double> weights(dataSize, 1.0 / dataSize);

    for (int i = 0; i < 5; ++i) {
        DecisionTree tree(applicants, loans);
        bool prediction;
        double error = 0.0;

        for (size_t j = 0; j < dataSize; ++j) {
            prediction = tree.predict(applicants[j], loans[j]);
            if (prediction != loans[j].defaultStatus) {
                error += weights[j];
            }
        }

        double alpha = 0.5 * log((1.0 - error) / error);
        alphaValues.push_back(alpha);

        for (size_t j = 0; j < dataSize; ++j) {
            prediction = tree.predict(applicants[j], loans[j]);
            if (prediction != loans[j].defaultStatus) {
                weights[j] *= exp(alpha);
            }
            else {
                weights[j] *= exp(-alpha);
            }
        }

        double sumWeights = accumulate(weights.begin(), weights.end(), 0.0);
        for (auto& weight : weights) {
            weight /= sumWeights;
        }

        weakLearners.push_back(tree);
    }
}

bool AdaBoost::predict(const Applicant& applicant, const Loan& loan) {
    double sum = 0.0;
    for (size_t i = 0; i < weakLearners.size(); ++i) {
        bool prediction = weakLearners[i].predict(applicant, loan);
        sum += alphaValues[i] * (prediction ? 1 : -1);
    }
    return sum > 0;
}

// Define methods of IntegrationAndDeployment class
vector<Applicant> IntegrationAndDeployment::readApplicantsFromFile(const string& filename) {
    DataCollection dataCollection;
    return dataCollection.readApplicantsFromFile(filename);
}

void IntegrationAndDeployment::writeApplicantsToFile(const vector<Applicant>& applicants, const string& filename) {
    DataCollection dataCollection;
    dataCollection.writeApplicantsToFile(applicants, filename);
}

bool IntegrationAndDeployment::assessLoanRisk(const Applicant& applicant, const Loan& loan, const string& applicantsFile, const string& loansFile) {
    DataCollection dataCollection;
    vector<Applicant> applicants = dataCollection.readApplicantsFromFile(applicantsFile);
    vector<Loan> loans = dataCollection.readLoansFromFile(loansFile);
    AdaBoost adaBoost(applicants, loans);
    bool adaBoostPrediction = adaBoost.predict(applicant, loan);
    applicants.push_back(applicant);
    writeApplicantsToFile(applicants, applicantsFile);
    return adaBoostPrediction;
}
// Main function
int main() {
    IntegrationAndDeployment integrationAndDeployment;

    int choice;
    do {
        cout << "\nMenu:" << endl;
        cout << "1. Risk Analysis" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Applicant applicant;
            Loan loan;
            getUserInput(applicant, loan);
            bool riskAssessment = integrationAndDeployment.assessLoanRisk(applicant, loan, "applicants.txt", "loans.txt");
            cout << "Risk Assessment Result: " << (riskAssessment ? "High risk" : "Low risk") << endl;
            break;
        }
        case 2: {
            cout << "Exiting program..." << endl;
            break;
        }
        default:
            cout << "Invalid choice. Please enter a number between 1 and 2." << endl;
        }
    } while (choice != 2);

    return 0;
}
