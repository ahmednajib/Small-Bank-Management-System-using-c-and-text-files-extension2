#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

struct sUsers
{
    string UserName;
    string Password;
    int Permissions = 0;
    bool MarkForDelete = false;
};

sUsers CurrentUser;

//Necessary functions//

void MainPage();
void TransactionMainPage();
void ManageUsersMainPage();
void LoginPage();

vector <string> SplitWords(string S1, string delim)
{
    short pos = 0;
    string Word;
    vector <string> vSplitedWords;

    while ((pos = S1.find(delim)) != std::string::npos)
    {
        Word = S1.substr(0, pos);
        if (Word != "")
        {
            vSplitedWords.push_back(Word);
        }

        S1.erase(0, pos + delim.length());
    }

    if (Word != "")
    {
        vSplitedWords.push_back(S1);
    }

    return vSplitedWords;
}

sClient ConvertLineToRecord(string Line, string delim = "#//#")
{
    sClient Client;
    vector<string> vClientData;

    vClientData = SplitWords(Line, delim);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient ClientData, string delim = "#//#")
{
    string stClientData = "";
    stClientData += ClientData.AccountNumber + delim;
    stClientData += ClientData.PinCode + delim;
    stClientData += ClientData.Name + delim;
    stClientData += ClientData.Phone + delim;
    stClientData += to_string(ClientData.AccountBalance);

    return stClientData;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{

    fstream MyFile;
    vector<sClient> vClients;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string line;
        sClient Client;

        while (getline(MyFile, line))
        {

            Client = ConvertLineToRecord(line);
            vClients.push_back(Client);

        }

        MyFile.close();

    }
    return vClients;

}

void AddDataLineToFile(string FileName, string Line)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << Line << endl;
        MyFile.close();
    }

}

vector <sClient> SaveClientsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string Line = "";

    if (MyFile.is_open())
    {
        for (sClient C : vClients) {

            if (C.MarkForDelete == false)
            {
                Line = ConvertRecordToLine(C);
                MyFile << Line << endl;
            }

        }
        MyFile.close();
    }
    return vClients;
}

//Add New Client// 
bool CheckClientByAccountNumber(string AccountNumber)
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            return true;
        }
    }

    return false;

}

sClient ReadNewClient()
{
    sClient ClientData;

    cout << "Enter Account Number ?  ";
    getline(cin >> ws, ClientData.AccountNumber);

    while (CheckClientByAccountNumber(ClientData.AccountNumber))
    {
        cout << "Client With Account Number[" << ClientData.AccountNumber << "] already exist, try another Account Number ?";
        getline(cin, ClientData.AccountNumber);
    }

    cout << "Enter Pin Code ?  ";
    getline(cin, ClientData.PinCode);

    cout << "Enter Name ?  ";
    getline(cin, ClientData.Name);

    cout << "Enter Phone ?  ";
    getline(cin, ClientData.Phone);

    cout << "Enter Account Balance ?  ";
    cin >> ClientData.AccountBalance;

    return ClientData;

}

void AddNewClient()
{
    sClient sClient;

    sClient = ReadNewClient();

    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(sClient));
}

void AddClients()
{
    char AddMore = 'y';

    do
    {
        cout << "Adding a New Client:\n\n";
        AddNewClient();

        cout << "\nClient Added Successfully, do you want to add more clients? y/press any key: ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');

}

//Show Client List// 

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllCLientsData(vector <sClient> vClients)
{
    cout << "\t\t\t\t\t\tClient list (" << vClients.size() << ") Client(s).";
    cout << "\n\n _______________________________________________________________________________________________________________________\n\n";
    cout << "| " << setw(15) << left << "Account Number";
    cout << "| " << setw(10) << left << "Pin Code";
    cout << "| " << setw(40) << left << "Client Name";
    cout << "| " << setw(12) << left << "Phone";
    cout << "| " << setw(12) << left << "Balance";
    cout << "\n _______________________________________________________________________________________________________________________\n\n";

    for (sClient client : vClients)
    {
        PrintClientRecord(client);
        cout << endl;
    }

    cout << "\n\n _______________________________________________________________________________________________________________________\n\n";
}

void PrintClientCard(sClient Client)
{
    cout << "\n\nThe following are client details :\n";
    cout << "-------------------------------------------------------\n";
    cout << "Account Nnumber: " << Client.AccountNumber << endl;
    cout << "Pin Code: " << Client.PinCode << endl;
    cout << "Name: " << Client.Name << endl;
    cout << "Phone: " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "-----------------------------------------------------\n";
}

//Find Client// 

bool FindClientByAccountNumber(sClient& client, string AccountNumber)
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            client = C;
            return true;
        }
    }

    return false;
}

void ShowFoundClient(string AccountNumber)
{
    sClient Client;
    if (FindClientByAccountNumber(Client, AccountNumber))
    {

        PrintClientCard(Client);
    }
    else
    {
        cout << "Client with account number (" << AccountNumber << ") Not Found!";
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "Please enter Account Number? ";
    cin >> AccountNumber;

    return AccountNumber;
}

//Delete Client//

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

bool DeleteClientByAccountNumber(vector <sClient>& vClients, string AccountNumber)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(Client, AccountNumber))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want to delete this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n\nClient deleted successfully.";
            return true;
        }

    }
    else
    {
        cout << "Client with account number (" << AccountNumber << ") Not Found!";
        return false;
    }
}

//Update Client//

sClient ChangeClientRecord(string AccountNumber)
{
    sClient ClientData;

    ClientData.AccountNumber = AccountNumber;

    cout << "\n\nEnter Pin Code ?  ";
    getline(cin >> ws, ClientData.PinCode);

    cout << "Enter Name ?  ";
    getline(cin, ClientData.Name);

    cout << "Enter Phone ?  ";
    getline(cin, ClientData.Phone);

    cout << "Enter Account Balance ?  ";
    cin >> ClientData.AccountBalance;

    return ClientData;

}

bool UpdateClientByAccountNumber(vector <sClient>& vClients, string AccountNumber)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(Client, AccountNumber))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want to Update this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveClientsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated successfully.";
            return true;
        }

    }
    else
    {
        cout << "Client with account number (" << AccountNumber << ") Not Found!";
        return false;
    }
}

//Main Page//

void ShowMainMenueScreen()
{
    cout << "=================================================================\n";
    cout << "\t\t\tMain Menue Screen";
    cout << "\n=================================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users\n";
    cout << "\t[8] Logout\n";
    cout << "=================================================================\n";

}

short WhatToDo()
{
    short WhatToDo;
    do {

        cout << "\nChoose what do you want to do? [1 to 8]?";
        cin >> WhatToDo;

    } while (WhatToDo < 1 || WhatToDo > 8);

    return WhatToDo;
}

void PressAnyKeyToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Minue...";
    system("pause>0");
    MainPage();
}

void ProcessScreen(string message)
{
    cout << "--------------------------------------- \n";
    cout << "\t" << message << endl;
    cout << "--------------------------------------- \n";
}

//Transaction Menue//

void PressAnyKeyToTransactionMenue()
{
    cout << "\n\nPress any key to go back to Main Minue...";
    system("pause>0");
    TransactionMainPage();
}

short WhatTransactionToDo()
{
    short WhatToDo;
    do {

        cout << "\nChoose what do you want to do? [1 to 4]?";
        cin >> WhatToDo;

    } while (WhatToDo < 1 || WhatToDo > 4);

    return WhatToDo;
}

void ShowTransactionsMenueScreen()
{
    cout << "=================================================================\n";
    cout << "\t\t\tTransactions Menue Screen";
    cout << "\n=================================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "=================================================================\n";
}

double TheAmount()
{
    double Amount = 0;

    cout << "\n\nPlease enter your Amount? ";
    cin >> Amount;

    return Amount;
}

void WithdrawScreen()
{
    ProcessScreen("Withdraw Screen");
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;
    double Amount = 0;
    char Answer = 'n';

    while (!CheckClientByAccountNumber(AccountNumber))
    {
        cout << "Client With Account Number[" << AccountNumber << "] doesn't exist, try another Account Number ?";
        cin >> AccountNumber;
    }

    if (FindClientByAccountNumber(Client, AccountNumber)) {

        PrintClientCard(Client);

        Amount = TheAmount();

        while (Amount > Client.AccountBalance || Amount < 0)
        {
            cout << "\nYour Balance is less than the Amount you entered, you can withdraw up to :" << Client.AccountBalance << "\nPlease enter another amount? ";
            cin >> Amount;
        }

        cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.AccountBalance -= Amount;

                    SaveClientsDataToFile(ClientsFileName, vClients);

                    cout << "\n\nyour amount is withdrawed successfully, your remain balance is  " << C.AccountBalance;
                    break;
                }

            }

        }

    }

}

void DepositScreen()
{
    ProcessScreen("Withdraw Screen");
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    sClient Client;
    double Amount = 0;
    char Answer = 'n';

    while (!CheckClientByAccountNumber(AccountNumber))
    {
        cout << "Client With Account Number[" << AccountNumber << "] doesn't exist, try another Account Number ?";
        cin >> AccountNumber;
    }

    if (FindClientByAccountNumber(Client, AccountNumber)) {

        PrintClientCard(Client);

        Amount = TheAmount();

        while (Amount < 0)
        {
            cout << "\nyou entered a negative number, Try again!..";
            cin >> Amount;
        }

        cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.AccountBalance += Amount;

                    SaveClientsDataToFile(ClientsFileName, vClients);

                    cout << "\n\nyour amount is deposited successfully, your new balance is  " << C.AccountBalance;
                    break;
                }

            }

        }

    }

}

double TotalBalances()
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    double Sum = 0;

    for (sClient& C : vClients)
        Sum += C.AccountBalance;

    return Sum;
}

void PrintClientBalanceRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllCLientsBalances(vector <sClient> vClients)
{
    cout << "\t\t\t\t\t\tBalances list (" << vClients.size() << ") Client(s).";
    cout << "\n\n _______________________________________________________________________________________________________________________\n\n";
    cout << "| " << setw(15) << left << "Account Number";
    cout << "| " << setw(40) << left << "Client Name";
    cout << "| " << setw(12) << left << "Balance";
    cout << "\n _______________________________________________________________________________________________________________________\n\n";

    for (sClient client : vClients)
    {
        PrintClientBalanceRecord(client);
        cout << endl;
    }

    cout << "\n\n _______________________________________________________________________________________________________________________\n\n";
    cout << "Total balances = " << TotalBalances() << endl;
}

void TransactionMainPage()
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    system("cls");
    ShowTransactionsMenueScreen();

    switch (WhatTransactionToDo())
    {
    case 1:
    {
        system("cls");
        DepositScreen();
        PressAnyKeyToTransactionMenue();
        break;
    }

    case 2:
    {
        system("cls");
        WithdrawScreen();
        PressAnyKeyToTransactionMenue();
        break;
    }

    case 3:
    {
        system("cls");
        PrintAllCLientsBalances(vClients);
        PressAnyKeyToTransactionMenue();
        break;
    }

    case 4:
    {
        system("cls");
        MainPage();
        break;
    }

    }
}

// Users //

void PrintUserRecord(sUsers User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;
}

void PrintAllUsersData(vector <sUsers> vUsers)
{
    cout << "\t\t\t\t\t\tUsers list (" << vUsers.size() << ") User(s).";
    cout << "\n\n _______________________________________________________________________________________________________________________\n\n";
    cout << "| " << setw(15) << left << "User Name";
    cout << "| " << setw(10) << left << "Password";
    cout << "| " << setw(40) << left << "Permissions";
    cout << "\n _______________________________________________________________________________________________________________________\n\n";

    for (sUsers User : vUsers)
    {
        PrintUserRecord(User);
        cout << endl;
    }

    cout << "\n\n _______________________________________________________________________________________________________________________\n\n";
}

string ConvertUserRecordToLine(sUsers UserData, string delim = "#//#")
{
    string stUserData = "";
    stUserData += UserData.UserName + delim;
    stUserData += UserData.Password + delim;
    stUserData += to_string(UserData.Permissions);

    return stUserData;
}

sUsers ConvertUserLineToRecord(string Line, string delim = "#//#")
{
    sUsers User;
    vector<string> vUserData;

    vUserData = SplitWords(Line, delim);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

vector <sUsers> LoadUsersDataFromFile(string FileName)
{

    fstream MyFile;
    vector<sUsers> vUsers;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string line;
        sUsers User;

        while (getline(MyFile, line))
        {

            User = ConvertUserLineToRecord(line);
            vUsers.push_back(User);

        }

        MyFile.close();

    }
    return vUsers;

}

bool UserExistByUserName(string UserName)
{
    vector <sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    for (sUsers C : vUsers)
    {
        if (C.UserName == UserName)
        {
            return true;
        }
    }

    return false;

}

short ReadPermissionToSet()
{
    char Answer = 'N';
    short Permissions = 0;
    cout << "\n\nDo you want to give full access? Y/N? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y') {
        Permissions = -1;
    }
    else
    {
        cout << "\n\nDo you want to give access to :";

        cout << "\n\nShow Client List? Y/N?";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            Permissions += 1;
        }

        cout << "\n\nAdd New Client? Y/N?";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            Permissions += 2;
        }

        cout << "\n\nDelete Client? Y/N?";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            Permissions += 4;
        }

        cout << "\n\nUpdate Client? Y/N?";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            Permissions += 8;
        }

        cout << "\n\nFind Client? Y/N?";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            Permissions += 16;
        }

        cout << "\n\nTransactions? Y/N?";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            Permissions += 32;
        }

        cout << "\n\nManage Users? Y/N?";
        cin >> Answer;
        if (toupper(Answer) == 'Y') {
            Permissions += 64;
        }

    }

    return Permissions;
}

sUsers ReadNewUser()
{
    sUsers UserData;

    cout << "Enter User Name ?  ";
    getline(cin >> ws, UserData.UserName);

    while (UserExistByUserName(UserData.UserName))
    {
        cout << "User With User Name [" << UserData.UserName << "] already exist, try another User Name ?";
        getline(cin, UserData.UserName);
    }

    cout << "Enter Password ?  ";
    getline(cin, UserData.Password);

    UserData.Permissions = ReadPermissionToSet();

    return UserData;

}

void AddNewUser()
{
    sUsers sUser;

    sUser = ReadNewUser();

    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(sUser));
}

void AddUsers()
{
    char AddMore = 'y';

    do
    {
        cout << "\nAdding a New User:\n\n";
        AddNewUser();

        cout << "\nUser Added Successfully, do you want to add more users? Y/N: ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');

}

string ReadUserName()
{
    string UserName = "";

    cout << "Please enter UserName? ";
    cin >> UserName;

    return UserName;
}

bool FindUserByUserName(sUsers& User, string UserName)
{
    vector <sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    for (sUsers U : vUsers)
    {
        if (U.UserName == UserName)
        {
            User = U;
            return true;
        }
    }

    return false;
}

void PrintUserCard(sUsers User)
{
    cout << "\n\nThe following are User details :\n";
    cout << "-------------------------------------------------------\n";
    cout << "User Name: " << User.UserName << endl;
    cout << "Password: " << User.Password << endl;
    cout << "Permissions: " << User.Permissions << endl;
    cout << "-----------------------------------------------------\n";
}

void ShowFoundUser(string UserName)
{
    sUsers User;
    if (FindUserByUserName(User, UserName))
    {

        PrintUserCard(User);
    }
    else
    {
        cout << "User with User Name (" << UserName << ") Not Found!";
    }
}

vector <sUsers> SaveUsersDataToFile(string FileName, vector <sUsers> vUsers)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string Line = "";

    if (MyFile.is_open())
    {
        for (sUsers U : vUsers) {

            if (U.MarkForDelete == false)
            {
                Line = ConvertUserRecordToLine(U);
                MyFile << Line << endl;
            }

        }
        MyFile.close();
    }
    return vUsers;
}

bool MarkUserForDeleteByUserName(string UserName, vector <sUsers>& vUsers)
{
    for (sUsers& U : vUsers)
    {
        if (U.UserName == UserName)
        {
            U.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

bool DeleteUserByUserName(vector <sUsers>& vUsers, string UserName)
{
    sUsers User;
    char Answer = 'n';

    if (FindUserByUserName(User, UserName))
    {
        if (User.Permissions == -1)
        {
            cout << "\nYou Can't delete an admin!!";
        }
        else
        {
            PrintUserCard(User);
            cout << "\n\nAre you sure you want to delete this User? y/n ? ";
            cin >> Answer;

            if (Answer == 'y' || Answer == 'Y')
            {
                MarkUserForDeleteByUserName(UserName, vUsers);
                SaveUsersDataToFile(UsersFileName, vUsers);

                vUsers = LoadUsersDataFromFile(UsersFileName);
                cout << "\n\nUser deleted successfully.";
                return true;
            }
        }
    }
    else
    {
        cout << "User with User Name (" << UserName << ") Not Found!";
        return false;
    }
}

sUsers ChangeUserRecord(string UserName)
{
    sUsers UserData;

    UserData.UserName = UserName;

    cout << "\n\nEnter Password ?  ";
    getline(cin >> ws, UserData.Password);

    UserData.Permissions = ReadPermissionToSet();

    return UserData;

}

bool UpdateUserByUserName(vector <sUsers>& vUsers, string UserName)
{
    sUsers User;
    char Answer = 'n';

    if (FindUserByUserName(User, UserName))
    {

        PrintUserCard(User);

        cout << "\n\nAre you sure you want to Update this user? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {

            for (sUsers& U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U = ChangeUserRecord(UserName);
                    break;
                }

            }

            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated successfully.";
            return true;
        }

    }
    else
    {
        cout << "User with User Name (" << UserName << ") Not Found!";
        return false;
    }
}

void PressAnyKeyToManageUsersMainMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Main Page...";
    system("pause>0");
    ManageUsersMainPage();
}

void ShowManageUsersMenueScreen()
{
    cout << "=================================================================\n";
    cout << "\t\t\tManage Users Menue Screen";
    cout << "\n=================================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "=================================================================\n";
}

short WhatManageUsersToDo()
{
    short WhatToDo;
    do {

        cout << "\nChoose what do you want to do? [1 to 6]?";
        cin >> WhatToDo;

    } while (WhatToDo < 1 || WhatToDo > 6);

    return WhatToDo;
}

void ManageUsersMainPage()
{
    vector <sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName;
    system("cls");
    ShowManageUsersMenueScreen();

    switch (WhatManageUsersToDo())
    {
    case 1:
    {
        system("cls");
        PrintAllUsersData(vUsers);
        PressAnyKeyToManageUsersMainMenue();
        break;
    }

    case 2:
    {
        system("cls");
        ProcessScreen("Add New Users Screen");
        AddUsers();
        PressAnyKeyToManageUsersMainMenue();
        break;
    }

    case 3:
    {
        system("cls");
        ProcessScreen("Delete User Screen");
        UserName = ReadUserName();
        DeleteUserByUserName(vUsers, UserName);
        PressAnyKeyToManageUsersMainMenue();
        break;
    }

    case 4:
    {
        system("cls");
        ProcessScreen("Update User Info Screen");
        UserName = ReadUserName();
        UpdateUserByUserName(vUsers, UserName);
        PressAnyKeyToManageUsersMainMenue();
        break;
    }

    case 5:
    {
        system("cls");
        ProcessScreen("Find User Screen");
        UserName = ReadUserName();
        ShowFoundUser(UserName);
        PressAnyKeyToManageUsersMainMenue();
        break;
    }

    case 6:
    {
        system("cls");
        MainPage();
        break;
    }

    }
}

void AccessDeniedScreen()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Contact Your Admin.";
    cout << "\n------------------------------------\n";
}

void MainPage()
{
    system("cls");
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = "";
    ShowMainMenueScreen();

    short Position = WhatToDo();

    switch (Position)
    {
    case 1:
    {
        if (CurrentUser.Permissions == -1 || (CurrentUser.Permissions & 1) == 1) {
            system("cls");
            PrintAllCLientsData(vClients);
            PressAnyKeyToMainMenue();
        }
        else
        {
            system("cls");
            AccessDeniedScreen();
            PressAnyKeyToMainMenue();
        }
        break;
    }

    case 2:
    {
        if (CurrentUser.Permissions == -1 || (CurrentUser.Permissions & 2) == 2)
        {
            system("cls");
            ProcessScreen("Add New Clients Screen");
            AddClients();
            PressAnyKeyToMainMenue();
        }
        else
        {
            system("cls");
            AccessDeniedScreen();
            PressAnyKeyToMainMenue();
        }
        break;
    }

    case 3:
    {
        if (CurrentUser.Permissions == -1 || (CurrentUser.Permissions & 4) == 4)
        {
            system("cls");
            ProcessScreen("Delete Client Screen");
            AccountNumber = ReadClientAccountNumber();
            DeleteClientByAccountNumber(vClients, AccountNumber);
            PressAnyKeyToMainMenue();
        }
        else
        {
            system("cls");
            AccessDeniedScreen();
            PressAnyKeyToMainMenue();
        }
        break;
    }

    case 4:
    {
        if (CurrentUser.Permissions == -1 || (CurrentUser.Permissions & 8) == 8)
        {
            system("cls");
            ProcessScreen("Update Client Info Screen");
            AccountNumber = ReadClientAccountNumber();
            UpdateClientByAccountNumber(vClients, AccountNumber);
            PressAnyKeyToMainMenue();
        }
        else
        {
            system("cls");
            AccessDeniedScreen();
            PressAnyKeyToMainMenue();
        }
        break;
    }

    case 5:
    {
        if (CurrentUser.Permissions == -1 || (CurrentUser.Permissions & 16) == 16)
        {
            system("cls");
            ProcessScreen("Find Client Screen");
            AccountNumber = ReadClientAccountNumber();
            ShowFoundClient(AccountNumber);
            PressAnyKeyToMainMenue();
        }
        else
        {
            system("cls");
            AccessDeniedScreen();
            PressAnyKeyToMainMenue();
        }
        break;
    }

    case 6:
    {
        if (CurrentUser.Permissions == -1 || (CurrentUser.Permissions & 32) == 32)
        {
            TransactionMainPage();
        }
        else
        {
            system("cls");
            AccessDeniedScreen();
            PressAnyKeyToMainMenue();
        }
        break;
    }

    case 7:
    {
        if (CurrentUser.Permissions == -1 || (CurrentUser.Permissions & 64) == 64)
        {
            system("cls");
            ManageUsersMainPage();
        }
        else
        {
            system("cls");
            AccessDeniedScreen();
            PressAnyKeyToMainMenue();
        }
        break;
    }

    case 8:
    {
        LoginPage();
        break;
    }

    }

}

bool FindUserByUserNameAndPassword(string UserName, string Password, sUsers& User)
{
    vector <sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUsers& U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U;
            return true;
        }
    }

    return false;
}

void LoginPage()
{

    bool LoginFaild = false;

    string Username, Password;

    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;

        LoginFaild = !FindUserByUserNameAndPassword(Username, Password, CurrentUser);

    } while (LoginFaild);

    MainPage();

}

int main()
{
    LoginPage();

    system("pause>0");
    return 0;

}