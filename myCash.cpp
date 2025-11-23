// 2103024
// Dewan Md. Nafis Iqbal
#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

int trsId = 101;

class History
{
public:
    int tranID;
    string description;
    double amount;
    double balance;

    History(int tranID, string description, double amount, double balance)
        : tranID(tranID), description(description), amount(amount), balance(balance) {}

    void saveToFile()
    {
        ofstream outFile("history.txt", ios::app);
        if (outFile.is_open())
        {
            outFile << tranID << "\t" << description << "\t" << amount << "\t" << balance << endl;
            outFile.close();
        }
    }
};

// forward declaration so Members methods can call it
void saveMembersToFile();

class Members
{
    string uMobile, uName, uPass;
    double uAmount;
    bool uExist;

public:
    vector<History> transactionHistory;

    Members(string uMobile, string uName, string uPass, bool uExist)
        : uMobile(uMobile), uName(uName), uPass(uPass), uExist(uExist), uAmount(0.0) {}

    void performTransaction(int tranID, string desc, double amount)
    {
        uAmount += amount;
        History transaction(tranID, desc, amount, uAmount);
        transactionHistory.push_back(transaction);
        transaction.saveToFile();
        // persist member balances immediately so Members.txt reflects changes during runtime
        saveMembersToFile();
    }

    void showTransHistory()
    {
        cout << "Transaction history" << endl;
        cout << "Tran ID\tDescription\tAmount\tBalance\n";
        for (const auto &transaction : transactionHistory)
        {
            cout << transaction.tranID << "\t" << transaction.description << "\t"
                 << transaction.amount << "\t" << transaction.balance << endl;
        }
    }

    void setName(string n) { uName = n; }
    void setPass(string p) { uPass = p; }
    void setExist(bool uExist) { this->uExist = uExist; }
    void setAmount(float a) { uAmount = a; }

    string getMobile() { return uMobile; }
    string getName() { return uName; }
    string getPass() { return uPass; }
    double getAmount() { return uAmount; }
};

vector<Members> users;

void saveMembersToFile()
{
    ofstream mainFile("Members.txt");
    if (mainFile.is_open())
    {
        mainFile << "Moble no\tUsername\tAmount\tPin\n";
        for (auto &user : users)
        {
            mainFile << user.getMobile() << "\t" << user.getName()
                     << "\t" << user.getAmount() << "\t" << user.getPass() << "\n";
        }
        mainFile.close();
    }
}

int search(string mob)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users.at(i).getMobile() == mob)
            return i;
    }
    return -1;
}

string gOTP()
{
    srand(time(0));
    string otp;
    for (int i = 0; i < 6; i++)
        otp += to_string(rand() % 10);
    return otp;
}

string setPass()
{
    string pass;
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == '\r')
            break;
        else if (ch == 8 && !pass.empty())
        {
            pass.pop_back();
            cout << "\b \b";
        }
        else
        {
            pass.push_back(ch);
            cout << '*';
        }
    }
    return pass;
}

void Register()
{
    string mobile;
    string name;
    cout << "Enter mobile no. (11-digit): ";
    cin >> mobile;

    while (mobile.at(0) != '0' || mobile.at(1) != '1' || mobile.length() != 11)
    {
        cout << "Invalid mobile no." << endl;
        cout << "Enter mobile no. (11-digit): ";
        cin >> mobile;
    }

    // if (mobile.at(0) != '0' || mobile.at(1) != '1' || mobile.length() != 11)
    // {
    //     cout << "Invalid mobile no." << endl;
    //     getch();
    //     return;
    // }

    if (search(mobile) >= 0)
    {
        cout << "Member already exists" << endl;
        getch();
        return;
    }
    cout << "Enter name: ";
    cin.ignore(); // Ignore any remaining newline characters in the input buffer.
    getline(cin, name);

    string pass;
    string confirmPass;

    cout << "Enter password: ";
    pass = setPass();
    char ch;

    cout << "\nConfirm password: ";
    confirmPass = setPass();

    while (pass != confirmPass)
    {
        cout << "\nPasswords do not match.\n Reenter password: ";
        pass = "";        // Clear the password input.
        confirmPass = ""; // Clear the confirmation input.
        // Re-enter the password.
        pass = setPass();
        cout << "\nConfirm password: ";
        confirmPass = setPass();
    }
    cout << endl;
    // generating otp
    string otp = gOTP();
    cout << "MyCash OTP: " << otp;
    string confirmOtp;
    cout << endl;
    cout << "Enter otp: ";
    cin >> confirmOtp;
    cout << endl;

    while (true)
    {
        if (confirmOtp != otp)
        {
            cout << "Wrong otp.\nNew ";
            otp = gOTP();
            cout << "OTP: " << otp;
            cout << endl;
            confirmOtp = "";
            cout << "Enter otp: ";
            cin >> confirmOtp;
            cout << endl;
        }
        else
            break;
    }

    users.push_back(Members(mobile, name, pass, true));
    // persist new member immediately
    saveMembersToFile();
    cout << "Registration successful" << endl;
    system("pause");
}

void update(int i)
{
    cout << "Old Name: " << users.at(i).getName() << endl;
    cout << "New Name (enter to ignore): ";
    string newName;
    cin.ignore(); // Ignore any remaining newline characters in the input buffer.
    getline(cin, newName);
    if (newName != "")
    {
        users.at(i).setName(newName);
    }
    // code for setting the pin;
    cout << "Old pin: ";
    string oldPass;
    oldPass = setPass();
    if (oldPass == users.at(i).getPass())
    {
        cout << "\nNew pin (enter to ignore): ";
        string newPass;
        newPass = setPass();
        if (newPass != "")
        {
            string confirmPass;
            cout << "\nConfirm New pin: ";
            confirmPass = setPass();
            if (confirmPass == newPass)
            {
                users.at(i).setPass(newPass);
            }
        }
    }
    string otp = gOTP();
    cout << "\nMyCash OTP: " << otp << "\nEnter OTP:" << endl;
    string confirmOtp;
    cin >> confirmOtp;
    if (otp == confirmOtp)
    {
        // save updated member info
        saveMembersToFile();
        cout << "Update is successful.\nPress any key to go to main Menu." << endl;
        getch();
    }
    else
        return;
}

void removeMe(int id)
{
    string otp = gOTP();
    cout << "MyCash OTP: " << otp << "\nEnter OTP:" << endl;
    string confirmOtp;
    cin >> confirmOtp;
    if (otp == confirmOtp)
    {
        users.at(id).setExist(false);
        // persist removal immediately
        saveMembersToFile();
        cout << "Remove is successful.\nBack to MyCash Login Menu" << endl;
        getch();
    }
    else
        return;
}

void sendMoney(int id)
{
    string mobile;
    int amount;
    cout << "Enter Destination no. (11-digit):";
    cin >> mobile;

    if (mobile.at(0) != '0' || mobile.at(1) != '1' || mobile.length() != 11)
    {
        cout << "Invalid mobile no." << endl;
        getch();
        return;
    }
    int j = search(mobile);
    if (j < 0)
    {
        cout << "Account doesn't exists." << endl;
        getch();
        return;
    }
    cout << "Enter Amount:";
    cin >> amount;
    if (users.at(id).getAmount() < amount)
    {
        cout << "Not enough money." << endl;
        getch();
        return;
    }
    cout << "Sending " << amount << " to " << mobile << endl;
    cout << "Are you sure(Y/N)? ";
    char ch;
    cin >> ch;
    if (ch == 'Y' || ch == 'y')
    {

        string otp = gOTP();
        cout << "MyCash OTP: " << otp << "\nEnter OTP:";
        string confirmOtp;
        cin >> confirmOtp;
        if (otp == confirmOtp)
        {
            users.at(id).performTransaction(trsId, "Send Money", -amount);
            users.at(j).performTransaction(trsId, "Send Money", amount);
            trsId++;
            cout << "Send Money is Successful\nPress any key to go to main menu...." << endl;
            getch();
            return;
        }
        else
            return;
    }
    else if (ch == 'N' || ch == 'n')
    {
        cout << "Send Money is Canceled\nPress any key to go to main menu...." << endl;
        getch();
        return;
    }
}

void cashIn(int id)
{
    float amount;
    cout << "Enter amount: ";
    cin >> amount;
    cout << "Cash-in " << amount << endl;
    cout << "Are you sure(Y/N)? ";
    char ch;
    cin >> ch;
    if (ch == 'Y' || ch == 'y')
    {
        users.at(id).performTransaction(trsId++, "Cash-in", amount);
        cout << "Cash-in is Successful\nPress any key go to main menu....." << endl;
        getch();
    }
    else
    {
        cout << "Cash-in is Canceled\nPress any key go to main menu....." << endl;
        getch();
        return;
    }
}

void cashOut(int id)
{
    float amount;
    cout << "Enter amount: ";
    cin >> amount;
    cout << "Cash-out " << amount << endl;
    cout << "Are you sure(Y/N)? ";
    char ch;
    cin >> ch;
    if (ch == 'Y' || ch == 'y')
    {
        if (users.at(id).getAmount() < amount)
        {
            cout << "Not enough money." << endl;
            getch();
            return;
        }
        else
        {
            string otp = gOTP();
            cout << "MyCash OTP: " << otp << "\nEnter OTP:";
            string confirmOtp;
            cin >> confirmOtp;
            if (otp == confirmOtp)
            {
                users.at(id).performTransaction(trsId++, "Cash-out", -amount);
                cout << "Cash-out is Successful\nPress any key go to main menu....." << endl;
                getch();
            }
        }
    }
    else
    {
        cout << "Cash-out is Canceled\nPress any key go to main menu....." << endl;
        getch();
        return;
    }
}

void payBill(int j)
{
    cout << "Enter Bill Type (Gas/Electricity/Water/Internet-1/2/3/4): ";
    int billType;
    cin >> billType;
    switch (billType)
    {
        char ch;
    case 1:
        cout << "Your Gas Bill: 1000" << endl;
        cout << "Want to pay(Y/N)? ";
        cin >> ch;
        if (ch == 'Y' || ch == 'y')
        {
            if (users.at(j).getAmount() < 1000)
            {
                cout << "Not enough money." << endl;
                getch();
                return;
            }
            string otp = gOTP();
            cout << "MyCash OTP: " << otp << "\nEnter OTP:";
            string confirmOtp;
            cin >> confirmOtp;
            if (otp == confirmOtp)
            {
                cout << "Bill Payment is Successful" << endl;
                users.at(j).performTransaction(trsId++, "Gas Bill", -1000);
                cout << "Press any key go to main menu....." << endl;
                getch();
                return;
            }
            else
            {
                cout << "Wrong OTP" << endl;
                getch();
                return;
            }
        }
        else
        {
            cout << "Bill Payment is Canceled" << endl;
            cout << "Press any key go to main menu....." << endl;
            getch();
            return;
        }
        break;
    case 2:
        cout << "Your Electricity Bill: 980" << endl;
        cout << "Want to pay(Y/N)? ";
        cin >> ch;
        if (ch == 'Y' || ch == 'y')
        {
            if (users.at(j).getAmount() < 980)
            {
                cout << "Not enough money." << endl;
                getch();
                return;
            }

            string otp = gOTP();
            cout << "MyCash OTP: " << otp << "\nEnter OTP:";
            string confirmOtp;
            cin >> confirmOtp;
            if (otp == confirmOtp)
            {
                cout << "Bill Payment is Successful" << endl;
                users.at(j).performTransaction(trsId++, "Elec Bill", -980);
                cout << "Press any key go to main menu....." << endl;
                getch();
                return;
            }
            else
            {
                cout << "Wrong OTP" << endl;
                getch();
                return;
            }
        }
        else
        {
            cout << "Bill Payment is Canceled" << endl;
            cout << "Press any key go to main menu....." << endl;
            getch();
            return;
        }
        break;
    case 3:
        cout << "Your Water Bill: 400" << endl;
        cout << "Want to pay(Y/N)? ";
        cin >> ch;
        if (ch == 'Y' || ch == 'y')
        {
            if (users.at(j).getAmount() < 400)
            {
                cout << "Not enough money." << endl;
                getch();
                return;
            }
            string otp = gOTP();
            cout << "MyCash OTP: " << otp << "\nEnter OTP:";
            string confirmOtp;
            cin >> confirmOtp;
            if (otp == confirmOtp)
            {
                cout << "Bill Payment is Successful" << endl;
                users.at(j).performTransaction(trsId++, "Water Bill", -400);
                cout << "Press any key go to main menu....." << endl;
                getch();
                return;
            }
            else
            {
                cout << "Wrong OTP" << endl;
                getch();
                return;
            }
        }
        else
        {
            cout << "Bill Payment is Canceled" << endl;
            cout << "Press any key go to main menu....." << endl;
            getch();
            return;
        }
        break;
    case 4:
        cout << "Your Internet Bill: 1200" << endl;
        cout << "Want to pay(Y/N)? ";
        cin >> ch;
        if (ch == 'Y' || ch == 'y')
        {
            if (users.at(j).getAmount() < 1200)
            {
                cout << "Not enough money." << endl;
                getch();
                return;
            }

            string otp = gOTP();
            cout << "MyCash OTP: " << otp << "\nEnter OTP:";
            string confirmOtp;
            cin >> confirmOtp;
            if (otp == confirmOtp)
            {
                cout << "Bill Payment is Successful" << endl;
                users.at(j).performTransaction(trsId++, "INet Bill", -1200);
                cout << "Press any key go to main menu....." << endl;
                getch();
                return;
            }
            else
            {
                cout << "Wrong OTP" << endl;
                getch();
                return;
            }
        }
        else
        {
            cout << "Bill Payment is Canceled" << endl;
            cout << "Press any key go to main menu....." << endl;
            getch();
            return;
        }
        break;
    default:
        break;
    }
}

void checkBalance(int id)
{
    cout << "Balance: " << users.at(id).getAmount() << endl;
    cout << "Press any key go to main menu....." << endl;
    getch();
    return;
}

void history(int id)
{
    users.at(id).showTransHistory();
    cout << "Press any key go to main menu....." << endl;
    getch();
}

void login()
{
    cout << "Enter mobile no. (11-digit): ";
    string mobile;
    cin.ignore(); // Ignore any remaining newline characters in the input buffer.
    getline(cin, mobile);
    // cin >> mobile;
    if (mobile.at(0) != '0' || mobile.at(1) != '1' || mobile.length() != 11)
    {
        cout << "Invalid mobile no." << endl;
        getch();
        return;
    }

    int j = search(mobile);
    if (j < 0)
    {
        cout << "Account doesn't exists." << endl;
        getch();
        return;
    }

    string pass;
    char ch;
    cout << "Enter password: ";
    while (true)
    {
        ch = _getch();
        if (ch == '\r')
        {
            break; // Enter key, end password input
        }
        else if (ch == 8)
        { // Backspace key
            if (!pass.empty())
            {
                pass.pop_back(); // Remove the last character from the password
                cout << "\b \b"; // Move the cursor back, erase the character, and move the cursor back again
            }
        }
        else
        {
            pass.push_back(ch); // Add the character to the password
            cout << '*';        // Display an asterisk for each character
        }
    }

    if (pass != users.at(j).getPass())
    {
        cout << "Invalid password." << endl;
        getch();
        return;
    }

    int opt;
    do
    {
        system("cls");
        cout << "********** MyCash Menu ********" << endl;
        cout << "1. Update Me   " << endl;
        cout << "2. Remove Me   " << endl;
        cout << "3. Send Money  " << endl;
        cout << "4. Cash-in  " << endl;
        cout << "5. Cash-out  " << endl;
        cout << "6. Pay Bill  " << endl;
        cout << "7. Check Balance  " << endl;
        cout << "8. History  " << endl;
        cout << "9. Logout  " << endl;
        cout << "Enter Your Option (1-9): ";
        cin >> opt;
        switch (opt)
        {
        case 1:
            update(j);
            break;
        case 2:
            removeMe(j);
            break;
        case 3:
            sendMoney(j);
            break;
        case 4:
            cashIn(j);
            break;
        case 5:
            cashOut(j);
            break;
        case 6:
            payBill(j);
            break;
        case 7:
            checkBalance(j);
            break;
        case 8:
            history(j);
            break;
        case 9:
            return;
            break;
        }
    } while (1);
}
int Menu()
{
    int op;
    do
    {
        system("cls");
        cout << "\t*** MyCash Login***" << endl;
        cout << "\t1. Login" << endl;
        cout << "\t2. Register" << endl;
        cout << "\t3. Exit" << endl;
        cout << "\tEnter Your Option: ";
        cin >> op;
        switch (op)
        {
        case 1:
            login();
            break;
        case 2:
            Register();
            break;
        case 3:
            saveMembersToFile();
            exit(0);
        }
    } while (1);
}
int main()
{
    Menu();
    return 0;
}