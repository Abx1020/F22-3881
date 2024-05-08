#include "Header.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include<sstream>
#include <unordered_map>
using namespace std;

int main()
{
    UserManagement userManager;
    Repository repo[100];
    HashTable obj;
    int hashVal = 0;
    int choice;
    string username, password;

    ifstream repoNameFile("repositorynames.txt");
    string repoName;

    ofstream repoNameFileOut;

    do
    {
        try
        {
            cout << "Menu:\n1. Register\n2. Login\n3. Exit\nEnter your choice: ";
            cin >> choice;

            if (cin.fail())
            {
                // Clear input buffer
                cin.clear();
                cin.ignore(10000, '\n');
                throw runtime_error("Invalid input. Please enter a number.");
            }

            switch (choice)
            {
            case 1:
                // User Registration
                cout << "User Registration\n";
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                hashVal = obj.gethash(username);
                userManager.registerUser(username, password);
                break;
            case 2:
                // User Login
                cout << "User Login\n";
                cout << "Enter username: ";
                cin >> username;
                hashVal = obj.gethash(username);
                cout << "Enter password: ";
                cin >> password;

                if (userManager.loginUser(username, password))
                {
                    cout << "Login successful. Welcome, " << username << "!" << endl;


                    do
                    {
                        cout << "Menu:\n1. Create new repository\n2. Add file to repository\n3. Delete file from repository\n4. Display contents of repository\n5. Logout\n6. Follow A User\n7. Unfollow A User\nEnter your choice: ";
                        cin >> choice;
                        Graph obj; string temp;
                        if (cin.fail())
                        {
                            // Clear input buffer
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid input. Please enter a number." << endl;
                            continue;
                        }

                        switch (choice)
                        {
                        case 1:
                            cout << "Enter repository name to add: ";
                            cin.ignore();
                            getline(cin, repoName);
                            repo[hashVal].createRepository(repoName, hashVal);

                            // Append the repository name to repositorynames.txt
                            repoNameFileOut.open("repositorynames.txt", ios::app);
                            if (repoNameFileOut.is_open())
                            {
                                repoNameFileOut << repoName << endl;
                                repoNameFileOut.close();
                            }
                            else
                            {
                                cout << "Error: Unable to open repositorynames.txt for appending." << endl;
                            }
                            break;
                        case 2:
                            cout << "Enter repository index to add file: ";
                            if (!(cin >> choice))
                            {
                                cout << "Invalid input. Please enter a number." << endl;
                                cin.clear();
                                cin.ignore(10000, '\n');
                                continue;
                            }
                            cout << "Enter project name to add: ";
                            cin >> repoName;
                            repo[hashVal].addFileToRepository(choice, repoName, hashVal);
                            break;
                        case 3:
                            cout << "Enter repository index to delete file from: ";
                            if (!(cin >> choice))
                            {
                                cout << "Invalid input. Please enter a number." << endl;
                                cin.clear();
                                cin.ignore(10000, '\n');
                                continue;
                            }
                            cin.ignore();
                            cout << "Enter project name to delete: ";
                            getline(cin, repoName);
                            repo[hashVal].deleteFileFromRepository(choice, repoName, hashVal);
                            break;
                        case 4:
                            cout << "Enter repository index to display: ";
                            if (!(cin >> choice))
                            {
                                cout << "Invalid input. Please enter a number." << endl;
                                cin.clear();
                                cin.ignore(10000, '\n');
                                continue;
                            }
                            repo[hashVal].displayRepository(choice, hashVal);
                            break;
                        case 5:
                            userManager.logoutUser(username);
                            cout << "Logged out successfully." << endl;
                            break;
                        case 6:

                            cout << "Enter the username you want to follow:\n";
                            cin >> temp;
                            obj.followUser(username, temp);
                        case 7:

                            cout << "Enter the username you want to unfollow:\n";
                            cin >> temp;
                            obj.unfollowUser(username, temp);
                        default:
                            cout << "Invalid choice. Please enter again." << endl;
                        }
                    } while (choice != 5);

                }
                else {
                    cout << "Login failed. Invalid username or password." << endl;
                }
                break;
            case 3:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter again." << endl;
            }
        }
        catch (const exception& e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    } while (choice != 3);
    system("pause");
    return 0;
}