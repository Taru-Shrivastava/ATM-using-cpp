#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string FILE_EXTENSION = ".txt";

class BankAccount {
public:
    BankAccount(string accountName, double accountBalance) :
        accountName(accountName), accountBalance(accountBalance), next(nullptr) {}

    void deposit(double amount) {
        accountBalance += amount;
    }

    bool withdraw(double amount) {
        if (accountBalance >= amount) {
            accountBalance -= amount;
            return true;
        } else {
            cout << "Insufficient funds!" << endl;
            return false;
        }
    }

    void transfer(BankAccount& destination, double amount) {
        if (withdraw(amount)) {
            destination.deposit(amount);
        } else {
            cout << "Transfer failed! Insufficient funds." << endl;
        }
    }

    void printInfo() const {
        cout << "Account Name: " << accountName << endl;
        cout << "Account Balance: $" << accountBalance << endl;
    }

    BankAccount* getNext() const {
        return next;
    }

    void setNext(BankAccount* nextAccount) {
        next = nextAccount;
    }

    // Read account information from a file
    static BankAccount* readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return nullptr;
        }

        string accountName;
        double accountBalance;
        file >> accountName >> accountBalance;

        file.close();
        return new BankAccount(accountName, accountBalance);
    }

    // Write account information to a file
    void writeToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        file << accountName << " " << accountBalance;

        file.close();
    }

private:
    string accountName;
    double accountBalance;
    BankAccount* next;
};

class ATMUser {
public:
    ATMUser(string name) : name(name), accounts(nullptr) {}

    void addAccount(BankAccount* account) {
        if (!accounts) {
            accounts = account;
        } else {
            account->setNext(accounts);
            accounts = account;
        }
    }

    void printAccounts() const {
        BankAccount* currentAccount = accounts;
        while (currentAccount) {
            currentAccount->printInfo();
            currentAccount = currentAccount->getNext();
        }
    }

    // Read user information from a file
    static ATMUser* readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return nullptr;
        }

        string userName;
        file >> userName;

        ATMUser* user = new ATMUser(userName);

        int numAccounts;
        file >> numAccounts;

        for (int i = 0; i < numAccounts; ++i) {
            BankAccount* account = BankAccount::readFromFile(filename + "_account" + to_string(i) + FILE_EXTENSION);
            if (account) {
                user->addAccount(account);
            }
        }

        file.close();
        return user;
    }

    // Write user information to a file
    void writeToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        file << name << endl;
        file << getNumAccounts() << endl;

        BankAccount* currentAccount = accounts;
        int accountIndex = 0;
        while (currentAccount) {
            currentAccount->writeToFile(filename + "_account" + to_string(accountIndex) + FILE_EXTENSION);
            ++accountIndex;
            currentAccount = currentAccount->getNext();
        }

        file.close();
    }

private:
    string name;
    BankAccount* accounts;

    int getNumAccounts() const {
        int count = 0;
        BankAccount* currentAccount = accounts;
        while (currentAccount) {
            ++count;
            currentAccount = currentAccount->getNext();
        }
        return count;
    }
};

int main() {
    // Create an ATM user
    ATMUser user("John Doe");
    string acc1, acc2;
    double amt1 , amt2;
    cout << "Enter account names for John Doe: ";
    cin >> acc1 >> acc2;

    cout << "Enter amount : ";
    cin >> amt1 >> amt2;

    // Create bank accounts
    BankAccount account1(acc1, amt1);
    BankAccount account2(acc2, amt2);
    // Add accounts to the user
    user.addAccount(&account1);
    user.addAccount(&account2);

    // Print user accounts
    cout << "User Accounts:" << endl;
    user.printAccounts();

    // Write user information to a file
    user.writeToFile("user_info" + FILE_EXTENSION);

    // Read user information from a file
    ATMUser* readUser = ATMUser::readFromFile("user_info" + FILE_EXTENSION);
    if (readUser) {
        cout << "\nUser Information Read from File:" << endl;
         readUser->printAccounts();
    }

    return 0;
}
